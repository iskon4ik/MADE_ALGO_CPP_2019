/*

Iskander Falyakhov

Реализовать дек с динамическим зацикленным буфером.

Обрабатывать команды push * и pop *.

Формат ввода
В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Если дана команда pop *, то число b - ожидаемое значение. Если команда pop * вызвана для пустой структуры данных,
то ожидается “-1”.


in
3
1 44
3 50
2 44

out
YES


in
2
2 -1
1 10

out2
2 -1
1 10
YES


in
2
3 44
4 66

out
NO
*/

#include <iostream>
#include <cassert>

class Deque {
public:
    Deque();
    ~Deque();

    bool IsEmpty();
    void PushFront(int value);
    int PopFront();
    void PushBack(int value);
    int PopBack();

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

bool Deque::IsEmpty() {
    return data_size == 0;
}

void Deque::PushFront(int value) {
    if(data_size==0){
        data[start_idx] = value;
        data_size ++;
    }
    else{
        start_idx --;
        if(start_idx < 0){
            start_idx = capacity-1;
        }
        data[start_idx] = value;
        data_size ++;
    }
    ResizeBigger();
}

int Deque::PopFront() {
    assert(data_size > 0);
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
            if( A.IsEmpty() ) {
                if( b != -1 ) {
                    std::cout<<"NO";
                    return 0;
                }
            }
            else {
                int result = A.PopFront();
                if (result != b) {
                    std::cout << "NO";
                    return 0;
                }
            }
        } else if (a == 3) {
            A.PushBack(b);
        } else if (a == 4) {
            if( A.IsEmpty() ) {
                if( b != -1 ) {
                    std::cout<<"NO";
                    return 0;
                }
            }
            else {
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