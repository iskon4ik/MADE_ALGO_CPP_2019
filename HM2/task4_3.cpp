/*
Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n).
Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент.
Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j. Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

 4_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.


In
10 4
1 2 3 4 5 6 7 8 9 10
Out
5

In
10 0
3 6 5 7 2 9 8 10 4 1
Out
1

In
10 9
0 0 0 0 0 0 0 0 0 1
Out
1

*/
#include <iostream>
#include <cstdlib>
#include <random>

template <class T>
class IsLessDefaultFunctor {
public:
    bool operator()(const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class IsLess=IsLessDefaultFunctor<T> >
int Partition(T * a, int l, int r, IsLess isLess = IsLess()) {
    if( r-l == 1){
        return l;
    }

    int pivot_index = std::rand() % (r-l) + l;

    std::swap(a[pivot_index], a[r-1]);
    int i = l, j = l;
    while(j < r-1) {
        if( ! isLess(a[j], a[r-1]) ) {
            j++;
        }
        else {
            std::swap(a[i], a[j]);
            i++;
            j++;
        }
    }
    std::swap(a[i], a[r-1]);
    return i;
}

template <class T, class IsLess=IsLessDefaultFunctor<T> >
int FindKStatistic(T * a, int l, int r, int k, IsLess isLess = IsLess()) {
    while( true ) {
        int index_after_partition = Partition(a, l, r, isLess);
        //int index_after_partition = std::nth_element(k, a);
        if( index_after_partition == k ) {
            return a[index_after_partition];
        }
        else if( k < index_after_partition ) {
            r = index_after_partition;
        }
        else {
            l = index_after_partition + 1;
        }
    }
}

int main() {

    int n, k;

    std::cin>>n>>k;

    int * arr = new int [n];

    for( int i=0; i<n; i++ ) {
        std::cin>>arr[i];
    }

    std::cout<<FindKStatistic(arr, 0, n, k);
    delete [] arr;

    return 0;
}