#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

struct Edge {
    Edge (short pTo, short pWeight=0, short pCapacity=0) : to{pTo}, weight{pWeight}, capacity{pCapacity} { }
    short to;
    short weight;
    short capacity;
};

struct Vertex {
    Vertex (Edge* pAdjacentEdges = nullptr, short pNumEdges = 0) : adjacentEdges {pAdjacentEdges}, numEdges {pNumEdges} { }

    //Array of adjacentEdges
    Edge* adjacentEdges;
    short numEdges;
    short index;
    
    //Algorithm based variables
    short BFScolor = 0; //0 is white, 1 is gray, 2 is black but this is unnecessary
    short BFSdistance = __SHRT_MAX__;
    Vertex* BFSparent = nullptr;

    short DFScolor = 0;
    short DFStimeDiscovered = __SHRT_MAX__;
    short DFStimeFinished = __SHRT_MAX__;
    Vertex* DFSparent = nullptr;
};

struct Graph {
    //Adjacency List Implementation
    Graph (Vertex* pVertices, short pNumVertices) : vertices{pVertices}, numVertices{pNumVertices} {
        for(short i = 0; i < numVertices; i++) {
            vertices[i].index = i;
        }
    }

    Vertex* vertices;
    short numVertices;
};

//In-place operation
void BreadthFirstSearch(Graph &pGraph, Vertex &pStartVertex) {
    
    std::queue<Vertex*> BFSqueue;

    //Inititalization
    for (short i = 0; i < pGraph.numVertices; i++) {
        pGraph.vertices[i].BFScolor = 0;
        pGraph.vertices[i].BFSdistance = __SHRT_MAX__;
        pGraph.vertices[i].BFSparent = nullptr;
        //std::cout<<"alpha"<<std::endl;
    }
    
    pStartVertex.BFScolor = 1;
    pStartVertex.BFSdistance = 0;
    pStartVertex.BFSparent = nullptr;

    for (short i = 0; i < pGraph.numVertices; i++) {
        //std::cout<<"e"<<pGraph.vertices[i].index<<std::endl;
    }

    //Main operation
    BFSqueue.push(&pStartVertex);
    while (BFSqueue.size() != 0) {
        //std::cout<<"beta:"<<BFSqueue.front()->index<<std::endl;
        Vertex* u = BFSqueue.front();
        for(short i = 0; i < u->numEdges; i++) {
            //v is one of the vertices adjacent to u
            Vertex* v = &pGraph.vertices[u->adjacentEdges[i].to];
            if (v->BFScolor == 0) {
                v->BFScolor = 1;
                v->BFSdistance = u->BFSdistance + 1;
                v->BFSparent = u;
                //std::cout << v->BFSparent->index << std::endl;
                BFSqueue.push(v);
            }
        }
        u->BFScolor = 2; //This step is unnecessary but enhances visualization
        BFSqueue.pop();
    }
}

//If a pStartVertex is given that is not the same as the one used in BFS, this fails
void PrintBFSpath(Graph &pGraph, Vertex &pStartVertex, Vertex &pEndVertex) {
    if (pStartVertex.index == pEndVertex.index) {
        std::cout << pStartVertex.index;
    }
    else if (!pEndVertex.BFSparent) {
        std::cout << "No path exists from " << pStartVertex.index << " to " << pEndVertex.index << std::endl;
    }
    else {
        PrintBFSpath(pGraph, pStartVertex, *pEndVertex.BFSparent);
        std::cout << ">>" << pEndVertex.index;
    }
}

//Technically this only needs to be twice the size of a short
//Since the max value is 2 * number of vertices
//But this is hardly worth mentioning with the graph sizes I'm using
int DFStime = 0; 

//Recursive
void DepthFirstSearchVisit(Graph &pGraph, Vertex &u) {
    DFStime += 1;
    u.DFStimeDiscovered = DFStime;
    u.DFScolor = 1;
    for(short i = 0; i < u.numEdges; i++) {
        Vertex& v = pGraph.vertices[u.adjacentEdges[i].to];
        if (v.DFScolor == 0) {
            v.DFSparent = &u;
            DepthFirstSearchVisit(pGraph, v);
        }
    }
    u.DFScolor = 2;
    DFStime += 1;
    u.DFStimeFinished = DFStime;
}

//Depth first search covers the entire graph, 
//resulting in a forest rather than a tree.
void DepthFirstSearch(Graph &pGraph) {
    //Initialization
    DFStime = 0;
    for(short i = 0; i < pGraph.numVertices; i++) {
        pGraph.vertices[i].DFScolor = 0;
        pGraph.vertices[i].DFSparent = nullptr;
    }

    //Main Operation
    for(short i = 0; i < pGraph.numVertices; i++) {
        if (pGraph.vertices[i].DFScolor == 0) {
            //Searches every vertex in the graph
            DepthFirstSearchVisit(pGraph, pGraph.vertices[i]);
        }
    }
}

struct VertexList {
    Vertex* pointer;
    short length;
};

struct ComponentList {
    VertexList* pointer;
    short length;
};

//Necessary for StronglyConnectedComponents algorithm
Graph TransposeGraph(const Graph &pGraph) {
    std::vector<std::vector<Edge>> dynamicGraph(pGraph.numVertices);
    //std::cout << "size" << std::endl;
    //std::cout << "seez" << pGraph.vertices[0].numEdges << std::endl;;
    for (short i = 0; i < dynamicGraph.size(); i++) {
        //std::cout << "stts" << pGraph.vertices[i].numEdges << std::endl;;
        for (short j = 0; j < pGraph.vertices[i].numEdges; j++) {
            //std::cout << "sggs" << pGraph.vertices[i].numEdges << std::endl;;
            //Suppose there is an edge in vertices[i] pointing to vertices[z]
            //This line adds an edge in vertices[z] pointing to vertices[i]
            //Weight and capacity are neglected because this is used for StronglyConnectedComponents
            dynamicGraph[pGraph.vertices[i].adjacentEdges[j].to].push_back(Edge{i});
        }
    }
    //std::cout << "stts" << dynamicGraph[4].data()[0].to << std::endl;;
    //Stuck here and Im working on a segmentation fault.

    //std::cout<<dynamicGraph[5][0].to<<std::endl;

    Vertex vertexArray[pGraph.numVertices];
    //int* arr = vec.data();

    for (short i = 0; i < pGraph.numVertices; i++) {
        //std::cout<<i<<std::endl;
        transform(dynamicGraph[i].begin(), dynamicGraph[i].end(), vertexArray[i].adjacentEdges,[](const Edge elem){return elem;});
        vertexArray[i].index = i;
        //std::cout<<vertexArray[i].index<<std::endl;
        vertexArray[i].adjacentEdges = dynamicGraph[i].data();
        vertexArray[i].numEdges = dynamicGraph[i].size();
        //std::cout<<vertexArray[i].index<<std::endl;
    }
    //std::cout<<vertexArray[5].adjacentEdges[0].to<<std::endl;
    //std::cout<<vertexArray[8].numEdges<<std::endl;
    Graph transposedGraph = Graph{vertexArray, pGraph.numVertices};
    
    //std::cout<<transposedGraph.vertices[5].adjacentEdges[0].to<<std::endl;

    //std::cout<<"hoy"<<transposedGraph.vertices[4].index<<std::endl;

    return transposedGraph;
}

/*
ComponentList StronglyConnectedComponents(Graph pGraph) {
    DepthFirstSearch(pGraph);
    Graph pGraphTranspose = TransposeGraph(pGraph);
    DepthFirstSearch(pGraphTranspose);
    
}
*/

int main() {
    //TODO: Clean this up by reading from a file or something
    //      Create a program to create graphs with
    Edge Edges0[1] = {Edge{1, 0, 100}};
    Edge Edges1[2] = {Edge{2, 1, 5}, Edge{7, 2, 10}};
    Edge Edges2[2] = {Edge{6, 4, 3}, Edge{3, 8, 10}};
    Edge Edges3[2] = {Edge{4, 16, 7}, Edge{5, 32, 10}};
    //No edges4
    Edge Edges5[1] = {Edge{4, 64, 6}};
    Edge Edges6[1] = {Edge{7, 128, 3}};
    Edge Edges7[1] = {Edge{2, 256, 10}};
    //No edges8

    Vertex vertices[9] = {
        Vertex{Edges0, 1}, 
        Vertex{Edges1, 2}, 
        Vertex{Edges2, 2}, 
        Vertex{Edges3, 2}, 
        Vertex{}, 
        Vertex{Edges5, 1}, 
        Vertex{Edges6, 1}, 
        Vertex{Edges7, 1}, 
        Vertex{}
    };

    Graph graph{vertices, 9};
    /*
    BreadthFirstSearch(graph, graph.vertices[0]);

    PrintBFSpath(graph, graph.vertices[0], graph.vertices[5]);

    DepthFirstSearch(graph);

    //Testing everything
    for (short i = 0; i < graph.numVertices; i++) {
        Vertex* parent = graph.vertices[i].BFSparent;
        if (parent) {
            std::cout << graph.vertices[i].index << ":" << parent->index<< std::endl;
        }
        parent = graph.vertices[i].DFSparent;
        if (parent) {
            std::cout << graph.vertices[i].index << ":" << parent->index<< std::endl;
        }
    }
*/
    Graph tGraph = TransposeGraph(graph);
    std::cout<<tGraph.vertices[4].index<<std::endl;
    std::cout<<tGraph.vertices[4].index<<std::endl;
    std::cout<<tGraph.vertices[4].index<<std::endl;
    /*for (short i = 0; i < tGraph.numVertices; i++) {
        std::cout<<tGraph.vertices[4].index<<std::endl;
        std::cout<<"o"<<tGraph.vertices[4].index<<std::endl;
    }*/
    BreadthFirstSearch(tGraph, tGraph.vertices[4]);
    //std::cout<<tGraph.vertices[8].numEdges<<std::endl;

    //PrintBFSpath(tGraph, tGraph.vertices[5], tGraph.vertices[0]);

    //DepthFirstSearch(tGraph);

    //Testing everything
    for (short i = 0; i < tGraph.numVertices; i++) {
        Vertex* parent = tGraph.vertices[i].BFSparent;
        if (parent) {
            std::cout << tGraph.vertices[i].index << "bfs:" << parent->index<< std::endl;
        }
        parent = tGraph.vertices[i].DFSparent;
        if (parent) {
            std::cout << tGraph.vertices[i].index << "dfs:" << parent->index<< std::endl;
        }
    }
}