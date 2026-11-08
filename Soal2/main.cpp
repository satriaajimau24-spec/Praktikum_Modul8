#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include "buffer.h"

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

static void cetakIsi(const Buffer& b) {
    int n = jumlahData(b);
    cout << "Isi FIFO: ";
    if (n == 0) { cout << "(kosong)\n"; return; }
    for (int i = 0; i < n; i++) {
        Rekaman r = elemenKe(b, i);
        cout << r.nomor << ":" << r.nilai << (i + 1 < n ? ", " : "\n");
    }
}

static void cmdTulis(Buffer& b, const string tok[], int n) {
    long k;
    if (n < 2 || !parseInt(tok[1], k)) { cout << "DITOLAK: format TULIS k v1 ... vk\n"; return; }
    if (k < 1 || k > KAPASITAS) { cout << "DITOLAK: k harus 1-6\n"; return; }
    if (n != k + 2) { cout << "DITOLAK: jumlah nilai tidak sama dengan k\n"; return; }
    int nilai[KAPASITAS];
    for (int i = 0; i < k; i++) {
        long v;
        if (!parseInt(tok[2 + i], v)) { cout << "DITOLAK: nilai '" << tok[2 + i] << "' bukan bilangan bulat\n"; return; }
        if (v < -1000 || v > 1000) { cout << "DITOLAK: nilai " << v << " di luar rentang -1000..1000\n"; return; }
        nilai[i] = (int)v;
    }
    if (ruangKosong(b) < k) {
        cout << "DITOLAK: ruang kosong hanya " << ruangKosong(b) << "\n"; return;
    }
    // seluruh batch valid -> tambahkan satu per satu
    int nomorAwal = b.nomorBerikut;
    for (int i = 0; i < k; i++) enqueue(b, nilai[i]);
    cout << "OK: " << k << " rekaman masuk (nomor " << nomorAwal << "-" << (b.nomorBerikut - 1) << ")\n";
    cetakIsi(b);
}

static void cmdAmbil(Buffer& b, const string tok[], int n) {
    long k;
    if (n != 2 || !parseInt(tok[1], k)) { cout << "DITOLAK: format AMBIL k\n"; return; }
    if (k < 1 || k > KAPASITAS) { cout << "DITOLAK: k harus 1-6\n"; return; }
    if (k > jumlahData(b)) { cout << "DITOLAK: data hanya " << jumlahData(b) << "\n"; return; }
    for (int i = 0; i < k; i++) {
        Rekaman r;
        dequeue(b, r);
        cout << "Keluar " << r.nomor << ":" << r.nilai << "\n";
    }
    cetakIsi(b);
}

static void cmdLihat(const Buffer& b) {
    cetakIsi(b);
    cout << "Keadaan fisik:\n";
    cetakFisik(b);
    cout << "  nomor berikutnya=" << b.nomorBerikut << ", total pergeseran=" << b.pergeseran << "\n";
}

static void cmdRingkas(const Buffer& b) {
    int n = jumlahData(b);
    if (n == 0) {
        cout << "Jumlah 0; total 0; minimum TIDAK ADA; maksimum TIDAK ADA; rata-rata TIDAK ADA\n";
        return;
    }
    long total = 0;
    int mn = elemenKe(b, 0).nilai, mx = mn;
    for (int i = 0; i < n; i++) {          // hanya membaca, queue tidak berubah
        int v = elemenKe(b, i).nilai;
        total += v;
        if (v < mn) mn = v;
        if (v > mx) mx = v;
    }
    printf("Jumlah %d; total %ld; minimum %d; maksimum %d; rata-rata %.2f\n",
           n, total, mn, mx, (double)total / n);
}

int main(int argc, char** argv) {
    bool echo = (argc > 1 && string(argv[1]) == "--echo");
    Buffer b;
    createBuffer(b);
    cout << "== " << namaVersi() << " ==\n";

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
        if (cmd == "TULIS") cmdTulis(b, tok, n);
        else if (cmd == "AMBIL") cmdAmbil(b, tok, n);
        else if (cmd == "LIHAT") cmdLihat(b);
        else if (cmd == "RINGKAS") cmdRingkas(b);
        else if (cmd == "KOSONGKAN") { kosongkan(b); cout << "OK: buffer dikosongkan (nomor & pergeseran tidak direset)\n"; }
        else if (cmd == "SELESAI") break;
        else cout << "DITOLAK: perintah tidak dikenal\n";
    }
    cout << "Selesai. Total pergeseran = " << b.pergeseran << " rekaman\n";
    return 0;
}