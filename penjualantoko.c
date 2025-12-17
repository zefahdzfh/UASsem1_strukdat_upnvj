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

struct Produk* cariProduk(int id) {
    struct Produk *t = headProduk;
    while (t != NULL) {
        if (t->id == id) return t;
        t = t->next;
    }
    return NULL;
}

void simpanProdukKeFile() {
    FILE *f = fopen("produk.txt", "w");
    struct Produk *p = headProduk;
    if (!f) return;
    while (p != NULL) {
        fprintf(f, "%d;%s;%d;%d\n", p->id, p->nama, p->stok, p->harga);
        p = p->next;
    }
    fclose(f);
}

void simpanPenjualanKeFile() {
    FILE *f = fopen("penjualan.txt", "w");
    struct Penjualan *p = headJual;
    if (!f) return;
    while (p != NULL) {
        fprintf(f, "%d;%s;%d;%s\n",
                p->idProduk, p->namaProduk, p->jumlah, p->tanggal);
        p = p->next;
    }
    fclose(f);
}

void muatProdukDariFile() {
    FILE *f = fopen("produk.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        struct Produk *baru = malloc(sizeof(struct Produk));
        if (fscanf(f, "%d;%[^;];%d;%d\n",
                   &baru->id, baru->nama, &baru->stok, &baru->harga) == 4) {
            baru->next = NULL;
            if (headProduk == NULL) headProduk = baru;
            else {
                struct Produk *t = headProduk;
                while (t->next != NULL) t = t->next;
                t->next = baru;
            }
        } else free(baru);
    }
    fclose(f);
}

void muatPenjualanDariFile() {
    FILE *f = fopen("penjualan.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        struct Penjualan *baru = malloc(sizeof(struct Penjualan));
        if (fscanf(f, "%d;%[^;];%d;%[^\n]\n",
                   &baru->idProduk,
                   baru->namaProduk,
                   &baru->jumlah,
                   baru->tanggal) == 4) {
            baru->next = NULL;
            if (headJual == NULL) headJual = baru;
            else {
                struct Penjualan *t = headJual;
                while (t->next != NULL) t = t->next;
                t->next = baru;
            }
        } else free(baru);
    }
    fclose(f);
}

void tambahProduk() {
    int id;
    printf("Masukkan ID produk: ");
    scanf("%d", &id);

    if (cariProduk(id) != NULL) {
        printf("ID sudah terdaftar. Tambah produk dibatalkan\n");
        return;
    }

    struct Produk *baru = malloc(sizeof(struct Produk));
    baru->id = id;
    printf("Masukkan nama produk: ");
    scanf(" %[^\n]", baru->nama);
    printf("Masukkan stok produk: ");
    scanf("%d", &baru->stok);
    printf("Masukkan harga produk: ");
    scanf("%d", &baru->harga);

    baru->next = NULL;

    if (headProduk == NULL) headProduk = baru;
    else {
        struct Produk *t = headProduk;
        while (t->next != NULL) t = t->next;
        t->next = baru;
    }
    printf("Produk berhasil ditambahkan\n");
}

void tampilProduk() {
    struct Produk *t = headProduk;
    if (t == NULL) {
        printf("Tidak ada produk\n");
        return;
    }
    while (t != NULL) {
        printf("ID: %d | Nama: %s | Stok: %d | Harga: %d\n",
               t->id, t->nama, t->stok, t->harga);
        t = t->next;
    }
}

void menuCariProduk() {
    int id;
    printf("Masukkan ID produk: ");
    scanf("%d", &id);

    struct Produk *p = cariProduk(id);
    if (p == NULL) printf("Produk tidak ditemukan\n");
    else {
        printf("ID: %d\nNama: %s\nStok: %d\nHarga: %d\n",
               p->id, p->nama, p->stok, p->harga);
    }
}

void hapusProduk() {
    int id;
    printf("Masukkan ID produk: ");
    scanf("%d", &id);

    struct Produk *t = headProduk;
    struct Produk *prev = NULL;

    while (t != NULL && t->id != id) {
        prev = t;
        t = t->next;
    }

    if (t == NULL) {
        printf("Produk tidak ditemukan\n");
        return;
    }

    if (prev == NULL) headProduk = t->next;
    else prev->next = t->next;

    free(t);
    printf("Produk berhasil dihapus\n");
}

void ubahProduk() {
    int id;
    printf("Masukkan ID produk yang ingin diubah: ");
    scanf("%d", &id);

    struct Produk *p = cariProduk(id);

    if (p == NULL) {
        printf("Produk tidak ditemukan\n");
        return;
    }

    printf("Nama baru: ");
    scanf(" %[^\n]", p->nama);
    printf("Stok baru: ");
    scanf("%d", &p->stok);
    printf("Harga baru: ");
    scanf("%d", &p->harga);

    printf("Produk berhasil diubah\n");
}

void sortProduk() {
    if (headProduk == NULL) return;

    int swapped;
    struct Produk *ptr;
    struct Produk *last = NULL;

    do {
        swapped = 0;
        ptr = headProduk;

        while (ptr->next != last) {
            if (ptr->id > ptr->next->id) {
                int tid = ptr->id;
                char tnama[50];
                int tstok = ptr->stok;
                int tharga = ptr->harga;

                strcpy(tnama, ptr->nama);

                ptr->id = ptr->next->id;
                strcpy(ptr->nama, ptr->next->nama);
                ptr->stok = ptr->next->stok;
                ptr->harga = ptr->next->harga;

                ptr->next->id = tid;
                strcpy(ptr->next->nama, tnama);
                ptr->next->stok = tstok;
                ptr->next->harga = tharga;

                swapped = 1;
            }
            ptr = ptr->next;
        }
        last = ptr;
    } while (swapped);

    printf("Data produk berhasil diurutkan\n");
}

void simpanPenjualan(int idP, char namaP[], int jumlah, char tanggal[]) {
    struct Penjualan *baru = malloc(sizeof(struct Penjualan));
    baru->idProduk = idP;
    strcpy(baru->namaProduk, namaP);
    baru->jumlah = jumlah;
    strcpy(baru->tanggal, tanggal);
    baru->next = NULL;

    if (headJual == NULL) headJual = baru;
    else {
        struct Penjualan *t = headJual;
        while (t->next != NULL) t = t->next;
        t->next = baru;
    }
}

void jualProduk() {
    int id, jumlah;
    char tanggal[20];

    printf("Masukkan ID produk: ");
    scanf("%d", &id);

    struct Produk *p = cariProduk(id);
    if (p == NULL) {
        printf("Produk tidak ditemukan\n");
        return;
    }

    printf("Jumlah jual: ");
    scanf("%d", &jumlah);

    if (jumlah > p->stok) {
        printf("Stok tidak cukup\n");
        return;
    }

    printf("Tanggal (DD-MM-YYYY): ");
    scanf(" %[^\n]", tanggal);

    p->stok -= jumlah;

    simpanPenjualan(p->id, p->nama, jumlah, tanggal);

    printf("Penjualan berhasil\n");
}

void tampilPenjualan() {
    struct Penjualan *t = headJual;
    if (t == NULL) {
        printf("Belum ada penjualan\n");
        return;
    }
    while (t != NULL) {
        printf("ID: %d | Nama: %s | Jumlah: %d | Tanggal: %s\n",
               t->idProduk, t->namaProduk, t->jumlah, t->tanggal);
        t = t->next;
    }
}

int main() {
    int pilih;

    muatProdukDariFile();
    muatPenjualanDariFile();

    do {
        printf("\nMENU TOKO\n");
        printf("1. Tambah Produk\n");
        printf("2. Tampilkan Produk\n");
        printf("3. Cari Produk\n");
        printf("4. Hapus Produk\n");
        printf("5. Jual Produk\n");
        printf("6. Tampilkan Penjualan\n");
        printf("7. Ubah Produk\n");
        printf("8. Urutkan Produk\n");
        printf("0. Simpan dan Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: tambahProduk(); break;
            case 2: tampilProduk(); break;
            case 3: menuCariProduk(); break;
            case 4: hapusProduk(); break;
            case 5: jualProduk(); break;
            case 6: tampilPenjualan(); break;
            case 7: ubahProduk(); break;
            case 8: sortProduk(); break;
            case 0:
                simpanProdukKeFile();
                simpanPenjualanKeFile();
                printf("Data disimpan. Keluar...\n");
                break;
            default:
                printf("Pilihan tidak valid\n");
        }

    } while (pilih != 0);

    return 0;
}
