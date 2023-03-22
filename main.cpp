using namespace std;
#include <vector>
#include <queue>
#include <iostream>
#include <set>
#include <algorithm>
#include <string>

struct Edge {
    Edge (int pFrom, int pTo, int pWeight=0, int pCapacity=0) : from{pFrom}, to{pTo}, weight{pWeight}, capacity{pCapacity} { }

    string toString() {
        return "(" + to_string(from) + "," + to_string(to) + ")";
    }

    int from;
    int to;
    int weight;
    int capacity;
};

struct Vertex {
    Vertex(int pIndex) : index{pIndex} {}
    vector<Edge> edges;
    const int index;

    //Algorithm based variables
    int BFScolor = 0; //0 is white, 1 is gray, 2 is black but this is unnecessary
    int BFSdistance = __SHRT_MAX__;
    Vertex* BFSparent = nullptr;

    int DFScolor = 0;
    int DFStimeDiscovered = __SHRT_MAX__;
    int DFStimeFinished = __SHRT_MAX__;
    Vertex* DFSparent = nullptr;

    int KruskralSet = 0;
};

struct Graph {
    vector<Vertex> vertices;
    vector<Edge> edges;
    void AddVertex() {
        vertices.push_back(Vertex{vertices.size()});
    }
    void AddEdge(int pSrc, int pDest, int pWeight = 0, int pCapacity = 0) {
        vertices[pSrc].edges.push_back(Edge{pSrc, pDest, pWeight, pCapacity});
        //Not sure if this following line causes each edge to be stored twice or not
        //Can use pointers in vertices to their edges in the big graph edge list to fix this
        edges.push_back(vertices[pSrc].edges[vertices[pSrc].edges.size()-1]);
    }
    /*
    void ChangeKruskralSet(int pWhichVertex, int pNewSetNum) { //1, 0
        //If the vertex's set is not its own index
        if (pWhichVertex != vertices[pWhichVertex].KruskralSet) {
            ChangeKruskralSet(vertices[pWhichVertex].KruskralSet, pNewSetNum);
        }
        vertices[pWhichVertex].KruskralSet = pNewSetNum;
    }*/

    int FindKruskralSet(int pIndex) {
        if (pIndex == vertices[pIndex].KruskralSet) {
            return pIndex;
        }
        else {
            return FindKruskralSet(vertices[pIndex].KruskralSet);
        }
    }
};

//In-place operation
void BreadthFirstSearch(Graph &pGraph, Vertex &pStartVertex) {
    
    std::queue<Vertex*> BFSqueue;

    //Inititalization
    for (int i = 0; i < pGraph.vertices.size(); i++) {
        pGraph.vertices[i].BFScolor = 0;
        pGraph.vertices[i].BFSdistance = __SHRT_MAX__;
        pGraph.vertices[i].BFSparent = nullptr;
    }
    
    pStartVertex.BFScolor = 1;
    pStartVertex.BFSdistance = 0;
    pStartVertex.BFSparent = nullptr;

    //Main operation
    BFSqueue.push(&pStartVertex);
    while (BFSqueue.size() != 0) {
        Vertex* u = BFSqueue.front();
        for(int i = 0; i < u->edges.size(); i++) {
            //v is one of the vertices adjacent to u
            Vertex* v = &pGraph.vertices[u->edges[i].to];
            if (v->BFScolor == 0) {
                v->BFScolor = 1;
                v->BFSdistance = u->BFSdistance + 1;
                v->BFSparent = u;
                BFSqueue.push(v);
            }
        }
        u->BFScolor = 2; //This step is unnecessary but enhances visualization
        BFSqueue.pop();
    }
}

//If a pStartVertex is given that is not the same as the one used in BFS, this fails
void PrintBFSpath(const Graph &pGraph, const Vertex &pStartVertex, const Vertex &pEndVertex) {
    if (pStartVertex.index == pEndVertex.index) {
        cout << pStartVertex.index;
    }
    else if (!pEndVertex.BFSparent) {
        cout << "No path exists from " << pStartVertex.index << " to " << pEndVertex.index << std::endl;
    }
    else {
        PrintBFSpath(pGraph, pStartVertex, *pEndVertex.BFSparent);
        cout << ">>" << pEndVertex.index;
    }
}

//Takes graph pGraph and tranposes it into pTransposed
void TransposeGraph(const Graph &pGraph, Graph &pTransposed) {
    for(int i = 0; i < pGraph.vertices.size(); i++) {
        pTransposed.AddVertex();
    }

    for(int i = 0; i < pGraph.vertices.size(); i++)
        for(int j = 0; j < pGraph.vertices[i].edges.size(); j++)
            pTransposed.AddEdge(pGraph.vertices[i].edges[j].to, i);
            //For now, the weight and capacity are neglected for the transposed version
}

void StronglyConnectedComponents(const Graph pGraph) {
    //Incomplete
}

// Compares two vertices by index
// Required for using std::set
bool operator<(const Vertex& vertex, const Vertex& otherVertex) {
    return (vertex.index < otherVertex.index);
}

// Compares two edges by weight
bool operator<(const Edge& e1, const Edge& e2)
{
    return (e1.weight < e2.weight);
}

//Remember why a "contains" method is not included by default
//https://stackoverflow.com/questions/1701067/how-to-check-that-an-element-is-in-a-stdset#:~:text=51-,Just%20to%20clarify,-%2C%20the%20reason%20why
/*
template <typename T>
bool contained(set<T> pSet, T pItem) {
    return pSet.find(pItem) != pSet.end();
}
*/

//Temporarily stopped working on Kruskral because I realized the solution has to do with set ranking, which I am currently unfamiliar with

/*
Graph KruskalMST(Graph &pGraph) {
    //Finds the minimum-spanning-tree for an undirected graph
    vector<Edge> minimumSpanningTree;

    for(int i = 0; i < pGraph.vertices.size(); i++) {
        //Creates a separate tree for each vertex
        pGraph.vertices[i].KruskralSet = i;
    }

    vector<Edge> allEdges = pGraph.edges;
    for(int i = 0; i < allEdges.size(); i++) {
        //cout << allEdges[i].toString() << endl;
    }
    sort(allEdges.begin(), allEdges.end());
    //cout <<"he"<<endl;
    for(int i = 0; i < allEdges.size(); i++) {
        //cout << allEdges[i].toString() << endl;
    }

    std::cout<<"Edges ordered"<<std::endl;

    for(int i = 0; i < allEdges.size(); i++) {
        //If either vertex set doesnt contain the other
        //if (pGraph.vertices[allEdges[i].from].KruskralSet != pGraph.vertices[allEdges[i].to].KruskralSet) {
        cout << "Full log: " << allEdges[i].toString() << endl;
        if (pGraph.FindKruskralSet(allEdges[i].from) != pGraph.FindKruskralSet(allEdges[i].to)) {
            //cout <<"he "<<pGraph.vertices[allEdges[i].from].KruskralSet<<endl;
            //cout <<"ho "<<pGraph.vertices[allEdges[i].to].KruskralSet<<endl;
            cout << "Sets " << pGraph.FindKruskralSet(allEdges[i].from) << " ! " << pGraph.FindKruskralSet(allEdges[i].to) << endl;
            //Save the edge in the MST
            minimumSpanningTree.push_back(allEdges[i]);
            //cout << allEdges[i].toString() << endl;
            //Bring the "from" vertex set into the "to" vertex's tree
            //pGraph.ChangeKruskralSet(allEdges[i].from, pGraph.vertices[allEdges[i].to].KruskralSet);
            //pGraph.ChangeKruskralSet(allEdges[i].to, pGraph.vertices[allEdges[i].from].KruskralSet);
            pGraph.vertices[allEdges[i].to].KruskralSet = pGraph.FindKruskralSet(allEdges[i].from);
        }
    }

    cout << "hof" << endl;
    for(int i = 0; i < pGraph.vertices.size(); i++) {
        cout << i << " " << pGraph.vertices[i].KruskralSet << " ! " << pGraph.FindKruskralSet(i) << endl;
    }

    for (Edge e : minimumSpanningTree) {
        cout << e.toString() << endl;
    }


    return pGraph;
}*/

//Recursive
void DepthFirstSearchVisit(Graph &pGraph, Vertex &u, int pTime) {
    pTime += 1;
    u.DFStimeDiscovered = pTime;
    u.DFScolor = 1;
    for(int i = 0; i < u.edges.size(); i++) {
        Vertex& v = pGraph.vertices[u.edges[i].to];
        if (v.DFScolor == 0) {
            v.DFSparent = &u;
            DepthFirstSearchVisit(pGraph, v, pTime);
        }
    }
    u.DFScolor = 2;
    pTime += 1;
    u.DFStimeFinished = pTime;
}

//Depth first search covers the entire graph, 
//resulting in a forest rather than a tree.
void DepthFirstSearch(Graph &pGraph) {
    //Initialization
    for(int i = 0; i < pGraph.vertices.size(); i++) {
        pGraph.vertices[i].DFScolor = 0;
        pGraph.vertices[i].DFSparent = nullptr;
    }

    //Main Operation
    for(int i = 0; i < pGraph.vertices.size(); i++) {
        if (pGraph.vertices[i].DFScolor == 0) {
            //Searches every vertex in the graph
            DepthFirstSearchVisit(pGraph, pGraph.vertices[i], 0);
        }
    }
}

int main() {
    Graph graph;
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddVertex();
    graph.AddEdge(0, 1, 1, 100);
    graph.AddEdge(1, 2, 2, 5);
    graph.AddEdge(1, 7, 4, 10);
    graph.AddEdge(2, 6, 5, 3);
    graph.AddEdge(2, 3, 7, 10);
    graph.AddEdge(3, 4, 8, 7);
    graph.AddEdge(3, 5, 3, 10);
    graph.AddEdge(5, 4, 3, 6);
    graph.AddEdge(6, 7, 4, 3);
    graph.AddEdge(7, 2, 7, 10);

    BreadthFirstSearch(graph, graph.vertices[0]);
    DepthFirstSearch(graph);
    KruskalMST(graph);

    //Testing everything
    for (int i = 0; i < graph.vertices.size(); i++) {
        Vertex* parent = graph.vertices[i].BFSparent;
        if (parent) {
            //std::cout << graph.vertices[i].index << "bfs:" << parent->index<< std::endl;
        }
        parent = graph.vertices[i].DFSparent;
        if (parent) {
            //std::cout << graph.vertices[i].index << "dfs:" << parent->index<< std::endl;
        }
    }
    /*
    Graph graphT;
    TransposeGraph(graph, graphT);
    BreadthFirstSearch(graphT, graphT.vertices[4]);
    DepthFirstSearch(graphT);

    //Testing everything
    for (int i = 0; i < graphT.vertices.size(); i++) {
        Vertex* parent = graphT.vertices[i].BFSparent;
        if (parent) {
            std::cout << graphT.vertices[i].index << "bfs:" << parent->index<< std::endl;
        }
        parent = graphT.vertices[i].DFSparent;
        if (parent) {
            std::cout << graphT.vertices[i].index << "dfs:" << parent->index<< std::endl;
        }
    }
    */
}