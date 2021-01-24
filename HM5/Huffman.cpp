#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

class Node
{
	public:
	 int a;
	 char c;
	 Node *left, *right;
	 
	 Node(){left=right=NULL;}

	 Node(Node *L, Node *R) 
	 {  left =  L;
	    right = R;
	    a = L->a + R->a;  }
};


struct MyCompare
{
    bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};


vector<bool> code;                
map<char,vector<bool> > table;    

void BuildTable(Node *root)
{	
    if (root->left!=NULL) 
                      { code.push_back(0);
                      BuildTable(root->left);}
     
    if (root->right!=NULL)
                       { code.push_back(1);
                       BuildTable(root->right);}
   
    if (root->left==NULL && root->right==NULL) table[root->c]=code;     
    
    code.pop_back();
}


int main (int argc, char *argv[])
{
////// ������� ������� ��������	
	ifstream f("1.txt", ios::out | ios::binary);
	
	map<char,int> m;
	
	while (!f.eof())
	{ char c = f.get(); 
	   m[c]++;}

  
////// ���������� ��������� ���� � ������ list
	     	     
   list<Node*> t;
   for( map<char,int>::iterator itr=m.begin(); itr!=m.end(); ++itr)
   {  
      Node *p = new Node;
      p->c = itr->first;
      p->a = itr->second;  
      t.push_back(p);      }	
	

//////  ������� ������		

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
	
	Node *root = t.front();   //root - ��������� �� ������� ������

////// ������� ���� '������-���':			

   	BuildTable(root);   
		
////// ������� ���� � ���� output.txt

    f.clear(); f.seekg(0); // ���������� ��������� ����� � ������ �����
    
	ofstream g("output.txt", ios::out | ios::binary);
    	
    int count=0; char buf=0;
    while (!f.eof())
    { char c = f.get();
	  vector<bool> x = table[c];
	  for(int n=0; n<x.size(); n++)
	   {buf = buf | x[n]<<(7-count);   
	    count++;   
	    if (count==8) { count=0;   g<<buf; buf=0; } 
       }
    }

    f.close();
	g.close(); 
	
///// ���������� �� ����� output.txt � �������������� �������

	ifstream F("output.txt", ios::in | ios::binary);

	setlocale(LC_ALL,"Russian"); // ���� ������� ������� ������������ � ��������� ������
	
	Node *p = root;
	count=0; char byte; 
	byte = F.get();
	while(!F.eof())
	{   bool b = byte & (1 << (7-count) ) ; 
		if (b) p=p->right; else p=p->left;
		if (p->left==NULL && p->right==NULL) {cout<<p->c; p=root;}  
		count++;
		if (count==8) {count=0; byte = F.get();}
	}
	
	F.close();	

	return 0;
}