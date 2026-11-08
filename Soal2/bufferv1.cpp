#define VERSI 1
#include "buffer.h"
#include <iostream>

void createBuffer(Buffer& b) {
    b.head = -1;
    b.tail = -1;
    b.nomorBerikut = 1;
    b.pergeseran = 0;
    for (int i = 0; i < KAPASITAS; i++) { b.data[i].nomor = 0; b.data[i].nilai = 0; }
}

int jumlahData(const Buffer& b) {
    return (b.head == -1) ? 0 : (b.tail - b.head + 1);
}

int ruangKosong(const Buffer& b) {
    return KAPASITAS - jumlahData(b);
}

bool enqueue(Buffer& b, int nilai) {
    if (b.tail == KAPASITAS - 1) return false;       // penuh
    if (b.head == -1) { b.head = 0; b.tail = 0; }
    else b.tail++;
    b.data[b.tail].nomor = b.nomorBerikut++;
    b.data[b.tail].nilai = nilai;
    return true;
}

bool dequeue(Buffer& b, Rekaman& hasil) {
    if (b.head == -1) return false;                  // kosong
    hasil = b.data[0];
    for (int i = 1; i <= b.tail; i++) {              // geser seluruh data setelahnya
        b.data[i - 1] = b.data[i];
        b.pergeseran++;
    }
    b.tail--;
    if (b.tail < 0) { b.head = -1; b.tail = -1; }
    return true;
}

Rekaman elemenKe(const Buffer& b, int i) {
    return b.data[b.head + i];
}

void kosongkan(Buffer& b) {
    b.head = -1;
    b.tail = -1;
}

void cetakFisik(const Buffer& b) {
    for (int i = 0; i < KAPASITAS; i++) {
        std::cout << "  [" << i << "] ";
        if (b.head != -1 && i >= b.head && i <= b.tail)
            std::cout << b.data[i].nomor << ":" << b.data[i].nilai;
        else
            std::cout << "-";
        std::cout << "\n";
    }
    std::cout << "  head=" << b.head << " tail=" << b.tail << "\n";
}

const char* namaVersi() { return "Versi 1 (array geser)"; }