#include <iostream>
//#include "Huffman.h"
#include <vector>
#include <map>
#include <list>
#include <string>
#include <fstream>

typedef unsigned char byte;

struct IInputStream {
    virtual // Возвращает false, если поток закончился
    bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};

class BitsReader {
public:
    void ReadBit(bool bit);
    void ReadByte(unsigned char byte);

    std::vector<unsigned char> GetResult();

private:
    std::vector<unsigned char> buffer_;
    unsigned char accumulator_ = 0;
    int bits_count_ = 0;
};

void BitsReader::ReadBit(bool bit) {
    // Ставим бит в аккумулятор на нужное место
    accumulator_ |= static_cast<unsigned char>(bit) << bits_count_;
    ++bits_count_;
    if (bits_count_ == 8) {
        bits_count_ = 0;
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
    }
}

void BitsReader::ReadByte(unsigned char byte) {
    if (bits_count_ == 0) {
        buffer_.push_back(byte);
    } else {
        accumulator_ |= byte << bits_count_;
        buffer_.push_back(accumulator_);
        accumulator_ = byte >> (8 - bits_count_);
    }
}

std::vector<unsigned char> BitsReader::GetResult() {
    if (bits_count_ != 0) {
        // Добавляем в буфер аккумулятор, если в нем что-то есть.
        buffer_.push_back(accumulator_);
    }
//    buffer_.push_back(static_cast<unsigned char>(bits_count_));
    return std::move(buffer_);
}

class BitsWriter {
public:
    void WriteBit(bool bit);
    void WriteByte(unsigned char byte);

    std::vector<unsigned char> GetResult();

private:
    std::vector<unsigned char> buffer_;
    unsigned char accumulator_ = 0;
    int bits_count_ = 0;
};

void BitsWriter::WriteBit(bool bit) {
    // Ставим бит в аккумулятор на нужное место
    accumulator_ |= static_cast<unsigned char>(bit) << bits_count_;
    ++bits_count_;
    if (bits_count_ == 8) {
        bits_count_ = 0;
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
    }
}

void BitsWriter::WriteByte(unsigned char byte) {
    if (bits_count_ == 0) {
        buffer_.push_back(byte);
    } else {
        accumulator_ |= byte << bits_count_;
        buffer_.push_back(accumulator_);
        accumulator_ = byte >> (8 - bits_count_);
    }
}

std::vector<unsigned char> BitsWriter::GetResult() {
    if (bits_count_ != 0) {
        // Добавляем в буфер аккумулятор, если в нем что-то есть.
        buffer_.push_back(accumulator_);
    }
    buffer_.push_back(static_cast<unsigned char>(bits_count_));
    return std::move(buffer_);
}

class Node
{
public:
    int a;
    byte c;
    Node *left, *right;

    Node(){left=right=nullptr;}

    Node(Node *L, Node *R)
    {  left =  L;
        right = R;
        a = L->a + R->a;  }
};

struct MyCompare
{
    bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};

class Tree {
public:
    Node * root = nullptr;
};

std::vector<bool> code;
std::map<char,std::vector<bool> > table;

Tree tree;

void BuildTable(Node *root)
{
    if (root->left!=nullptr)
    {
        code.push_back(false);
        BuildTable(root->left);
    }

    if (root->right!=nullptr)
    {
        code.push_back(true);
        BuildTable(root->right);
    }

    if (root->left==nullptr && root->right==nullptr) table[root->c]=code;

    code.pop_back();
}

std::vector<byte> ToVector(IInputStream& input)
//std::vector<byte> ToVector(std::ifstream& input)
{
    std::vector<byte> vec;
    byte value;
//    while (input >> value)
//    input.read()
//    while (input.read(reinterpret_cast<char *>(value), 8))
    while(input.Read(value))
    {
        vec.push_back(value);
    }
    return vec;
}

void FromVector(const std::vector<byte>& vec, IOutputStream& output)
//void FromVector(const std::vector<byte>& vec, std::ofstream& output)
{
    for (const byte& value: vec)
    {
        byte new_value = (value & 0xF0) >> 4 | (value & 0x0F) << 4;
        new_value = (new_value & 0xCC) >> 2 | (new_value & 0x33) << 2;
        new_value = (new_value & 0xAA) >> 1 | (new_value & 0x55) << 1;
        output.Write(new_value);
//        output << char(value);
    }

//    for (unsigned char byte : vec) {
//        for (int i = 7; i >= 0; --i) {
//            std::cout << ((byte >> i) & 1);
//        }
//        std::cout << " ";
//    }
}

void Encode(IInputStream& original, IOutputStream& compressed)
//void Encode(std::ifstream& original, std::ofstream& compressed)
{
//    copyStream(original, compressed);
    BitsReader bits_reader;

//    for (unsigned char byte : original) {
//        bits_reader.ReadByte(byte);
//    }

    std::vector<byte> temp = ToVector(original);

    std::map<byte, int> freq;

    for(unsigned char i : temp) {
        freq[i]++;
    }

    std::list<Node*> t;
    for(auto & itr : freq)
    {
        Node *p = new Node;
        p->c = itr.first;
        p->a = itr.second;
        t.push_back(p);
    }

    while (t.size()!=1)
    {
        t.sort(MyCompare());

        Node *SonL = t.front();
        t.pop_front();
        Node *SonR = t.front();
        t.pop_front();

        Node *parent = new Node(SonL,SonR);
        t.push_back(parent);

    }
//    Node *root = t.front();
    tree.root = t.front();
    BuildTable(tree.root);

    BitsWriter bits_writer;
    for(int i=0; i<temp.size(); ++i) {
        std::vector<bool> char_code = table[temp[i]];
        for(int j=0; j<char_code.size(); ++j) {
//            new_temp.push_back(char_code[j]);
            bits_writer.WriteBit(char_code[j]);
        }
    }

    std::vector<byte> new_temp = std::move(bits_writer.GetResult());

    FromVector(new_temp, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original)
//void Decode(std::ifstream& compressed, std::ofstream& original)
{
//    copyStream(compressed, original);
    std::vector<byte> temp = ToVector(compressed);
    Node *p = tree.root;

    BitsReader bits_reader;

    int amount_bit = 0;
    if(temp[temp.size() - 1] == 0) {
        amount_bit = (temp.size() - 1) * 8;
    } else {
        for(int i=0; i<temp.size() - 2; ++i) {
            amount_bit += 8;
        }
        amount_bit += int(temp[temp.size() - 1]);
    }

    int byte_count = 0;
    byte byte_new = temp[byte_count];
    int count = 0;

    for(int i=0; i<amount_bit; ++i) {
        bool b = byte_new & (1 << (7 - count) );
//        bool b = byte_new & (1 >> (7 - count) );
        if (b) p=p->right; else p=p->left;
        if (p->left==nullptr && p->right==nullptr) {
//            new_temp.push_back(p->c);
            bits_reader.ReadByte(p->c);
            p=tree.root;
        }
        count++;
        if (count==8) {
            count=0;
//            byte = F.get();
            byte_count ++;
            if(byte_count<temp.size()-2){
                byte_new = temp[byte_count];
            }
        }
    }
//    for(int i=0; i<amount_bit; ++i) {
//        bits_reader.ReadBit(temp[i]);
//    }

//    count=0; char byte;
//    byte = F.get();
//    while(!F.eof())
//    {   bool b = byte & (1 << (7-count) ) ;
//        if (b) p=p->right; else p=p->left;
//        if (p->left==NULL && p->right==NULL) {cout<<p->c; p=root;}
//        count++;
//        if (count==8) {count=0; byte = F.get();}
//    }
    std::vector<unsigned char> new_temp = std::move(bits_reader.GetResult());
    FromVector(new_temp, original);
}

int main() {
//    std::ifstream input("input.txt", std::ios::binary);
//
//    std::vector<char> bytes(
//            (std::istreambuf_iterator<char>(input)),
//            (std::istreambuf_iterator<char>()));
//
//    input.close();
//
//    for (unsigned char byte : bytes) {
//        for (int i = 0; i < 8; ++i) {
//            std::cout << ((byte >> i) & 1);
//        }
//        std::cout << " ";
//    }
//    std::cout<<std::endl;



//    std::ifstream infile;
//    std::ofstream outfile;
//
//    infile.open("input.txt");
//    outfile.open("output.txt");
//
//    char t;
//    std::cin >> t;
//
//    std::cout<<t;
//    Encode(infile, outfile);
//    infile.close();
//    outfile.close();

//    infile.open("output.txt");
//    outfile.open("result.txt");
//
//    Decode(infile, outfile);
//    infile.close();
//    outfile.close();

    BitsWriter bits_writer;

    bits_writer.WriteBit(true);
    bits_writer.WriteBit(false);
    bits_writer.WriteByte(0xFE);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(true);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(false);
    bits_writer.WriteByte(0xFE);
    bits_writer.WriteBit(true);
    bits_writer.WriteBit(false);
    std::vector<unsigned char> result =
            std::move(bits_writer.GetResult());

    for (unsigned char byte : result) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
    std::cout<<std::endl;

    for (unsigned char byte : result) {
        for (int i = 7; i >= 0; --i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
    std::cout<<std::endl;

    for (unsigned char byte : result) {
        unsigned char new_byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
        new_byte = (new_byte & 0xCC) >> 2 | (new_byte & 0x33) << 2;
        new_byte = (new_byte & 0xAA) >> 1 | (new_byte & 0x55) << 1;
        for (int i = 0; i < 8; ++i) {
            std::cout << ((new_byte >> i) & 1);
        }
        std::cout << " ";
    }
    std::cout<<std::endl;

//    for (int i = 0; i < 8; ++i) {
//        std::cout << ((0xFE >> i) & 1);
//    }
    BitsReader bits_reader;

    for (unsigned char byte : result) {
        bits_reader.ReadByte(byte);
//        for (int i = 0; i < 8; ++i) {
////            std::cout << ((byte >> i) & 1);
//        }
//        std::cout << " ";
    }

    std::vector<unsigned char> result_reader = std::move(bits_reader.GetResult());

    for (unsigned char byte : result_reader) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
    std::cout<<std::endl;

//    std::vector<int> test(5, 0);

//    std::vector<int> test = {1, 2, 3, 4, 5};
//
//    std::cout<<test.size()<<" "<<test[0]<<" "<<test[test.size() - 1]<<std::endl;
//
//    byte b_test = 00000000;
//
//    std::cout<<b_test;

//    char test_char = (char) 'A';
//
//    std::cout<<test_char;



    return 0;
}