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

template<class T, class H>
class HashTable
{
public:
	HashTable(const H& _hasher);
	HashTable(const HashTable& t) = delete;
	HashTable& operator=(const HashTable& t) = delete;
	~HashTable();

	bool Has(const T& key) const;
	bool Add(const T& key);
	bool Delete(const T& key);

private:
	struct Node {
		T Key;
		Node* Next;

		Node() : Next(nullptr) {}
		Node(const T& key, Node* next) : Key(key), Next(next) {}
	};
	H hasher;
	vector<Node*> table;
	unsigned int keysCount;

	void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable(const H& _hasher) :
	hasher(_hasher),
	table(8, nullptr),
	keysCount(0)
{
}

template<class T, class H>
HashTable<T, H>::~HashTable()
{
	for (int i = 0; i < table.size(); i++) {
		while (table[i] != nullptr) {
			Node* tempNode = table[i];
			table[i] = table[i]->Next;
			delete tempNode;
		}
	}
}

template<class T, class H>
bool HashTable<T, H>::Has(const T& key) const
{
	int hash = hasher(key) % table.size();
	Node* node = table[hash];
	while (node != nullptr) {
		if (node->Key == key)
			break;
		node = node->Next;
	}

	return node != nullptr;
}

template<class T, class H>
bool HashTable<T, H>::Add(const T& key)
{
	if (keysCount > table.size() * 3)
		growTable();

	unsigned int hash = hasher(key) % table.size();
	Node* node = table[hash];
	while (node != nullptr) {
		if (node->Key == key)
			return false;
		node = node->Next;
	}
	table[hash] = new Node(key, table[hash]);
	++keysCount;
	return true;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
	vector<Node*> newTable(table.size() * 2, nullptr);

	for (int i = 0; i < table.size(); i++) {
		while (table[i] != nullptr) {
			Node* tempNode = table[i];
			table[i] = table[i]->Next;
			unsigned int hash = hasher(tempNode->Key) % newTable.size();
			tempNode->Next = newTable[hash];
			newTable[hash] = tempNode;
		}
	}

	table = std::move(newTable);
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T& key)
{
	unsigned int hash = hasher(key) % table.size();
	Node* node = table[hash];
	Node* prevNode = nullptr;
	while (node != nullptr) {
		if (node->Key == key)
			break;
		prevNode = node;
		node = node->Next;
	}

	if (node == nullptr) {
		return false;
	}

	if (prevNode == nullptr) {
		table[hash] = node->Next;
	}
	else {
		prevNode->Next = node->Next;
	}
	delete node;
	--keysCount;
	return true;
}

int main()
{
	Hasher hasher;
	HashTable<string, Hasher> table(hasher);

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
