#ifndef SEM3LAB4_DIRECTEDGRAPH_H
#define SEM3LAB4_DIRECTEDGRAPH_H
#include "Graph.h"
template <typename T>
class DirectedGraph : public Graph<T> {
public:
    void addEdge(const T& from, const T& to, double weight) override {
        if (!this->vertexAccess.containsKey(from) || !this->vertexAccess.containsKey(to)) {
            throw std::invalid_argument("One or both vertices not found");
        }

        typename Graph<T>::Vertex* fromVertex = this->vertexAccess.get(from);
        typename Graph<T>::Vertex* toVertex = this->vertexAccess.get(to);
        fromVertex->neighbors.append(Pair<typename Graph<T>::Vertex*, double>(toVertex, weight));
    }

    void removeEdge(const T& from, const T& to) override {
        if (!this->vertexAccess.containsKey(from) || !this->vertexAccess.containsKey(to)) {
            throw std::invalid_argument("One or both vertices not found");
        }

        typename Graph<T>::Vertex* fromVertex = this->vertexAccess.get(from);

        for (int i = 0; i < fromVertex->neighbors.getLength(); ++i) {
            if (fromVertex->neighbors[i].first == this->vertexAccess.get(to)) {
                fromVertex->neighbors.delByIndex(i);
                return;
            }
        }
    }
};

#endif //SEM3LAB4_DIRECTEDGRAPH_H
