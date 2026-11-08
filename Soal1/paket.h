#ifndef PAKET_H
#define PAKET_H

#include <string>

struct Paket {
    std::string kode;  
    int berat;         
};

struct Node {
    Paket info;
    Node* next;
};

struct Queue {
    Node* head;
    Node* tail;
};

struct Statistik {
    int terkirim;       
    long totalBerat;    
};

void createQueue(Queue& q);
bool isEmptyQueue(const Queue& q);

Node* alokasi(const std::string& kode, int berat);   
void dealokasi(Node*& p);                            

void enqueue(Queue& q, Node* p);                     
Node* dequeue(Queue& q);                             

Node* findElm(const Queue& q, const std::string& kode);
bool fFindElm(const Queue& q, const Node* p);        
int nbList(const Queue& q);
void printInfo(const Queue& q);
void delAll(Queue& q);

// fungsi bantu
int posisiElm(const Queue& q, const std::string& kode);  
long totalBerat(const Queue& q);

#endif