#ifndef BUFFER_H
#define BUFFER_H

#ifndef VERSI
#error "Tentukan versi saat kompilasi, mis. -DVERSI=1"
#endif

const int KAPASITAS = 6;

struct Rekaman {
    int nomor;   // nomor urut otomatis, mulai dari 1
    int nilai;   // -1000..1000
};

#if VERSI == 3
struct Buffer {                       // circular: head + count, TANPA tail
    Rekaman data[KAPASITAS];
    int head;
    int count;
    int nomorBerikut;
    long pergeseran;
};
#else
struct Buffer {                       // versi 1 dan 2: head + tail
    Rekaman data[KAPASITAS];
    int head;
    int tail;
    int nomorBerikut;
    long pergeseran;
};
#endif

void createBuffer(Buffer& b);
int jumlahData(const Buffer& b);
int ruangKosong(const Buffer& b);
bool enqueue(Buffer& b, int nilai);            // false jika penuh
bool dequeue(Buffer& b, Rekaman& hasil);       // false jika kosong
Rekaman elemenKe(const Buffer& b, int i);      // elemen FIFO ke-i (0 = depan)
void kosongkan(Buffer& b);                     // nomorBerikut & pergeseran tidak direset
void cetakFisik(const Buffer& b);              // isi tiap indeks fisik + penanda
const char* namaVersi();

#endif