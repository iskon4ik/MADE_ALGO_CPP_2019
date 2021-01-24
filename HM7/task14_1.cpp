/*
 Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.

 Ввод
4 4
1 2 1
2 3 2
3 4 5
4 1 4
Вывод
7

Ввод
5 10
4 3 3046
4 5 90110
5 1 57786
3 2 28280
4 3 18010
4 5 61367
4 1 18811
4 2 69898
3 5 72518
3 1 85838
Вывод
107923
 */



#include <iostream>
#include <vector>
#include <set>

class Graph {
private:
    std::vector< std::vector< std::pair<int, int> > > edges;
    int n;
public:
    Graph(int n);

    void AddEdge(int a, int b, int w);
    int FindMinPath();
};

Graph::Graph(int size) {
    n = size;
    edges = std::vector< std::vector< std::pair<int, int> > >();

    for(int i=0; i<n; ++i) {
        edges.emplace_back();
    }
}

void Graph::AddEdge(int a, int b, int w) {
    edges[a - 1].push_back(std::make_pair(b - 1, w));
    edges[b - 1].push_back(std::make_pair(a - 1, w));
}

int Graph::FindMinPath() {
    std::set<int> taken_nodes;

    const int INF = 1000000000;
    int min_path = 0;
    std::vector<int> min_edges (n, INF);
    min_edges[0] = 0;
    std::set < std::pair<int,int> > queue_to_add;
    queue_to_add.insert (std::make_pair (0, 0));
    for (int i=0; i<n; i++) {
        int node = queue_to_add.begin()->second;
        min_path += queue_to_add.begin()->first;


        queue_to_add.erase (queue_to_add.begin());

        taken_nodes.insert(node);

        for (size_t j=0; j < edges[node].size(); ++j) {
            int to = edges[node][j].first,
                    cost = edges[node][j].second;
            if (cost < min_edges[to] && !(taken_nodes.find(to) != taken_nodes.end())) {
                queue_to_add.erase (std::make_pair (min_edges[to], to));
                min_edges[to] = cost;
//                sel_e[to] = v;
                queue_to_add.insert (std::make_pair (min_edges[to], to));
            }
        }
    }

    return min_path;
}

int main() {

    int n, k;

    std::cin >> n >> k;

    if(n==0 || k==0){
        std::cout<<0;
        return 0;
    }

    Graph MyGraph = Graph(n);

    for( int i=0; i<k; ++i){
        int a, b, w;
        std::cin >> a >> b >> w;

        MyGraph.AddEdge(a, b, w);
    }

    std::cout<<MyGraph.FindMinPath();

    return 0;
}