#define VERSI 2
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

static void padatkan(Buffer& b) {
    int n = jumlahData(b);
    for (int i = 0; i < n; i++) {
        b.data[i] = b.data[b.head + i];              // satu pemindahan = satu hitungan
        b.pergeseran++;
    }
    b.head = 0;
    b.tail = n - 1;
}

bool enqueue(Buffer& b, int nilai) {
    if (b.head == -1) {
        b.head = 0;
        b.tail = 0;
    } else {
        if (b.tail == KAPASITAS - 1) {
            if (b.head == 0) return false;           // benar-benar penuh
            padatkan(b);                             // ada ruang di depan
        }
        b.tail++;
    }
    b.data[b.tail].nomor = b.nomorBerikut++;
    b.data[b.tail].nilai = nilai;
    return true;
}

bool dequeue(Buffer& b, Rekaman& hasil) {
    if (b.head == -1) return false;
    hasil = b.data[b.head];
    b.head++;
    if (b.head > b.tail) { b.head = -1; b.tail = -1; }
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

const char* namaVersi() { return "Versi 2 (array dengan pemadatan)"; }