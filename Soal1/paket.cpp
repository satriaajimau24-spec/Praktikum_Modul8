#include "paket.h"
#include <iostream>
#include <new>

void createQueue(Queue& q) {
    q.head = nullptr;
    q.tail = nullptr;
}

bool isEmptyQueue(const Queue& q) {
    return q.head == nullptr;
}

Node* alokasi(const std::string& kode, int berat) {
    Node* p = new (std::nothrow) Node;
    if (p == nullptr) return nullptr;
    try {
        p->info.kode = kode;
    } catch (...) {
        delete p;
        return nullptr;
    }
    p->info.berat = berat;
    p->next = nullptr;
    return p;
}

void dealokasi(Node*& p) {
    delete p;
    p = nullptr;      
}

void enqueue(Queue& q, Node* p) {
    if (p == nullptr) return;
    p->next = nullptr;                 
    if (isEmptyQueue(q)) {
        q.head = p;
        q.tail = p;
    } else {
        q.tail->next = p;
        q.tail = p;
    }
}

Node* dequeue(Queue& q) {
    if (isEmptyQueue(q)) return nullptr;
    Node* p = q.head;
    q.head = p->next;
    if (q.head == nullptr) q.tail = nullptr;   
    p->next = nullptr;                          
    return p;
}

Node* findElm(const Queue& q, const std::string& kode) {
    for (Node* p = q.head; p != nullptr; p = p->next)
        if (p->info.kode == kode) return p;
    return nullptr;
}

bool fFindElm(const Queue& q, const Node* p) {
    if (p == nullptr) return false;
    
    for (const Node* x = q.head; x != nullptr; x = x->next)
        if (x == p) return true;
    return false;
}

int nbList(const Queue& q) {
    int n = 0;
    for (Node* p = q.head; p != nullptr; p = p->next) n++;
    return n;
}

void printInfo(const Queue& q) {
    if (isEmptyQueue(q)) {
        std::cout << "(kosong)";
        return;
    }
    for (Node* p = q.head; p != nullptr; p = p->next) {
        std::cout << p->info.kode << "(" << p->info.berat << ")";
        if (p->next != nullptr) std::cout << " -> ";
    }
}

void delAll(Queue& q) {
    while (!isEmptyQueue(q)) {
        Node* p = dequeue(q);
        dealokasi(p);
    }
}

int posisiElm(const Queue& q, const std::string& kode) {
    int pos = 1;
    for (Node* p = q.head; p != nullptr; p = p->next, pos++)
        if (p->info.kode == kode) return pos;
    return 0;
}

long totalBerat(const Queue& q) {
    long t = 0;
    for (Node* p = q.head; p != nullptr; p = p->next) t += p->info.berat;
    return t;
}