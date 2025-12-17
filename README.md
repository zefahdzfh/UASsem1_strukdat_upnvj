# 🛒 Sistem Manajemen Toko (Point of Sales) CLI

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Kampus](https://img.shields.io/badge/UPN-Veteran%20Jakarta-green.svg)
![Prodi](https://img.shields.io/badge/Prodi-D3%20Sistem%20Informasi-orange.svg)

Proyek ini adalah aplikasi berbasis *Command Line Interface* (CLI) yang dibuat menggunakan bahasa pemrograman C. Aplikasi ini berfungsi untuk memanajemen stok barang dan mencatat transaksi penjualan menggunakan struktur data **Linked List** dan penyimpanan file (File Handling).

> **Proyek Ujian Akhir Semester (UAS)**
> * **Mata Kuliah:** Struktur Data
> * **Semester:** 1
> * **Program Studi:** D3 Sistem Informasi
> * **Kampus:** UPN Veteran Jakarta

---

## 👨‍💻 Tim Pengembang

| Nama Mahasiswa | NIM | Peran / Tugas |
| :--- | :--- | :--- |
| **Hudzaifah Ibnu Puase** | **2510501107** | Lead Developer |
| **Fathussabil** | **2510501120** | Lead Developer |
| Nabiel PMMB | 2510501118 | Developer |
| Muhammad Alfrido Pasha | 2510501129 | Developer |

---

## 📚 Fitur Utama

Aplikasi ini memiliki fitur CRUD (Create, Read, Update, Delete) lengkap serta fitur transaksi dan penyimpanan data permanen.

### 📦 Manajemen Produk
1.  **Tambah Produk:** Menambahkan data barang baru (ID, Nama, Stok, Harga).
2.  **Tampilkan Produk:** Melihat seluruh daftar barang yang tersedia.
3.  **Cari Produk:** Mencari barang spesifik berdasarkan ID.
4.  **Ubah Produk:** Mengedit data barang (Nama, Stok, Harga).
5.  **Hapus Produk:** Menghapus barang dari daftar.
6.  **Urutkan Produk:** Mengurutkan daftar barang berdasarkan ID (Ascending) menggunakan algoritma *Bubble Sort*.

### 💰 Transaksi
1.  **Jual Produk:** Melakukan transaksi penjualan yang otomatis mengurangi stok barang.
2.  **Riwayat Penjualan:** Menampilkan log/histori transaksi yang pernah terjadi.

### 💾 Penyimpanan Data (Persistence)
* Data Produk disimpan otomatis ke file `produk.txt`.
* Data Penjualan disimpan otomatis ke file `penjualan.txt`.
* Data akan dimuat kembali (*load*) saat program dijalankan ulang.

---

## 🛠️ Implementasi Struktur Data

Proyek ini menerapkan konsep-konsep kunci dalam mata kuliah Struktur Data:

1.  **Singly Linked List:**
    Digunakan untuk menyimpan data produk dan data penjualan secara dinamis di memori.
    * `struct Produk`: Menyimpan data barang dan pointer `next`.
    * `struct Penjualan`: Menyimpan data transaksi dan pointer `next`.

2.  **File Handling (I/O):**
    Menggunakan library standard `<stdio.h>` untuk membaca dan menulis file database sederhana (`.txt`) agar data tidak hilang saat program ditutup.

3.  **Sorting & Searching:**
    * **Sorting:** Menggunakan metode *Bubble Sort* (tukar data antar node) untuk mengurutkan produk berdasarkan ID.
    * **Searching:** Menggunakan *Linear Search* (traversal linked list) untuk menemukan ID produk.

---

## 🚀 Cara Menjalankan Program

### Prasyarat
Pastikan komputer kamu sudah terinstall compiler C seperti GCC (MinGW untuk Windows atau build-essential untuk Linux/Mac).

### Langkah-langkah
1.  **Clone repository ini:**
    ```bash
    git clone [https://github.com/username-kamu/nama-repo.git](https://github.com/zefahdzfh/UASsem1_strukdat_upnvj.git)
    ```
2.  **Masuk ke direktori project:**
    ```bash
    cd nama-repo
    ```
3.  **Compile program:**
    ```bash
    gcc main.c -o toko
    ```
4.  **Jalankan program:**
    * **Windows:**
        ```bash
        toko.exe
        ```
    * **Linux/Mac:**
        ```bash
        ./toko
        ```

---

## 📂 Struktur File

```text
.
├── main.c           # Source code utama aplikasi
├── produk.txt       # Database produk (Dibuat otomatis)
├── penjualan.txt    # Database riwayat penjualan (Dibuat otomatis)
└── README.md        # Dokumentasi proyek