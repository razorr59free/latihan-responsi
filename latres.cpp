#include <iostream>
#include <string>
using namespace std;

struct Pelaku {
    string nama;
    string posisi;
    int nominal;
    Pelaku *kiri, *kanan;
};

struct Stack {
    string aksi; // "hapus" atau "tambah"
    Pelaku *data;
    Stack *next;
};

Pelaku *root = NULL;
Stack *logUndo = NULL;

// Fungsi buat node baru
Pelaku* buatNode(string nama, string posisi, int nominal) {
    Pelaku* baru = new Pelaku;
    baru->nama = nama;
    baru->posisi = posisi;
    baru->nominal = nominal;
    baru->kiri = baru->kanan = NULL;
    return baru;
}

// Insert ke BST
Pelaku* tambahData(Pelaku* akar, Pelaku* baru) {
    if (akar == NULL) return baru;
    if (baru->nama < akar->nama)
        akar->kiri = tambahData(akar->kiri, baru);
    else
        akar->kanan = tambahData(akar->kanan, baru);
    return akar;
}

// Simpan log undo
void simpanUndo(string aksi, Pelaku* data) {
    Stack* baru = new Stack;
    baru->aksi = aksi;
    baru->data = buatNode(data->nama, data->posisi, data->nominal);
    baru->next = logUndo;
    logUndo = baru;
}

// Inorder (urut A-Z)
void tampilInorder(Pelaku* akar) {
    if (akar != NULL) {
        tampilInorder(akar->kiri);
        cout << akar->nama << " - " << akar->posisi << " - Rp" << akar->nominal << endl;
        tampilInorder(akar->kanan);
    }
}

// Hapus node berdasarkan nama
Pelaku* hapusData(Pelaku* akar, string nama, bool& ketemu) {
    if (akar == NULL) return NULL;
    if (nama < akar->nama) {
        akar->kiri = hapusData(akar->kiri, nama, ketemu);
    } else if (nama > akar->nama) {
        akar->kanan = hapusData(akar->kanan, nama, ketemu);
    } else {
        ketemu = true;
        simpanUndo("hapus", akar);
        if (akar->kiri == NULL) {
            Pelaku* temp = akar->kanan;
            delete akar;
            return temp;
        } else if (akar->kanan == NULL) {
            Pelaku* temp = akar->kiri;
            delete akar;
            return temp;
        } else {
            Pelaku* pengganti = akar->kanan;
            while (pengganti->kiri != NULL) pengganti = pengganti->kiri;
            akar->nama = pengganti->nama;
            akar->posisi = pengganti->posisi;
            akar->nominal = pengganti->nominal;
            akar->kanan = hapusData(akar->kanan, pengganti->nama, ketemu);
        }
    }
    return akar;
}

// Undo aksi terakhir
void undo() {
    if (logUndo == NULL) {
        cout << "Tidak ada aksi yang bisa di-undo.\n";
        return;
    }

    Stack* temp = logUndo;
    logUndo = logUndo->next;

    if (temp->aksi == "tambah") {
        bool dummy;
        root = hapusData(root, temp->data->nama, dummy);
        cout << "Undo: Data " << temp->data->nama << " dihapus.\n";
    } else if (temp->aksi == "hapus") {
        root = tambahData(root, temp->data);
        cout << "Undo: Data " << temp->data->nama << " dikembalikan.\n";
    }

    delete temp;
}

void tambahPelaku() {
    string nama, posisi;
    int nominal;
    cout << "Nama: "; getline(cin, nama);
    cout << "Posisi: "; getline(cin, posisi);
    cout << "Nominal: "; cin >> nominal; cin.ignore();
    Pelaku* baru = buatNode(nama, posisi, nominal);
    root = tambahData(root, baru);
    simpanUndo("tambah", baru);
    cout << "Data berhasil ditambahkan.\n";
}

void hapusPelaku() {
    string nama;
    cout << "Nama pelaku yang ingin dihapus: ";
    getline(cin, nama);
    bool ketemu = false;
    root = hapusData(root, nama, ketemu);
    if (ketemu)
        cout << "Data berhasil dihapus.\n";
    else
        cout << "Data tidak ditemukan.\n";
}

int main() {
    int pilihan;
    do {
        cout << "\n=== MENU ===\n";
        cout << "1. Tambah Data Pelaku\n";
        cout << "2. Tampilkan Daftar Pelaku (A-Z)\n";
        cout << "3. Hapus Data Pelaku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan; cin.ignore();

        switch (pilihan) {
            case 1: tambahPelaku(); break;
            case 2: tampilInorder(root); break;
            case 3: hapusPelaku(); break;
            case 4: undo(); break;
            case 0: cout << "Keluar.\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);

    return 0;
}