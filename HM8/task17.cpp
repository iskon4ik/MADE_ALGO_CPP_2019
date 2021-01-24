/*
?a?aa?
ааааааа
Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
Найти позиции всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает,
 что все обычные символы совпадают с соответствующими из текста,
 а вместо символа “?” в тексте встречается произвольный символ.
Гарантируется, что сам “?” в тексте не встречается.
Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона “между вопросиками” в исходном тексте.
(Ахо-Корасик)
m ≤ 5000, n ≤ 2000000. Время: 10с, память 32Mb.

Ввод
ab??aba
ababacaba
Вывод
2

Ввод
aa??bab?cbaa?
aabbbabbcbaabaabbbabbcbaab
Вывод
0 13

Ввод
a?a
abaca
Вывод
0 2
*/

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <unordered_map>

#include <chrono>
#include<assert.h>
#include <sstream>
#include <iterator>

//std::unordered_map<string, int> umap;

int const size_of_alphabet = 26;

class AxoKorasik {
public:
    explicit AxoKorasik(const std::string & msk);
    std::vector<int> FindEntriesOfPattern(const std::string & text);

private:
    struct Node;

    std::string mask;
    std::vector<Node> nodes;
    std::vector<std::pair<int, int>> submask_positions;
//    std::vector<std::string> patterns;

    void AddPatternToBor(const std::pair<int, int> & submask_position, int pattern_index);

    void FindSubmaskPositions(const std::string & msk);

    int GetSuffixLink(int index);
    int GetGoodSuffixLink(int index);
    int GetLetterLink(int index, char character);
};

struct AxoKorasik::Node {
    Node(int parnt, char parnt_chr);

//    int edges[size_of_alphabet];
//    std::unordered_map<char, int> edges;
    std::map<char, int> edges;
//    int letter_link[size_of_alphabet];
//    std::unordered_map<char, int> letter_link;
    std::map<char, int> letter_link;
    int suffix_link;
    int good_suffix_link;
    int parent;
    char parent_char;
    bool is_the_end_of_pattern;

    std::vector<int> indices_of_patterns_connected;
};



AxoKorasik::Node::Node(int parnt, char parnt_chr)
        : parent_char(parnt_chr) , parent(parnt), suffix_link(-1), good_suffix_link(-1),
        is_the_end_of_pattern(false) {

//    for (int j = 0; j < size_of_alphabet; j++) {
//        edges[j] = -1;
//        letter_link[j] = -1;
//    }
}

AxoKorasik::AxoKorasik(const std::string & msk)
        : nodes(1, Node(0, -1))
        , mask(msk) {
    nodes[0].suffix_link = 0;

    FindSubmaskPositions(msk);

    for (size_t i = 0; i < submask_positions.size(); i++) {
        AddPatternToBor(submask_positions[i], i);
    }
}

void AxoKorasik::FindSubmaskPositions(const std::string &msk) {
    std::pair<int, int> current_submask_position;

    if (msk[0] != '?') {
        current_submask_position.first = 0;
    }

    if (msk[1] == '?' && msk[0] != '?') {
        current_submask_position.second = 0;
        submask_positions.push_back(current_submask_position);
    }

    for (size_t i = 1; i < msk.length() - 1; i++) {

        if (msk[i - 1] == '?' && msk[i] != '?') {
            current_submask_position.first = i;
        }

        if (msk[i + 1] == '?' && msk[i] != '?') {
            current_submask_position.second = i;
            submask_positions.push_back(current_submask_position);
        }

    }

    if (msk[msk.length() - 2] == '?' && msk[msk.length() - 1] != '?') {
        current_submask_position.first = (int)msk.length() - 1;
    }

    if (msk[msk.length() - 1] != '?') {
        current_submask_position.second = (int)(msk.length() - 1);
        submask_positions.push_back(current_submask_position);
    }
}

void AxoKorasik::AddPatternToBor(const std::pair<int, int> & submask_position, int pattern_index) {
    int current_node = 0;

    for (size_t i = submask_position.first; i <= submask_position.second; i++) {

//        char chr = mask[i] - 'a';
        char chr = mask[i];
        auto it = nodes[current_node].edges.find(chr);
//        if (nodes[current_node].edges[chr] == -1)
        if(it == nodes[current_node].edges.end())
        {
            nodes.emplace_back(current_node, chr);
            nodes[current_node].edges[chr] = (int)(nodes.size() - 1);
        }

        current_node = nodes[current_node].edges[chr];

    }

    nodes[current_node].is_the_end_of_pattern = true;
    nodes[current_node].indices_of_patterns_connected.push_back(pattern_index);
}



int AxoKorasik::GetSuffixLink(int index) {
    if (nodes[index].suffix_link == -1) {
        if (index == 0  || nodes[index].parent == 0) {
            nodes[index].suffix_link = 0;
        } else {
            nodes[index].suffix_link = GetLetterLink(GetSuffixLink(nodes[index].parent), nodes[index].parent_char);
        }
    }
    return nodes[index].suffix_link;
}

int AxoKorasik::GetGoodSuffixLink(int index) {
    if(nodes[index].good_suffix_link == -1) {
        int u = GetSuffixLink(index);
        if( u==0 ) {
            nodes[index].good_suffix_link = 0;
        } else {
            nodes[index].good_suffix_link = (nodes[u].is_the_end_of_pattern)?u:GetGoodSuffixLink(u);
        }
    }
    return nodes[index].good_suffix_link;
}

int AxoKorasik::GetLetterLink(int index, char character) {

    auto it = nodes[index].letter_link.find(character);

//    if (nodes[index].letter_link[character] == -1)
    if(it == nodes[index].letter_link.end())
    {
        auto it2 = nodes[index].edges.find(character);

//        if (nodes[index].edges[character] != -1)
        if(it2 != nodes[index].edges.end())
        {
            nodes[index].letter_link[character] = nodes[index].edges[character];
        } else if (index == 0) {
            nodes[index].letter_link[character] = 0;
        } else {
            nodes[index].letter_link[character] = GetLetterLink(GetSuffixLink(index), character);
        }

    }

    return nodes[index].letter_link[character];
}

std::vector<int> AxoKorasik::FindEntriesOfPattern(const std::string & text) {

    std::vector<int> entries(text.length(), 0);
    int v = 0;

    for (int i = 0; i < text.length(); i++) {
//        v = GetLetterLink(v, text[i] - 'a');
        v = GetLetterLink(v, text[i]);
        int u = v;
        do {
            if (nodes[u].is_the_end_of_pattern) {
                for (int index : nodes[u].indices_of_patterns_connected) {
                    int start_index = i - submask_positions[index].second
                                      + submask_positions[index].first;

                    if ((start_index - submask_positions[index].first >= 0) &&
                        (start_index - submask_positions[index].first + mask.length() - 1 < text.length()) ) {
                        entries[start_index - submask_positions[index].first]++;
                    }
//                    entries[i - (submask_positions[index].second - submask_positions[index].first + 1)]++;
                }
            }
            u = GetGoodSuffixLink(u);
        } while (u != 0);
//        while(GetLetterLink(v, text[i]) == -1) {
//            v = GetGoodSuffixLink(v);
//        }
//        v = GetLetterLink(v, text[i]);
//        for(int index: nodes[v].indices_of_patterns_connected) {
//            int start_index = i - submask_positions[index].second + submask_positions[index].first;
//            if(start_index - submask_positions[index].first >=0 &&
//            start_index - submask_positions[index].first <text.length()) {
//                entries[start_index - submask_positions[index].first]++;
//            }
//        }
    }

    std::vector<int> result;
    for (size_t i = 0; i < entries.size(); i++) {
        if (entries[i] == submask_positions.size()) {
            result.push_back(i);
        }
    }

    return result;
}

bool test_ex(const std::string& inputPattern, const std::string& inputText, const std::string& answer)
{
    std::ostringstream oss;

    if(inputPattern.empty() || inputText.empty() || inputPattern.size() > inputText.size()) {
//        std::cout<< "";
        return answer.empty();
//        return 0;
    }

    bool all_q = true;
    for(char i : inputPattern) {
        if(i != '?') {
            all_q = false;
            break;
        }
    }

    std::vector<int> myanswer;

    if(all_q) {
        myanswer = std::vector<int>(inputText.size() - inputPattern.size() + 1);
        for(int i=0; i<myanswer.size(); ++i) {
            myanswer[i] = i;
        }
    } else {
        AxoKorasik ahoCorasickMachine(inputPattern);
        myanswer = ahoCorasickMachine.FindEntriesOfPattern(inputText);
    }

    if (!myanswer.empty())
    {
        // Convert all but the last element to avoid a trailing ","
        std::copy(myanswer.begin(), myanswer.end()-1,
                  std::ostream_iterator<int>(oss, " "));

        // Now add the last element with no delimiter
        oss << myanswer.back();
    }
    std::cout<<'"'<<oss.str()<<'"'<<answer<<'"'<<std::endl;
    return oss.str() == answer;
}

int main() {

//    std::string target_pattern = "aa??bab?cbaa?";
//    std::string text = "aabbbabbcbaabaabbbabbcbaab";

    std::string target_pattern, text;
//    std::cin >> target_pattern >> text;

    text = std::string(2000000, 'a');

    for(int i=0; i<text.size(); ++i) {
        if(i%2==0 || i%3 == 0) {
            text[i] = 'b';
        }
    }

    target_pattern = std::string(5000, 'a');

    for(int i=0; i<target_pattern.size(); ++i) {
//        if(i%2==0) {
//            target_pattern[i] = 'b';
//        }
//        else
        if(i%2==0) {
            target_pattern[i] = '?';
        }
    }

//    assert(test_ex("aaccca?aac?c?ccc?cbbb", "aabbaaacbccbbcbcabccaaaacccabaacccbcccbcbbbbaaab","22")&&"Manual test 4159");
//
//    assert(test_ex("a?aa?aaa?aaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22")&&"Error with progr -1");
////
//    assert(test_ex("ab??aba", "ababacaba","2")&&"Error with progr 1");
////
//    assert(test_ex("sXe??;:j?:", "sXeZi;:j;:A7eCUWHU8We","0")&&"Error with progr 2");
////
//    assert(test_ex("at?art?oars?soar", "soarsoars","")&&"Error with progr 3");
//
//    assert(test_ex("i?in?tin?sting", "sting","")&&"Error with progr 4");
//    assert(test_ex("vine?vincent?cent?center", "vincentcentervine","")&&"Error with progr 5");
//    assert(test_ex("a?a", "a","")&&"Error with progr 6");
//    assert(test_ex("?n", "onOoSOg","0")&&"Error with progr 7");
//    assert(test_ex("ab??aba", "ababacaba","2")&&"Error with progr 8");
//    assert(test_ex("??", "bbaaaaaaaaaaaaa","0 1 2 3 4 5 6 7 8 9 10 11 12 13")&&"Error with progr 9");
//    assert(test_ex("a??", "abcaaaaab","0 3 4 5 6")&&"Error with progr 10");
//    assert(test_ex("a?aa?aaa??a?aa?", "aaaaaaaaaaaaaaa","0")&&"Error with progr 11");
//    assert(test_ex("a?aa?aaa??a?aa?", "","")&&"Error with progr 12");
//    assert(test_ex("ab??aba", "d","")&&"Error with progr 13");
//    assert(test_ex("", "dasdasd","")&&"Error with progr 14");
//    assert(test_ex("ba?aab?abab", "aababab","")&&"Error with progr 15");
//    assert(test_ex("???", "abcd","0 1")&&"Error with progr 16");
//    assert(test_ex("abc", "bbbc","")&&"Error with progr 17");
//    assert(test_ex("abc", "","")&&"Error with progr 18");

    if(target_pattern.empty() || text.empty() || target_pattern.size() > text.size()) {
        std::cout<< "";
        return 0;
    }

    bool all_q = true;
    for(char i : target_pattern) {
        if(i != '?') {
            all_q = false;
            break;
        }
    }

    std::vector<int> answer;

    auto begin = std::chrono::high_resolution_clock::now();

    if(all_q) {
        answer = std::vector<int>(text.size() - target_pattern.size() + 1);
        for(int i=0; i<answer.size(); ++i) {
            answer[i] = i;
        }
    } else {
        AxoKorasik ahoCorasickMachine(target_pattern);
        answer = ahoCorasickMachine.FindEntriesOfPattern(text);
    }
//
////    ba?aab?abab
////    aababab
//

//    AxoKorasik ahoCorasickMachine(target_pattern);
//
//    std::vector<int> answer = ahoCorasickMachine.FindEntriesOfPattern(text);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout<<"Answer Time: "<<std::chrono::duration_cast<std::chrono::seconds>(end-begin).count()<<std::endl;
    std::cout<<answer.size();
//
//    for (int i : answer) {
//        std::cout << i << " ";
//    }

    return 0;
}