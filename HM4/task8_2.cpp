/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
 удаления строки из множества и проверки принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.

Ввод
+ hello
+ bye
? bye
+ bye
- bye
? bye
? hello
Вывод
OK
OK
OK
FAIL
OK
FAIL
OK

+ 8
+ 9
+ 7
+ 5
+ 8
+ 2
+ 0
+ 9
+ 0
+ 4
- 7
- 3
- 0
- 9
- 8
? 6
? 2
? 8
? 9
? 5

 OK
 OK
 OK
 OK
 FAIL
 OK
 OK
 FAIL
 FAIL
 OK
 OK
 FAIL
 OK
 OK
 OK
 FAIL
 OK
 FAIL
 FAIL
 OK
 */
#include <iostream>
#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

using std::vector;
using std::string;
using std::cin;
using std::cout;

struct Hasher
{
    unsigned int operator()(const string& str) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < str.length(); i++)
            hash = hash * 127 + str[i];
        return hash;
    }
};

struct NilElement
{
    string operator()() const {
        return "";
    }
};

template<class T, class H, class N>
class HashTable
{
public:
    HashTable(const H& _hasher, const N& _nilelem);
    HashTable(const HashTable& t) = delete;
    HashTable& operator=(const HashTable& t) = delete;
    ~HashTable();

    bool Has(const T& key);
    bool Add(const T& key);
    bool Delete(const T& key);

private:
    H hasher;
    N nilelem;
    vector<T> table;
    vector<bool> deleted_status;
    unsigned int keysCount;
    unsigned int Probe(unsigned int hash, int i, const T& key);
    bool IsDeleted(int i);
    void growTable();
};


template<class T, class H, class N>
HashTable<T, H, N>::HashTable(const H& _hasher, const N& _nillelem) :
        hasher(_hasher),
        nilelem(_nillelem),
        table(8, _nillelem()),
        deleted_status(8, false),
        keysCount(0)
{
}

template<class T, class H, class N>
HashTable<T, H, N>::~HashTable()
{
    vector<T>().swap(table);
    vector<bool>().swap(deleted_status);
}

template<class T, class H, class N>
unsigned int HashTable<T, H, N>::Probe(unsigned int hash, int i, const T& key) {
    int k = std::abs(key[i % key.size()] - 'a');
    return hash + i*(2 * k + 1);
}

template <class T, class H, class N>
bool HashTable<T, H, N>::IsDeleted(int i) {
    return deleted_status[i];
}

template<class T, class H, class N>
bool HashTable<T, H, N>::Has(const T& key) {
    unsigned int hash = hasher(key) % table.size();
    for( int i=0; i<table.size(); ++i) {
        int j = Probe( hash, i, key) % table.size();
        if( table[j] == key && !IsDeleted(j) ) {
            return true;
        }
        if( table[j] == nilelem()) {
            return false;
        }
    }
    return false;
}

template<class T, class H, class N>
bool HashTable<T, H, N>::Add(const T& key)
{
    if (keysCount >= table.size() * 3/4)
        growTable();

    unsigned int hash = hasher(key) % table.size();
    bool faced_deleted = false;
    int index_of_first_deleted=0;
    for( int i=0; i<table.size(); ++i) {
        int j = Probe( hash, i, key) % table.size();
        if( table[j] == key && !IsDeleted(j)) {
            return false;
        }
        if(IsDeleted(j) && !faced_deleted) {
            faced_deleted = true;
            index_of_first_deleted = j;
        }
        if( table[j] == nilelem() ) {
            if( !faced_deleted ){
                table[j] = key;
                ++keysCount;
            } else {
                table[index_of_first_deleted] = key;
                deleted_status[index_of_first_deleted] = false;
            }
            return true;
        }
    }
    return false;
}

template<class T, class H, class N>
void HashTable<T, H, N>::growTable()
{
    vector<T> newTable(table.size() * 2, "");
    vector<bool> new_deleted_status(table.size() * 2, false);
    unsigned int new_keysCount = 0;
    for (int i = 0; i < table.size(); i++) {
        if (table[i] != nilelem() && !IsDeleted(i)) {
            T key = table[i];
            unsigned int hash = hasher(key) % newTable.size();
            for( int i_new=0; i_new<newTable.size(); ++i_new) {
                int j_new = Probe( hash, i_new, key) % newTable.size();
                if( newTable[j_new].empty() ) {
                    newTable[j_new] = key;
                    ++new_keysCount;
                    break;
                }
            }
        }
    }

    table = std::move(newTable);
    deleted_status = std::move(new_deleted_status);
    keysCount = new_keysCount;
}

template<class T, class H, class N>
bool HashTable<T, H, N>::Delete(const T& key)
{
    unsigned int hash = hasher(key) % table.size();
    for( int i=0; i<table.size(); ++i) {
        int j = Probe( hash, i, key) % table.size();
        if( table[j]==key && !IsDeleted(j)) {
            deleted_status[j] = true;
            return true;
        }
        if( table[j] == nilelem()) {
            return false;
        }
    }
    return false;
}

int main()
{
    Hasher hasher;
    NilElement nilelement;
    HashTable<string, Hasher, NilElement> table(hasher, nilelement);

    char command = 0;
    string word;
    while (cin >> command >> word) {
        switch (command) {
            case '+':
                cout << (table.Add(word) ? "OK" : "FAIL") << "\n";
                break;
            case '-':
                cout << (table.Delete(word) ? "OK" : "FAIL") << "\n";
                break;
            case '?':
                cout << (table.Has(word) ? "OK" : "FAIL") << "\n";
                break;
            default:
                assert(false);
        }
    }
    return 0;
}