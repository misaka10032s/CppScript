#include <cstring>
#include <vector>
#include <map>

#ifndef LIB_GRAPH_H_
#define LIB_GRAPH_H_

struct EDGE {
    EDGE(std::string fromV, std::string toV, int weight);
    EDGE();

    std::string fromV;
    std::string toV;
    int weight;
};

struct GRAPH {
    GRAPH();

    void addEdge(std::string P0, std::string P1, int W);
    void addEdge(EDGE newEdge);

    int cost;
    std::vector<EDGE> edges;
};

GRAPH MST_P(std::vector<EDGE> links);
GRAPH MST_K(std::vector<EDGE> links);

#endif