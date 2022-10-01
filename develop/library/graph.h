#include <string>
#include <vector>
#include <unordered_map>

#include "systemOS.h"

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
    void addEdge(EDGE const &newEdge);

    int cost, vertexNum;
    std::vector<EDGE> edges;
    std::unordered_map<std::string, bool> vertices;
};

GRAPH MST_P(std::vector<EDGE> const &links);
GRAPH MST_K(std::vector<EDGE> const &links);

#endif