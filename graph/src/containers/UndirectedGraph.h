#ifndef SEM3LAB4_UNDIRECTEDGRAPH_H
#define SEM3LAB4_UNDIRECTEDGRAPH_H
#include "Graph.h"
    template <typename T>
    class UndirectedGraph : public Graph<T> {
    public:
        void addEdge(const T& from, const T& to, double weight) override {
            if (!this->vertexAccess.containsKey(from) || !this->vertexAccess.containsKey(to)) {
                throw std::invalid_argument("One or both vertices not found");
            }

            typename Graph<T>::Vertex* fromVertex = this->vertexAccess.get(from);
            typename Graph<T>::Vertex* toVertex = this->vertexAccess.get(to);

            fromVertex->neighbors.append(Pair<typename Graph<T>::Vertex*, double>(toVertex, weight));
            toVertex->neighbors.append(Pair<typename Graph<T>::Vertex*, double>(fromVertex, weight));
        }

        void removeEdge(const T& from, const T& to) override {
            if (!this->vertexAccess.containsKey(from) || !this->vertexAccess.containsKey(to)) {
                throw std::invalid_argument("One or both vertices not found");
            }

            typename Graph<T>::Vertex* fromVertex = this->vertexAccess.get(from);
            typename Graph<T>::Vertex* toVertex = this->vertexAccess.get(to);

            auto& neighborsFrom = fromVertex->neighbors;
            auto& neighborsTo = toVertex->neighbors;

            for (int i = 0; i < neighborsFrom.getLength(); ++i) {
                if (neighborsFrom[i].first == toVertex) {
                    neighborsFrom.delByIndex(i);
                    break;
                }
            }

            for (int i = 0; i < neighborsTo.getLength(); ++i) {
                if (neighborsTo[i].first == fromVertex) {
                    neighborsTo.delByIndex(i);
                    break;
                }
            }
        }
    };

#endif //SEM3LAB4_UNDIRECTEDGRAPH_H
