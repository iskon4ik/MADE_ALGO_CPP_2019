/*
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта).

stdin
a
aaa
stdout
0 1 2

stdin
aba
abacababa
stdout
0 4 6

 */
#include <iostream>
#include <vector>
#include <string>

void find_entries_of_pattern(std::string& text, std::string& target_patrn)
{
    std::vector<int> prefix (target_patrn.length());

    prefix[0] = 0;
    for (int j = 0, i = 1; i < target_patrn.length(); ++i)
    {
        while ((j > 0) && (target_patrn[i] != target_patrn[j])){
            j = prefix[j - 1];
        }

        if (target_patrn[i] == target_patrn[j]){
            j++;
        }

        prefix[i] = j;
    }

    for (int k = 0, i = 0; i < text.length(); ++i)
    {
        while ((k > 0) && (target_patrn[k] != text[i])){
            k = prefix[k - 1];
        }

        if (target_patrn[k] == text[i]){
            k++;
        }

        if (k == target_patrn.length()){
            std::cout << (i - target_patrn.length() + 1) << " ";
        }
    }
}

int main() {
//    std::string text = "abacababa";
//    std::string pattern = "aba";

    std::string target_patrn;
    std::string text;

    std::cin >> target_patrn;
    std::cin>>text;

//    std::vector<int> prefix (target_patrn.length());
//
//    prefix[0] = 0;
//    for (int j = 0, i = 1; i < target_patrn.length(); ++i)
//    {
//        while ((j > 0) && (target_patrn[i] != target_patrn[j])){
//            j = prefix[j - 1];
//        }
//
//        if (target_patrn[i] == target_patrn[j]){
//            j++;
//        }
//
//        prefix[i] = j;
//    }
//
//    for (int k = 0, i = 0; i < text.length(); ++i)
//    {
//        while ((k > 0) && (target_patrn[k] != text[i])){
//            k = prefix[k - 1];
//        }
//
//        if (target_patrn[k] == text[i]){
//            k++;
//        }
//
//        if (k == target_patrn.length()){
//            std::cout << (i - target_patrn.length() + 1) << " ";
//        }
//    }
    find_entries_of_pattern(text, target_patrn);
    return 0;
}