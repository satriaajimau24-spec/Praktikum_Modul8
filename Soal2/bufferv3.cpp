#define VERSI 3
#include "buffer.h"
#include <iostream>

void createBuffer(Buffer& b) {
    b.head = 0;
    b.count = 0;
    b.nomorBerikut = 1;
    b.pergeseran = 0;
    for (int i = 0; i < KAPASITAS; i++) { b.data[i].nomor = 0; b.data[i].nilai = 0; }
}

int jumlahData(const Buffer& b) { return b.count; }

int ruangKosong(const Buffer& b) { return KAPASITAS - b.count; }

bool enqueue(Buffer& b, int nilai) {
    if (b.count == KAPASITAS) return false;
    int posTulis = (b.head + b.count) % KAPASITAS;   // posisi tulis dihitung
    b.data[posTulis].nomor = b.nomorBerikut++;
    b.data[posTulis].nilai = nilai;
    b.count++;
    return true;
}

bool dequeue(Buffer& b, Rekaman& hasil) {
    if (b.count == 0) return false;
    hasil = b.data[b.head];
    b.head = (b.head + 1) % KAPASITAS;
    b.count--;
    if (b.count == 0) b.head = 0;                    // keadaan kosong: head = count = 0
    return true;
}

Rekaman elemenKe(const Buffer& b, int i) {
    return b.data[(b.head + i) % KAPASITAS];
}

void kosongkan(Buffer& b) {
    b.head = 0;
    b.count = 0;
}

void cetakFisik(const Buffer& b) {
    for (int i = 0; i < KAPASITAS; i++) {
        std::cout << "  [" << i << "] ";
        int offset = (i - b.head + KAPASITAS) % KAPASITAS;
        if (offset < b.count)
            std::cout << b.data[i].nomor << ":" << b.data[i].nilai;
        else
            std::cout << "-";
        std::cout << "\n";
    }
    std::cout << "  head=" << b.head << " count=" << b.count << "\n";
}

const char* namaVersi() { return "Versi 3 (circular queue)"; }