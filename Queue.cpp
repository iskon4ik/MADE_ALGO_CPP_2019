#include <assert.h>
#include <iostream>

class Queue {
public:
	Queue();
	~Queue();

	void Enqueue(int data);
	int Dequeue();

private:
	struct Node {
		Node* Next;
		int Data;
		Node() : Next(nullptr), Data(0) {}
	};
	Node* head;
	Node* tail;
};

Queue::Queue() :
	head(nullptr),
	tail(nullptr)
{
}

Queue::~Queue()
{
	while( head != nullptr ) {
		Node* nextNode = head->Next;
		delete head;
		head = nextNode;
	}
}

void Queue::Enqueue(int data)
{
	Node* newNode = new Node;
	newNode->Data = data;
	if( tail == nullptr ) {
		assert( head == nullptr );
		head = newNode;
		tail = newNode;
	} else {
		tail->Next = newNode;
		tail = newNode;
	}
}

int Queue::Dequeue()
{
	assert( head != nullptr );

	int tempData = head->Data;
	Node* tempNode = head;
	head = head->Next;
	if( head == nullptr ) {
		tail = nullptr;
	}
	delete tempNode;
	return tempData;
}

int main()
{
	Queue q;
	q.Enqueue(123);
	q.Enqueue(123);
	std::cout << q.Dequeue();

	return 0;
}