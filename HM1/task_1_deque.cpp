//
// Created by iskander on 22.10.2019.
// 1_2 Реализовать дек с динамическим зацикленным буфером.
//
//
//

#include <iostream>
#include <cassert>

class Deque {
public:
    Deque();
    ~Deque();

    void PushFront(int value);
    int PopFront();
    void PushBack(int value);
    int PopBack();
    int GetSize();

private:
    int * data;
    int data_size;
    int capacity;
    int start_idx;

    void ResizeBigger();
    void ResizeSmaller();
};

Deque::Deque(): data(new int[1]), data_size(0), capacity(1), start_idx(0) {}

Deque::~Deque() {
    delete[] data;
}

int Deque::GetSize() {
    return data_size;
}

void Deque::PushFront(int value) {
    assert(data_size>0);
    start_idx --;
    if(start_idx < 0){
        start_idx = capacity-1;
    }
    data[start_idx] = value;
    data_size ++;
    ResizeBigger();
}

int Deque::PopFront() {
    if(data_size==0){
        return -1;
    }
    int result = data[start_idx];
    data_size --;
    start_idx ++;
    if(start_idx >= capacity){
        start_idx = 0;
    }
    ResizeSmaller();
    return result;
}

void Deque::PushBack(int value) {
    int back_idx = (start_idx + data_size) % capacity;
    data[back_idx] = value;
    data_size ++;
    ResizeBigger();
}

int Deque::PopBack() {
    assert(data_size > 0);
    int back_idx = (start_idx + data_size) % capacity;
    back_idx --;
    if(back_idx < 0){
        back_idx = capacity - 1;
    }
    int result = data[back_idx];
    data_size --;
    ResizeSmaller();
    return result;
}

void Deque::ResizeBigger() {
    if(data_size == capacity){
        int * new_data = new int [capacity*2];
        int copied_idx = start_idx;
        for(int n_copied = 0; n_copied < data_size; n_copied++){
            new_data[n_copied] = data[copied_idx];
            copied_idx ++;
            if(copied_idx >= capacity){
                copied_idx = 0;
            }
        }
        delete [] data;
        data = new_data;
        capacity *= 2;
        start_idx = 0;
    }
}

void Deque::ResizeSmaller() {
    if(data_size == capacity/2 - 1 && capacity >= 2){
        int * new_data = new int [capacity/2];
        int copied_idx = start_idx;
        for(int n_copied = 0; n_copied < data_size; n_copied++){
            new_data[n_copied] = data[copied_idx];
            copied_idx ++;
            if(copied_idx >= capacity){
                copied_idx = 0;
            }
        }
        delete [] data;
        data = new_data;
        capacity /= 2;
        start_idx = 0;
    }
}

int main() {

    Deque A;


    int n;
    std::cin >> n;
    int a, b;

    for (int i = 0; i < n; i++) {
        std::cin >> a >> b;
        if (a == 1) {
            A.PushFront(b);
        } else if (a == 2) {
            if(A.GetSize() == 0) {
                if( b != -1) {
                    std::cout << "NO";
                    return 0;
                }
            } else {
                int result = A.PopFront();
                if (result != b) {
                    std::cout << "NO";
                    return 0;
                }
            }
        } else if (a == 3) {
            A.PushBack(b);
        } else if (a == 4) {
            if(A.GetSize() == 0) {
                if( b != -1) {
                    std::cout << "NO";
                    return 0;
                }
            } else {
                int result = A.PopBack();
                if (result != b) {
                    std::cout << "NO";
                    return 0;
                }
            }
        }
    }
    std::cout << "YES";
    return 0;
}

/*
A.PushFront(99);
A.PopFront();
A.PushBack(1234);
A.PushFront(432);
A.PushBack(666);
A.PushFront(239);
A.PopBack();
A.PopFront();
A.PushBack(111);
A.PushBack(222);
A.PushBack(333);
A.PushBack(444);
A.PushBack(555);
A.PushBack(666);
A.PushBack(777);
A.PushBack(888);
A.PushBack(999);
A.PushBack(1000);

A.PushFront(-1);
A.PushFront(-2);
A.PushFront(-3);
A.PushFront(-4);
A.PushFront(-5);
A.PushFront(-6);
A.PushFront(-7);
A.PushFront(-8);

A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
A.PopBack();
*/

/*
    return 0;

    int mas[15];

    for(int i=0; i<15; i++){
        mas[i] = i*2;
    }

    for(int i=1; i<=15; i++){
        if(i%2 == 0){
            std::cout<<"PushFront "<<mas[i]<<std::endl;
            A.PushFront(mas[i]);
        }
        else {
            std::cout<<"PushBack "<<mas[i]<<std::endl;
            A.PushBack(mas[i]);
        }
        A.Print();
        std::cout<<std::endl;
    }

    for(int i=1; i<=13; i++){
        if(i%5 == 0){
            std::cout<<"PopFront "<<A.PopFront()<<std::endl;
            //A.PushFront(mas[i]);
        }
        else {
            std::cout<<"PopBack "<<A.PopBack()<<std::endl;
            //A.PushBack(mas[i]);
        }
        A.Print();
        std::cout<<std::endl;
    //}
*/

