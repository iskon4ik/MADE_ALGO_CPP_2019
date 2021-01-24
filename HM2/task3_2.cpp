/*
 Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
 Требуется отсортировать последовательность. Последовательность может быть очень длинной.
 Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.

 in
 10 4
 0 4 3 2 1 8 7 6 5 9

 out
 0 1 2 3 4 5 6 7 8 9
 */


#include <iostream>
#include <algorithm>

template <class T>
class IsLessDefaultFunctor {
public:
    bool operator()(const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class IsLess=IsLessDefaultFunctor<T> >
void Merge(T * a, int l, int mid, int r, IsLess isLess = IsLess()) {
    int * b = new int [r-l];

    int b1 = l;
    int e1 = mid;
    int b2 = e1;

    int curr = 0;

    while( curr < r-l ) {
        if( b1 < e1 && b2 < r) {
            if( a[b1] < a[b2]) {
                b[curr] = a[b1];
                b1++;
            } else {
                b[curr] = a[b2];
                b2++;
            }
        }
        else if( b1 < e1 ) {
            b[curr] = a[b1];
            b1++;
        }
        else {
            b[curr] = a[b2];
            b2++;
        }
        curr++;
    }

    for( int i=0; i < curr; i++ ) {
        a[i + l] = b[i];
    }

    delete [] b;
}

template <class T, class IsLess=IsLessDefaultFunctor<T> >
void MergeSort(T * a, int l, int r, IsLess isLess = IsLess()) {
    if( r-l < 2) {
        return;
    }
    if( r-l == 2 ) {
        if( !isLess(a[l], a[l+1]) ) {
            std::swap(a[l], a[l+1]);
        }
        return;
    }

    MergeSort(a, l,l + (r - l)/2);
    MergeSort(a, l + (r - l)/2, r);

    Merge(a, l, l + (r - l)/2, r, isLess);
}

template <class T, class IsLess=IsLessDefaultFunctor<T> >
void MergeSortByK(T * arr, int n, int k, IsLess isLess = IsLess()) {
    int l = 0, r = std::min(k, n);
    int prev_r = 0;
    do {
        MergeSort(arr, l, r, isLess);
        if(r > k) {
            Merge(arr, l-k, l, r, isLess);
        }
        l += k;
        prev_r = r;
        r = std::min(r + k, n);
    } while(prev_r != r);
}

int main() {
    int n, k;

    std::cin>>n>>k;

    int * arr = new int [n];
    for( int i=0; i<n; i++ ) {
        std::cin>>arr[i];
    }

    MergeSortByK(arr, n, k);

    for( int i=0; i<n; i++ ) {
        std::cout<<arr[i]<<' ';
    }

    delete [] arr;

    return 0;
}