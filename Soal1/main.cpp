#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include "paket.h"

using namespace std;

static bool parseInt(const string& s, long& out) {
    if (s.empty()) return false;
    char* end = nullptr;
    errno = 0;
    long v = strtol(s.c_str(), &end, 10);
    if (errno != 0 || *end != '\0') return false;
    out = v;
    return true;
}

static string upper(string s) {
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)toupper((unsigned char)s[i]);
    return s;
}

static void cmdMasuk(Queue& A, const Queue& B, const string tok[], int n) {
    if (n != 3) { cout << "DITOLAK: format MASUK kode berat\n"; return; }
    const string& kode = tok[1];
    long berat;
    if (kode.size() < 1 || kode.size() > 10) {
        cout << "DITOLAK: kode harus 1-10 karakter\n"; return;
    }
    if (!parseInt(tok[2], berat) || berat < 1 || berat > 5000) {
        cout << "DITOLAK: berat harus bilangan bulat 1-5000 gram\n"; return;
    }
    if (findElm(A, kode) != nullptr) { cout << "DITOLAK: kode " << kode << " sudah aktif di A\n"; return; }
    if (findElm(B, kode) != nullptr) { cout << "DITOLAK: kode " << kode << " sudah aktif di B\n"; return; }
    Node* p = alokasi(kode, (int)berat);
    if (p == nullptr) { cout << "DITOLAK: alokasi memori gagal\n"; return; }
    enqueue(A, p);
    cout << "OK: " << kode << " (" << berat << " gram) masuk di belakang A\n";
}

static void cmdPindah(Queue& A, Queue& B, const string tok[], int n) {
    long k;
    if (n != 2 || !parseInt(tok[1], k)) { cout << "DITOLAK: format PINDAH n\n"; return; }
    if (k <= 0) { cout << "DITOLAK: n harus positif\n"; return; }
    int isiA = nbList(A);
    if (k > isiA) {
        cout << "DITOLAK: A hanya berisi " << isiA << " paket\n"; return;
    }
    for (long i = 0; i < k; i++) {
        Node* p = A.head;                         
        bool a0 = fFindElm(A, p), b0 = fFindElm(B, p);
        string kode = p->info.kode;
        Node* q = dequeue(A);                     
        enqueue(B, q);                            
        bool a1 = fFindElm(A, q), b1 = fFindElm(B, q);
        printf("PINDAH %s alamat %p | sebelum: A=%d B=%d | sesudah: A=%d B=%d (alamat %s)\n",
               kode.c_str(), (void*)q, a0, b0, a1, b1, (p == q ? "tetap" : "BERUBAH"));
    }
    cout << "OK: " << k << " paket dipindahkan dari A ke B\n";
}

static void cmdKirim(Queue& B, Statistik& st, const string tok[], int n) {
    long k;
    if (n != 2 || !parseInt(tok[1], k)) { cout << "DITOLAK: format KIRIM n\n"; return; }
    if (k <= 0) { cout << "DITOLAK: n harus positif\n"; return; }
    int isiB = nbList(B);
    if (k > isiB) {
        cout << "DITOLAK: B hanya berisi " << isiB << " paket\n"; return;
    }
    long subtotal = 0;
    for (long i = 0; i < k; i++) {
        Node* p = dequeue(B);
        cout << "KIRIM: " << p->info.kode << " " << p->info.berat << " gram\n";
        st.terkirim++;
        st.totalBerat += p->info.berat;
        subtotal += p->info.berat;
        dealokasi(p);                             
    }
    cout << "OK: " << k << " paket terkirim, total " << subtotal << " gram\n";
}

static void cmdCari(const Queue& A, const Queue& B, const string tok[], int n) {
    if (n != 2) { cout << "DITOLAK: format CARI kode\n"; return; }
    const string& kode = tok[1];
    Node* p = findElm(A, kode);
    char lok = 'A';
    int pos;
    if (p != nullptr) pos = posisiElm(A, kode);
    else {
        p = findElm(B, kode);
        lok = 'B';
        pos = (p != nullptr) ? posisiElm(B, kode) : 0;
    }
    if (p == nullptr) { cout << "TIDAK DITEMUKAN\n"; return; }
    cout << kode << ": " << lok << ", posisi " << pos << ", berat " << p->info.berat << " gram\n";
}

static void cmdLihat(const Queue& A, const Queue& B) {
    cout << "A (depan->belakang): "; printInfo(A); cout << "\n";
    cout << "B (depan->belakang): "; printInfo(B); cout << "\n";
    cout << "A head=" << (isEmptyQueue(A) ? "null" : A.head->info.kode)
         << " tail=" << (isEmptyQueue(A) ? "null" : A.tail->info.kode) << " | "
         << "B head=" << (isEmptyQueue(B) ? "null" : B.head->info.kode)
         << " tail=" << (isEmptyQueue(B) ? "null" : B.tail->info.kode) << "\n";
}

static void cmdRekap(const Queue& A, const Queue& B, const Statistik& st) {
    cout << "A       : " << nbList(A) << " paket, " << totalBerat(A) << " gram\n";
    cout << "B       : " << nbList(B) << " paket, " << totalBerat(B) << " gram\n";
    cout << "Terkirim: " << st.terkirim << " paket, " << st.totalBerat << " gram\n";
}

int main(int argc, char** argv) {
    bool echo = (argc > 1 && string(argv[1]) == "--echo");
    Queue A, B;
    Statistik st = {0, 0};
    createQueue(A);
    createQueue(B);

    string line;
    while (getline(cin, line)) {
        string tok[16];
        int n = 0;
        istringstream iss(line);
        string t;
        while (iss >> t) { if (n < 16) tok[n] = t; n++; }
        if (n == 0) continue;
        if (echo) cout << "> " << line << "\n";
        if (n > 16) { cout << "DITOLAK: terlalu banyak argumen\n"; continue; }

        string cmd = upper(tok[0]);
        if (cmd == "MASUK") cmdMasuk(A, B, tok, n);
        else if (cmd == "PINDAH") cmdPindah(A, B, tok, n);
        else if (cmd == "KIRIM") cmdKirim(B, st, tok, n);
        else if (cmd == "CARI") cmdCari(A, B, tok, n);
        else if (cmd == "LIHAT") cmdLihat(A, B);
        else if (cmd == "REKAP") cmdRekap(A, B, st);
        else if (cmd == "BERSIHKAN") {
            delAll(A); delAll(B);
            cout << "OK: seluruh paket di A dan B dihapus (statistik terkirim tetap)\n";
        }
        else if (cmd == "SELESAI") break;
        else cout << "DITOLAK: perintah tidak dikenal\n";
    }
    delAll(A);
    delAll(B);
    cout << "Program selesai. Sisa node dibebaskan.\n";
    return 0;
}