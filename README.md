# Aplikasi Manajemen Toko  
**UAS Struktur Data — UPN Veteran Jakarta**

Project ini merupakan aplikasi manajemen toko sederhana berbasis konsol yang dikembangkan menggunakan bahasa C sebagai pemenuhan tugas **Ujian Akhir Semester (UAS)** mata kuliah **Struktur Data** di Universitas Pembangunan Nasional Veteran Jakarta (UPNVJ).

Aplikasi ini memanfaatkan struktur data **single linked list** untuk menyimpan dan mengelola data produk serta riwayat penjualan secara dinamis, sehingga tidak dibatasi oleh ukuran array statis.

---

## 🚀 Fitur Aplikasi

### 📦 Manajemen Produk
- Menambahkan data produk (ID, nama, stok, harga)
- Menampilkan seluruh daftar produk
- Mencari produk berdasarkan ID
- Menghapus produk dari sistem

### 🧾 Manajemen Penjualan
- Melakukan transaksi penjualan produk
- Validasi ketersediaan stok
- Pengurangan stok secara otomatis
- Penyimpanan riwayat penjualan
- Menampilkan riwayat transaksi penjualan

### 📋 Menu Interaktif
Program menyediakan menu utama berbasis perulangan `do-while` dengan navigasi sederhana menggunakan `switch-case` agar mudah digunakan oleh pengguna.

---

## 🧠 Konsep & Teknologi
- **Bahasa Pemrograman:** C  
- **Struktur Data:** Single Linked List  
- **Konsep:** Pointer, dynamic memory allocation, traversal linked list, CRUD data  

---

## 📂 Struktur Project
UASsem1_strukdat_upnvj
├── .vscode
├── penjualantoko.c
├── penjualantoko.exe
├── .gitignore
└── README.md


---

## ⚙️ Cara Kompilasi & Menjalankan Program

1. **Kompilasi Program**
   ```bash
   gcc penjualantoko.c -o penjualantoko
./penjualantoko

📄 Deskripsi Singkat Program

Aplikasi ini berfungsi sebagai simulasi kasir toko kecil yang mampu mengelola data produk dan transaksi penjualan secara efisien. Seluruh data disimpan sementara di dalam memori menggunakan linked list selama program dijalankan.

📝 Catatan

Project ini dibuat khusus untuk keperluan akademik dan pembelajaran pada mata kuliah Struktur Data.

