#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produk {
    int id;
    char nama[50];
    int stok;
    int harga;
    struct Produk *next;
};

struct Penjualan {
    int idProduk;
    char namaProduk[50];
    int jumlah;
    char tanggal[20];
    struct Penjualan *next;
};

struct Produk *headProduk = NULL;
struct Penjualan *headJual = NULL;

// -------------------- FUNGSI PRODUK ---------------------

void tambahProduk() {
    struct Produk *baru = (struct Produk*) malloc(sizeof(struct Produk));

    printf("\nMasukkan ID produk: ");
    scanf("%d", &baru->id);
    printf("Masukkan nama produk: ");
    scanf(" %[^\n]", baru->nama);
    printf("Masukkan stok produk: ");
    scanf("%d", &baru->stok);
    printf("Masukkan harga produk: ");
    scanf("%d", &baru->harga);

    baru->next = NULL;

    if (headProduk == NULL)
        headProduk = baru;
    else {
        struct Produk *temp = headProduk;
        while (temp->next != NULL) temp = temp->next;
        temp->next = baru;
    }

    printf("\nProduk berhasil ditambahkan!\n");
}

void tampilProduk() {
    struct Produk *temp = headProduk;
    if (temp == NULL) {
        printf("\nTidak ada produk.\n");
        return;
    }

    printf("\n=== DAFTAR PRODUK ===\n");
    while (temp != NULL) {
        printf("ID: %d | Nama: %s | Stok: %d | Harga: %d\n",
            temp->id, temp->nama, temp->stok, temp->harga);
        temp = temp->next;
    }
}

struct Produk* cariProduk(int id) {
    struct Produk *temp = headProduk;
    while (temp != NULL) {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void menuCariProduk() {
    int id;
    printf("\nMasukkan ID produk yang dicari: ");
    scanf("%d", &id);

    struct Produk *p = cariProduk(id);

    if (p == NULL)
        printf("Produk tidak ditemukan.\n");
    else {
        printf("\nProduk ditemukan:\n");
        printf("ID: %d\nNama: %s\nStok: %d\nHarga: %d\n",
                p->id, p->nama, p->stok, p->harga);
    }
}

void hapusProduk() {
    int id;
    printf("\nMasukkan ID produk yang akan dihapus: ");
    scanf("%d", &id);

    struct Produk *temp = headProduk;
    struct Produk *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Produk tidak ditemukan.\n");
        return;
    }

    if (prev == NULL)
        headProduk = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("Produk berhasil dihapus.\n");
}

// -------------------- FUNGSI PENJUALAN ---------------------

void simpanPenjualan(int idP, char namaP[], int jumlah, char tanggal[]) {
    struct Penjualan *baru = (struct Penjualan*) malloc(sizeof(struct Penjualan));

    baru->idProduk = idP;
    strcpy(baru->namaProduk, namaP);
    baru->jumlah = jumlah;
    strcpy(baru->tanggal, tanggal);
    baru->next = NULL;

    if (headJual == NULL)
        headJual = baru;
    else {
        struct Penjualan *temp = headJual;
        while (temp->next != NULL) temp = temp->next;
        temp->next = baru;
    }
}

void tampilPenjualan() {
    struct Penjualan *temp = headJual;

    if (temp == NULL) {
        printf("\nBelum ada penjualan.\n");
        return;
    }

    printf("\n=== RIWAYAT PENJUALAN ===\n");
    while (temp != NULL) {
        printf("ID Produk: %d | Nama: %s | Jumlah: %d | Tanggal: %s\n",
                temp->idProduk, temp->namaProduk, temp->jumlah, temp->tanggal);
        temp = temp->next;
    }
}

// -------------------- MENU JUAL PRODUK ---------------------

void jualProduk() {
    int id, jumlah;
    char tanggal[20];

    printf("\nMasukkan ID produk yang ingin dijual: ");
    scanf("%d", &id);

    struct Produk *p = cariProduk(id);

    if (p == NULL) {
        printf("Produk tidak ditemukan.\n");
        return;
    }

    printf("Masukkan jumlah yang ingin dijual: ");
    scanf("%d", &jumlah);

    if (jumlah > p->stok) {
        printf("Stok tidak mencukupi! Stok sekarang: %d\n", p->stok);
        return;
    }

    printf("Masukkan tanggal penjualan (DD-MM-YYYY): ");
    scanf(" %[^\n]", tanggal);

    p->stok -= jumlah;

    simpanPenjualan(p->id, p->nama, jumlah, tanggal);

    printf("\nPenjualan berhasil!\nSisa stok: %d\n", p->stok);
}

// -------------------- MAIN PROGRAM ---------------------

int main() {
    int pilih;

    do {
        printf("\n=== MENU TOKO ===\n");
        printf("1. Tambah Produk\n");
        printf("2. Tampilkan Semua Produk\n");
        printf("3. Cari Produk\n");
        printf("4. Hapus Produk\n");
        printf("5. Jual Produk\n");
        printf("6. Tampilkan Riwayat Penjualan\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: tambahProduk(); break;
            case 2: tampilProduk(); break;
            case 3: menuCariProduk(); break;
            case 4: hapusProduk(); break;
            case 5: jualProduk(); break;
            case 6: tampilPenjualan(); break;
            case 0: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid!\n");
        }
    } while (pilih != 0);

    return 0;
}
