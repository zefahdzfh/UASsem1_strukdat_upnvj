#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- STRUKTUR DATA ---

// 1. LINKED LIST: Master Data Barang (GUDANG)
// Ini buat nyimpen Stok Supermarket (Fitur Wajib: Search & View All)
typedef struct BarangGudang {
    int id;
    char nama[50];
    int harga;
    int stok;
    struct BarangGudang *next;
} BarangGudang;

// 2. LINKED LIST: Node untuk Barang di Keranjang Belanjaan User
typedef struct Item {
    char nama[50];
    int harga;
    int qty;
    struct Item *next;
} Item;

// 3. QUEUE: Node untuk Antrian Pelanggan
typedef struct Pelanggan {
    char namaPelanggan[50];
    Item *keranjangHead; 
    struct Pelanggan *next;
} Pelanggan;

// 4. STACK: Node untuk Riwayat Transaksi
typedef struct Riwayat {
    char namaPelanggan[50];
    int totalBelanja;
    struct Riwayat *next;
} Riwayat;

// --- VARIABEL GLOBAL ---

// Head untuk Gudang (Master Data)
BarangGudang *gudangHead = NULL;

// Queue Global (Antrian Kasir)
Pelanggan *front = NULL;
Pelanggan *rear = NULL;

// Stack Global (Riwayat Penjualan)
Riwayat *top = NULL;

// --- FUNGSI BANTUAN ---

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    printf("\nTekan Enter untuk lanjut...");
    getchar(); getchar();
}

// --- BAGIAN 1: GUDANG (INIT, SHOW ALL, SEARCH) ---
// Ini fitur yang diminta Dosen (Menampilkan & Mencari Data)

void tambahBarangGudang(int id, char *nama, int harga, int stok) {
    BarangGudang *baru = (BarangGudang*)malloc(sizeof(BarangGudang));
    baru->id = id;
    strcpy(baru->nama, nama);
    baru->harga = harga;
    baru->stok = stok;
    baru->next = NULL;

    if (gudangHead == NULL) {
        gudangHead = baru;
    } else {
        BarangGudang *temp = gudangHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = baru;
    }
}

// Inisialisasi Data Awal (Biar pas run gak kosong)
void initGudang() {
    tambahBarangGudang(1, "Indomie", 3500, 50);
    tambahBarangGudang(2, "Telur", 2000, 100);
    tambahBarangGudang(3, "Minyak", 15000, 20);
    tambahBarangGudang(4, "Beras", 60000, 10);
    tambahBarangGudang(5, "Gula", 12000, 30);
}

// FITUR WAJIB 1: Menampilkan Data Keseluruhan
void lihatSemuaStok() {
    printf("\n=== DAFTAR STOK BARANG (GUDANG) ===\n");
    printf("%-5s %-20s %-10s %-5s\n", "ID", "Nama Barang", "Harga", "Stok");
    printf("--------------------------------------------\n");
    
    BarangGudang *temp = gudangHead;
    while (temp != NULL) {
        printf("%-5d %-20s Rp%-9d %-5d\n", temp->id, temp->nama, temp->harga, temp->stok);
        temp = temp->next;
    }
    printf("--------------------------------------------\n");
}

// FITUR WAJIB 2: Mencari Data (Search Linear)
void cariBarang() {
    char keyword[50];
    int found = 0;
    
    printf("\nMasukkan Nama Barang yang dicari: ");
    scanf(" %[^\n]s", keyword); // Baca string dengan spasi

    BarangGudang *temp = gudangHead;
    printf("\n--- HASIL PENCARIAN ---\n");
    while (temp != NULL) {
        // strcasecmp/stricmp untuk ignore case sensitive (tergantung compiler)
        // Kita pakai strstr buat cari substring (misal cari "Indo" ketemu "Indomie")
        if (strstr(temp->nama, keyword) != NULL) {
            printf("[DITEMUKAN] ID: %d | Nama: %s | Stok: %d | Harga: Rp %d\n", 
                   temp->id, temp->nama, temp->stok, temp->harga);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("Barang dengan kata kunci '%s' tidak ditemukan.\n", keyword);
    }
}

// Helper: Ambil barang by ID buat belanja
BarangGudang* ambilBarangByID(int id) {
    BarangGudang *temp = gudangHead;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// --- BAGIAN 2: KERANJANG BELANJA (LINKED LIST) ---

void tambahKeKeranjang(Item **head, char *nama, int harga, int qty) {
    Item *newItem = (Item*)malloc(sizeof(Item));
    strcpy(newItem->nama, nama);
    newItem->harga = harga;
    newItem->qty = qty;
    newItem->next = NULL;

    if (*head == NULL) {
        *head = newItem;
    } else {
        Item *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newItem;
    }
    printf("  [+] %s (x%d) masuk keranjang!\n", nama, qty);
}

int hitungTotalKeranjang(Item *head) {
    int total = 0;
    Item *temp = head;
    while (temp != NULL) {
        total += (temp->harga * temp->qty);
        temp = temp->next;
    }
    return total;
}

void lihatKeranjang(Item *head) {
    if (head == NULL) {
        printf("  (Keranjang Kosong)\n");
        return;
    }
    Item *temp = head;
    int i = 1;
    while (temp != NULL) {
        printf("  %d. %s \t x%d \t Rp %d\n", i++, temp->nama, temp->qty, temp->harga * temp->qty);
        temp = temp->next;
    }
    printf("  --------------------------------\n");
    printf("  TOTAL SEMENTARA: Rp %d\n", hitungTotalKeranjang(head));
}

// --- BAGIAN 3: ANTRIAN KASIR (QUEUE) ---

void masukAntrian(char *nama, Item *keranjang) {
    Pelanggan *newPelanggan = (Pelanggan*)malloc(sizeof(Pelanggan));
    strcpy(newPelanggan->namaPelanggan, nama);
    newPelanggan->keranjangHead = keranjang;
    newPelanggan->next = NULL;

    if (rear == NULL) {
        front = rear = newPelanggan;
    } else {
        rear->next = newPelanggan;
        rear = newPelanggan;
    }
    printf("\n[INFO] %s masuk antrian kasir!\n", nama);
}

void lihatAntrian() {
    if (front == NULL) {
        printf("  [Antrian Kosong]\n");
    } else {
        Pelanggan *temp = front;
        int i = 1;
        while (temp != NULL) {
            printf("  %d. %s (Total: Rp %d)\n", i++, temp->namaPelanggan, hitungTotalKeranjang(temp->keranjangHead));
            temp = temp->next;
        }
    }
}

// --- BAGIAN 4: RIWAYAT TRANSAKSI (STACK) ---

void simpanRiwayat(char *nama, int total) {
    Riwayat *newRiwayat = (Riwayat*)malloc(sizeof(Riwayat));
    strcpy(newRiwayat->namaPelanggan, nama);
    newRiwayat->totalBelanja = total;
    newRiwayat->next = top; 
    top = newRiwayat;
}

void lihatRiwayat() {
    if (top == NULL) {
        printf("  [Belum ada transaksi selesai]\n");
    } else {
        Riwayat *temp = top;
        printf("  --- Tumpukan Struk (LIFO) ---\n");
        while (temp != NULL) {
            printf("  [STRUK] %s - Total: Rp %d\n", temp->namaPelanggan, temp->totalBelanja);
            temp = temp->next;
        }
    }
}

// --- BAGIAN 5: INTEGRASI PROSES KASIR ---

void prosesKasir() {
    if (front == NULL) {
        printf("\n[ERROR] Tidak ada antrian!\n");
        return;
    }

    Pelanggan *tempPelanggan = front;
    int totalBayar = hitungTotalKeranjang(tempPelanggan->keranjangHead);

    printf("\n--- MEMPROSES TRANSAKSI ---\n");
    printf("Pelanggan: %s\n", tempPelanggan->namaPelanggan);
    lihatKeranjang(tempPelanggan->keranjangHead);
    printf("STATUS: LUNAS.\n");

    simpanRiwayat(tempPelanggan->namaPelanggan, totalBayar);

    // Hapus memori keranjang
    Item *cartPtr = tempPelanggan->keranjangHead;
    while (cartPtr != NULL) {
        Item *sampah = cartPtr;
        cartPtr = cartPtr->next;
        free(sampah);
    }

    // Geser antrian
    front = front->next;
    if (front == NULL) rear = NULL;
    free(tempPelanggan); 
}

// --- MENUS ---

void menuGudang() {
    int pilihan;
    do {
        clearScreen();
        printf("=== MENU GUDANG & STOK ===\n");
        printf("1. Lihat Semua Stok (Display All)\n");
        printf("2. Cari Barang (Search Data)\n");
        printf("3. Kembali\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch(pilihan) {
            case 1: lihatSemuaStok(); pause(); break;
            case 2: cariBarang(); pause(); break;
        }
    } while (pilihan != 3);
}

void menuBelanja() {
    Item *keranjangSaya = NULL;
    int idBarang, qty;
    char namaUser[50];
    
    printf("\nMasukkan Nama Anda: ");
    scanf(" %[^\n]s", namaUser);

    int pilihan;
    do {
        clearScreen();
        printf("=== HALAMAN BELANJA: Halo, %s ===\n", namaUser);
        lihatSemuaStok(); // Menampilkan data dari Linked List Gudang
        
        printf("\nMenu:\n");
        printf("1. Pilih Barang (Input ID)\n");
        printf("2. Lihat Keranjang Saya\n");
        printf("3. Selesai & Bayar (Masuk Antrian)\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            printf("Masukkan ID Barang: "); scanf("%d", &idBarang);
            BarangGudang *brg = ambilBarangByID(idBarang);
            
            if (brg != NULL) {
                printf("Beli berapa %s? (Stok: %d): ", brg->nama, brg->stok);
                scanf("%d", &qty);
                if (qty > 0 && qty <= brg->stok) {
                    tambahKeKeranjang(&keranjangSaya, brg->nama, brg->harga, qty);
                    brg->stok -= qty; // Kurangi stok real-time
                } else {
                    printf("Stok tidak cukup / input salah!\n"); pause();
                }
            } else {
                printf("Barang tidak ditemukan!\n"); pause();
            }
        } else if (pilihan == 2) {
            lihatKeranjang(keranjangSaya); pause();
        } else if (pilihan == 3) {
            if (keranjangSaya != NULL) {
                masukAntrian(namaUser, keranjangSaya);
            } else {
                printf("Keranjang kosong!\n"); pause(); pilihan=0;
            }
        }
    } while (pilihan != 3);
}

int main() {
    initGudang(); // Masukkan data dummy otomatis
    
    int menuUtama;
    do {
        clearScreen();
        printf("========================================\n");
        printf("  SMART MARKET SYSTEM (INTEGRATED)\n");
        printf("========================================\n");
        printf("1. Admin Gudang (Cek Stok & Cari Barang)\n");
        printf("2. Pelanggan (Belanja)\n");
        printf("3. Kasir (Proses Antrian)\n");
        printf("4. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &menuUtama);

        switch(menuUtama) {
            case 1: menuGudang(); break;
            case 2: menuBelanja(); break;
            case 3: 
                // Menu Kasir Mini
                {
                    int sub;
                    do {
                        clearScreen();
                        printf("=== KASIR ===\n");
                        printf("1. Lihat Antrian\n2. Proses Bayar\n3. Lihat Riwayat\n4. Kembali\nPilih: ");
                        scanf("%d", &sub);
                        if(sub==1) { lihatAntrian(); pause(); }
                        else if(sub==2) { prosesKasir(); pause(); }
                        else if(sub==3) { lihatRiwayat(); pause(); }
                    } while(sub!=4);
                }
                break;
        }
    } while (menuUtama != 4);
    return 0;
}