/*
Для сложения чисел используется старый компьютер. Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
 Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка вычислений.
 ((1+2)+3) -> 1+2 + 3+3 = 9 ((1+3)+2) -> 1+3 + 4+2 = 10 ((2+3)+1) -> 2+3 + 5+1 = 11
 Требуется написать программу, которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел. Требуемое время работы O(n*log(n)).
Формат ввода
Вначале вводится n (n < 100) - количество чисел.
 Затем вводится n строк - значения чисел (значение каждого числа не превосходит 1 000 000 000,
 сумма всех чисел не превосходит 2 * 1 000 000 000).

in
5
5 2 3 4 6
out
45

in
5
3 7 6 1 9
out
56
*/

#include <iostream>

class Heap{
public:
    //Heap();
    Heap(int new_size);
    Heap(int * & arr, int n);
    ~Heap();

    int GetSize();
    void Add(int value);
    int TakeMin();
    void Print();

private:
    int * data;
    int size;
    int capacity;

    void SiftUp(int index);
    void SiftDown(int index);
};

int Heap::GetSize() {
    return size;
}

Heap::Heap(int new_size): data(new int [new_size]), size(0), capacity(new_size) {}

Heap::Heap(int * & arr, int n) {
    data = new int [n];
    size = n;
    for( int i=0; i<n; i++ ) {
        Add(arr[i]);
    }
}

Heap::~Heap() {
    delete [] data;
}

void Heap::SiftUp(int index) {
    while( data[index] < data[(index - 1) / 2] ) {
        std::swap(data[index], data[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void Heap::SiftDown(int index) {
    bool have_left_child = true;
    bool have_right_child = true;

    if( 2*index + 1 >= size ) {
        have_left_child = false;
    }
    if( 2*index + 2 >= size ) {
        have_right_child = false;
    }

    while((have_left_child && data[index]>data[2*index + 1]) || (have_right_child && data[index]>data[2*index + 2])){
        if( have_left_child && have_right_child ) {
            if(data[2*index + 1] <= data[2*index + 2]) {
                std::swap(data[index], data[2*index + 1]);
                index = 2*index + 1;
            }
            else {
                std::swap(data[index], data[2*index + 2]);
                index = 2*index + 2;
            }
        }
        else if( have_left_child ) {
            std::swap(data[index], data[2*index + 1]);
            index = 2*index + 1;
        }
        else {
            std::swap(data[index], data[2*index + 2]);
            index = 2*index + 2;
        }

        if( 2*index + 1 >= size ) {
            have_left_child = false;
        }
        if( 2*index + 2 >= size ) {
            have_right_child = false;
        }
    }
}

void Heap::Add(int value) {
    size ++;
    data[size-1] = value;
    if( size > 1 ) {
        SiftUp(size-1);
    }
    //Resize();
}

int Heap::TakeMin() {
    int result = data[0];
    //std::cout<<result<<", ";
    std::swap(data[0], data[size-1]);
    size--;
    SiftDown(0);
    return result;
}

void Heap::Print() {
    if( size==0 ) {
        std::cout<<"(Empty)"<<std::endl;
    }
    else {
        int print_on_layer = 1;
        int printed = 0;
        while(printed < size){
            for( int i=printed; i<printed+print_on_layer; i++ ){
                std::cout<<data[i]<<" ";
            }
            printed += print_on_layer;
            std::cout<<std::endl;
            if( printed + 2*print_on_layer <= size ) {
                print_on_layer *= 2;
            }
            else {
                print_on_layer = size - printed;
            }
        }
    }
    std::cout<<std::endl;
}

int CalculateTime(Heap H) {
    int sum_min_2 = 0;
    int time = 0;
    while( H.GetSize() > 1 ) {
        int temp = H.TakeMin();
        sum_min_2 = temp;
        temp = H.TakeMin();
        sum_min_2 += temp;
        H.Add(sum_min_2);
        time += sum_min_2;
    }
    return time;
}

int main() {

    int n;
    std::cin>>n;

    //Heap A = Heap(n);

    int * arr = new int [n];

    for( int i=0; i<n; i++ ) {
        int number;
        std::cin>>number;
        //A.Add(number);
        arr[i] = number;
    }

    Heap A = Heap(& arr, n);

    //A.Print();

    int sum_min_2 = 0;
    int answer = 0;
    while( A.GetSize() > 1 ) {
        int temp = A.TakeMin();
        sum_min_2 = temp;
        temp = A.TakeMin();
        sum_min_2 += temp;
        A.Add(sum_min_2);
        answer += sum_min_2;
    }

    answer = CalculateTime(A);

    std::cout<<answer;

    return 0;
}