//  Задача 6.3.
//Дано число N ≤ 10^4 и последовательность целых чисел из [-2^31..2^31] длиной N. Требуется построить бинарное дерево,
// заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root. Выведите элементы в порядке post-order (снизу вверх).
//
//Рекурсия запрещена.
/*
in
10
7
2
10
8
5
3
6
4
1
9
out
1 4 3 6 5 2 9 8 10 7

in
10
7
10
2
6
1
9
4
5
8
3
out
1 3 5 4 6 2 8 9 10 7

in
10
4
3
9
5
6
8
7
2
10
1
out
1 2 3 7 8 6 5 10 9 4
*/

#include <iostream>
#include <stack>

//template <class T>
//class IsLessDefaultFunctor {
//public:
//    bool operator()(const T& l, const T& r) {
//        return l < r;
//    }
//};

template <class T>
struct TreeNode {
//    explicit TreeNode(T _value): value(_value) {}
//    T value = 0;
//    TreeNode<T>* left = nullptr;
//    TreeNode<T>* right = nullptr;
    T value;
    TreeNode<T> * left, * right;
};

template <class T>
TreeNode<T> * newNode(int value){
    auto * node = new TreeNode<T>;
    node->value = value;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}
//template <class T>
//TreeNode<T>::TreeNode(T _value) {
//
//}

//template <class T>
//struct TreeNode {
//    T value ;
//    TreeNode<T>* left = nullptr;
//    TreeNode<T>* right = nullptr;
//};
//
//template <class T>
//struct TreeNode<T>* NewNode(T value){
//    struct TreeNode<T> * temp = new TreeNode<T>;
//    temp->value = value;
//    temp->left = nullptr;
//    temp->right = nullptr;
//};
//TreeNode(T _value) : value(_value) {
//    left = null
//}

//template <class T, class IsLess=IsLessDefaultFunctor<T> >
template <class T>
class BinaryTree {
public:
    ~BinaryTree();

    void Add(T value);
    void PostOrderPrint();

private:
    TreeNode<T>* root = nullptr;

//    void print_subtree(TreeNode<T>* node) const;
    void delete_subtree(TreeNode<T>* node);
};

template <class T>
BinaryTree<T>::~BinaryTree() {
    delete_subtree(root);
}

template <class T>
void BinaryTree<T>::delete_subtree(TreeNode<T> *node) {
    if (!node) return;
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}

template <class T>
void BinaryTree<T>::PostOrderPrint() {
    TreeNode<T> * node = root;
    std::stack<T> s;
    while(node) {
        if(!node->right) {
            s.push(node->value);
            node = node->left;
        }
        else {
            TreeNode<T>* prev = node->right;

            while(prev->left && prev->left != node)
                prev = prev->left;

            if(!prev->left) {
                prev->left = node;
                s.push(node->value);
                node = node->right;
            }
            else {
                node = node->left;
                prev->left = nullptr;
            }
        }
    }

    while(!s.empty()) {
        std::cout << s.top() << " ";
        s.pop();
    }
}

template <class T>
void BinaryTree<T>::Add(T value) {
    if (!root) {
//        root = new TreeNode<T>(value);
        root = newNode<T>(value);
    }
    else {
        TreeNode <T>* curr = root;

        while((value<curr->value && curr->left) || (value>curr->value && curr->right)){
            if (value < curr->value) {
                curr = curr->left;
            }
            else {
                curr =curr->right;
            }
        }

        if(value < curr->value) {
            curr->left = newNode<T>(value);
        }
        else {
            curr->right = newNode<T>(value);
        }
//        root->left = newNode<T>(value);
    }
}

//template <class T>
//void BinaryTree<T>::print_subtree(TreeNode<T> *node) const {
//    if (!node) return;
//    print_subtree(node->left);
//    print_subtree(node->right);
//    std::cout << node->value << " ";
//}

int main() {
    int n;
    std::cin>>n;
    int temp;
    BinaryTree<int> My_tree;
    for (int i=0; i<n; i++) {
        std::cin>>temp;
        My_tree.Add(temp);
    }

    My_tree.PostOrderPrint();
    return 0;
}