#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;

#include "systemOS.h"
#include "heap.h"
#include "graph.h"


// ######################## struct EDGE ########################
EDGE::EDGE(){
    this->fromV = "";
    this->toV = "";
    this->weight = 0;
    this->directional = 0;
}
EDGE::EDGE(std::string fromV, std::string toV, int weight){
    this->fromV = fromV;
    this->toV = toV;
    this->weight = weight;
    this->directional = 0;
}
EDGE::EDGE(std::string fromV, std::string toV, int weight, bool directional){
    this->fromV = fromV;
    this->toV = toV;
    this->weight = weight;
    this->directional = directional;
}

bool EDGE::operator == (EDGE const &EDGE1) const{
    if(this->weight != EDGE1.weight) return 0;
    if(this->directional){
        if(this->fromV != EDGE1.fromV) return 0;
        if(this->toV != EDGE1.toV) return 0;
    }
    else{
        if(this->fromV != EDGE1.fromV && this->fromV != EDGE1.toV) return 0;
        if(this->toV != EDGE1.fromV && this->toV != EDGE1.toV) return 0;
    }
    return 1;
};
bool EDGE::operator != (EDGE const &EDGE1) const{
    return !(*this == EDGE1);
};

// ######################## struct GRAPH ########################
GRAPH::GRAPH(){
    this->cost = 0;
    this->vertexNum = 0;
}

void GRAPH::addEdge(std::string P0, std::string P1, int W){
    this->edges.push_back(EDGE(P0, P1, W));
    this->cost += W;
    if(this->vertices.find(P0) == this->vertices.end()){
        this->vertices.emplace(P0, 1);
        this->vertexNum++;
    }
    if(this->vertices.find(P1) == this->vertices.end()){
        this->vertices.emplace(P1, 1);
        this->vertexNum++;
    }
}

void GRAPH::addEdge(EDGE const &newEdge){
    this->edges.push_back(newEdge);
    this->cost += newEdge.weight;
    if(this->vertices.find(newEdge.fromV) == this->vertices.end()){
        this->vertices.emplace(newEdge.fromV, 1);
        this->vertexNum++;
    }
    if(this->vertices.find(newEdge.toV) == this->vertices.end()){
        this->vertices.emplace(newEdge.toV, 1);
        this->vertexNum++;
    }
}

// ######################## MST Prim's algorithm ########################
GRAPH MST_P(std::vector<EDGE> const &links){
    int vertexNum = 0, edgeNum = links.size();
    std::unordered_map<std::string, std::string> vertexLinked;
    std::unordered_map<std::string, int> vertexMinWeight;
    std::unordered_map<std::string, bool> vertexIsVisited;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> vertexTo;
    std::string nowVertex;
    GRAPH forest;
    HEAP minHEAP(0);
    bool isNewNodeFrom, isNewNodeTo;

    for(int i=0; i<edgeNum; i++){
        isNewNodeFrom = isNewNodeTo = 0;
        if(vertexTo.find(links[i].fromV) == vertexTo.end()){
            vertexTo[links[i].fromV].emplace(links[i].toV, links[i].weight);
            isNewNodeFrom = 1;
        }
        else{
            if(vertexTo[links[i].fromV].find(links[i].toV) == vertexTo[links[i].fromV].end()){
                vertexTo[links[i].fromV].emplace(links[i].toV, links[i].weight);
            }
            else{
                vertexTo[links[i].fromV].emplace(links[i].toV, std::min(vertexTo[links[i].fromV][links[i].toV], links[i].weight));
            }
        }

        if(vertexTo.find(links[i].toV) == vertexTo.end()){
            vertexTo[links[i].toV].emplace(links[i].fromV, links[i].weight);
            isNewNodeTo = 1;
        }
        else{
            if(vertexTo[links[i].toV].find(links[i].fromV) == vertexTo[links[i].toV].end()){
                vertexTo[links[i].toV].emplace(links[i].fromV, links[i].weight);
            }
            else{
                vertexTo[links[i].toV].emplace(links[i].fromV, std::min(vertexTo[links[i].toV][links[i].fromV], links[i].weight));
            }
        }

        if(isNewNodeFrom){
            vertexNum++;
            vertexLinked.emplace(links[i].fromV, "");
            vertexMinWeight.emplace(links[i].fromV, 2147483647);
            vertexIsVisited.emplace(links[i].fromV, 0);
        }
        if(isNewNodeTo){
            vertexNum++;
            vertexLinked.emplace(links[i].toV, "");
            vertexMinWeight.emplace(links[i].toV, 2147483647);
            vertexIsVisited.emplace(links[i].toV, 0);
        }
    }

    // 18446744073709551616
    nowVertex = links.cbegin()->fromV;
    vertexMinWeight[nowVertex] = -2147483648;
    vertexIsVisited[nowVertex] = 1;

    int minV, vertexNumAdded = 0;
    std::string minDest, minOrig;
    NODE tmpEdge;

    while(vertexNumAdded < vertexNum - 1){
        for(auto [v,w]:vertexTo[nowVertex]){
            if(vertexIsVisited[v]) continue;

            if(w < vertexMinWeight[v]){
                minHEAP.push(v, w);
                vertexMinWeight[v] = w;
            }
        }

        do{
            tmpEdge = minHEAP.pop();
            if(tmpEdge.key == ""){
                for(auto [key, visited] : vertexIsVisited){
                    if(!visited){
                        tmpEdge.key = key;
                        break;
                    }
                }
            }
        }
        while(vertexIsVisited[tmpEdge.key]);

        minDest = tmpEdge.key;
        minOrig = vertexLinked[tmpEdge.key];
        minV = vertexMinWeight[tmpEdge.key];

        vertexIsVisited[tmpEdge.key] = 1;
        forest.addEdge(minOrig, minDest, minV);
        vertexNumAdded++;
        nowVertex = minDest;
    }

    // free(minHEAP);
    return forest;
};


// ######################## MST Kruskal's algorithm ########################
GRAPH MST_K(std::vector<EDGE> const &links){
    int vertexNum = 0, groupKeyTmp;
    std::unordered_map<std::string, int> vertexBelong;
    std::unordered_map<int, std::vector<std::string>> vertexGroup;
    std::vector<std::string> linksSorted;
    GRAPH forest;
    HEAP minHEAP(0);
    EDGE checkingEdge;

    for(auto l : links){
        if(vertexBelong.find(l.fromV) == vertexBelong.end()){
            vertexBelong.emplace(l.fromV, -1);
            vertexNum++;
        }
        if(vertexBelong.find(l.toV) == vertexBelong.end()){
            vertexBelong.emplace(l.toV, -1);
            vertexNum++;
        }
    }

    for(int i=0; i<(int)links.size(); i++) minHEAP.push(std::to_string(i), links[i].weight);

    while((int)forest.edges.size() < vertexNum){
        checkingEdge = links[std::stoi(minHEAP.pop().key)];

        if(checkingEdge.fromV == checkingEdge.toV) continue;

        if(vertexBelong[checkingEdge.fromV] == -1 && vertexBelong[checkingEdge.toV] == -1){
            for(int i=0; i<vertexNum; i++){
                if(vertexGroup.find(i) == vertexGroup.end()){
                    groupKeyTmp = i;
                    break;
                }
            }
            vertexBelong[checkingEdge.fromV] = vertexBelong[checkingEdge.fromV] = groupKeyTmp;
            vertexGroup.emplace(groupKeyTmp, std::initializer_list<std::string>{checkingEdge.fromV, checkingEdge.toV});
            forest.addEdge(checkingEdge);
        }
        else if(vertexBelong[checkingEdge.fromV] == -1 && vertexBelong[checkingEdge.toV] != -1){
            vertexBelong[checkingEdge.fromV] = vertexBelong[checkingEdge.toV];
            vertexGroup[vertexBelong[checkingEdge.toV]].push_back(checkingEdge.fromV);
            forest.addEdge(checkingEdge);
        }
        else if(vertexBelong[checkingEdge.fromV] != -1 && vertexBelong[checkingEdge.toV] == -1){
            vertexBelong[checkingEdge.toV] = vertexBelong[checkingEdge.fromV];
            vertexGroup[vertexBelong[checkingEdge.fromV]].push_back(checkingEdge.toV);
            forest.addEdge(checkingEdge);
        }
        else if(vertexBelong[checkingEdge.fromV] != vertexBelong[checkingEdge.toV]){
            groupKeyTmp = vertexBelong[checkingEdge.fromV];
            for(int i=0; i<(int)vertexGroup[groupKeyTmp].size(); i++){
                vertexBelong[vertexGroup[groupKeyTmp][i]] = vertexBelong[checkingEdge.toV];
                vertexGroup[vertexBelong[checkingEdge.toV]].push_back(vertexGroup[groupKeyTmp][i]);
            }
            vertexGroup.erase(groupKeyTmp);

            forest.addEdge(checkingEdge);
        }
    }

    // free(minHEAP);
    return forest;
}

// ######################## find shortest route between two points ########################
namespace std {
    template <>
    struct hash<EDGE>{
        std::size_t operator()(const EDGE& k) const
        {
        using std::size_t;
        using std::hash;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return ((hash<string>()(k.fromV)
                ^ (hash<string>()(k.toV) << 1)) >> 1)
                ^ (hash<int>()(k.weight) << 1);
        }
    };
}

void rfind(std::string start, std::string end, std::unordered_map<std::string, std::vector<EDGE>> &rtdict, std::vector<EDGE> &rt, std::vector<EDGE> &route, std::unordered_map<std::string, int> &pointV, std::unordered_map<EDGE, int> &hasGoThrough){
    std::vector<EDGE> possible = rtdict[start];
    if(possible.size() == 0){
        if(start != end) {
            hasGoThrough.erase(rt.back());
            rt.pop_back();
        }
        return;
    }
    for(auto edge : possible){
        if(hasGoThrough.find(edge) != hasGoThrough.end() || pointV[edge.fromV] + edge.weight > pointV[end]) continue;
        rt.push_back(edge);
        hasGoThrough.emplace(edge, 1);
        if(pointV[edge.toV] > pointV[edge.fromV] + edge.weight || pointV[edge.toV] == -2147483648){
            pointV[edge.toV] = pointV[edge.fromV] + edge.weight;
            if(edge.toV == end) {
                route = rt;
            }
        }
        rfind(edge.toV, end, rtdict, rt, route, pointV, hasGoThrough);
    }
};

GRAPH shortestRoute(std::vector<EDGE> const &links, std::string start, std::string end){
    GRAPH resRoute;
    std::unordered_map<std::string, int> pointV;
    std::unordered_map<EDGE, int> hasGoThrough;
    std::unordered_map<std::string, std::vector<EDGE>> rtdict;
    std::vector<EDGE> rt, route;

    for(auto l : links){
        pointV.emplace(l.fromV, -2147483648);
        pointV.emplace(l.toV, -2147483648);

        if(rtdict.find(l.fromV) == rtdict.end()) rtdict.emplace(l.fromV, std::vector<EDGE>({l}));
        else rtdict[l.fromV].push_back(l);

        if(!l.directional){
            if(rtdict.find(l.toV) == rtdict.end()) rtdict.emplace(l.toV, std::vector<EDGE>({l}));
            else rtdict[l.toV].push_back(l);
        }
    }

    rfind(start, end, rtdict, rt, route, pointV, hasGoThrough);

    for(int i=0; i<(int)route.size(); i++){
        resRoute.addEdge(route[i]);
    }
    return resRoute;
};