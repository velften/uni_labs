#ifndef SEM3LAB4_GRAPH_H
#define SEM3LAB4_GRAPH_H

#include <iostream>
#include "../ptrs/ShrdPtr.h"
#include "DynamicArray.h"
#include "HashTable.h"
#include "Pair.h"
#include "Stack.h"
#include "Queue.h"
#include "PriorityQueue.h"

template <typename T>
class Graph {
protected:
    struct Vertex {
        T data;
        DynamicArray<Pair<Vertex*, double>> neighbors;

        explicit Vertex(const T& value) : data(value), neighbors() {}
    };
    DynamicArray<ShrdPtr<Vertex>> vertices;
    HashTable<T, Vertex*> vertexAccess;
    bool weighted{true};

public:
    Graph() = default;

    bool isWeighted() const {
        return weighted;
    }

    void addVertex(const T& value) {
        if (vertexAccess.containsKey(value)) {
            throw std::invalid_argument("Vertex already exists");
        }
        auto newVertex = ShrdPtr<Vertex>(new Vertex(value));
        Vertex* rawPtr = newVertex.get();
        vertexAccess.add(value, rawPtr);
        vertices.append(newVertex);
    }

    virtual void addEdge(const T& from, const T& to, double weight) = 0;

    void removeVertex(const T& value) {
        if (!vertexAccess.containsKey(value)) {
            throw std::invalid_argument("Vertex not found");
        }
        Vertex* vertex = vertexAccess.get(value);
        vertexAccess.remove(value);

        for (int i = 0; i < vertices.getLength(); ++i) {
            Vertex* v = vertices[i].get();
            auto& neighbors = v->neighbors;
            for (int j = 0; j < neighbors.getLength(); ++j) {
                if (neighbors[j].first == vertex) {
                    neighbors.delByIndex(j);
                    --j;
                }
            }
        }
        for (int i = 0; i < vertices.getLength(); ++i) {
            if (vertices[i].get() == vertex) {
                vertices.delByIndex(i);
                break;
            }
        }
    }


    virtual void removeEdge(const T& from, const T& to) = 0;

    DynamicArray<T> BFS(const T& start) const {
        if (!vertexAccess.containsKey(start)) {
            throw std::invalid_argument("Start vertex not found");
        }

        HashTable<T, bool> visited;
        Queue<Vertex*> queue;
        DynamicArray<T> traversalResult;
        queue.push(vertexAccess.get(start));

        while (!queue.empty()) {
            Vertex* current = queue.front();
            queue.dequeue();

            if (visited.containsKey(current->data) && visited.get(current->data)) {
                continue;
            }

            visited.add(current->data, true);
            traversalResult.append(current->data);

            for (int i = 0; i < current->neighbors.getLength(); ++i) {
                Vertex* neighborVertex = current->neighbors[i].first;
                if (!visited.containsKey(neighborVertex->data) || !visited.get(neighborVertex->data)) {
                    queue.push(neighborVertex);
                }
            }
        }

        return traversalResult;
    }

    DynamicArray<T> DFS(const T& start) const {
        if (!vertexAccess.containsKey(start)) {
            throw std::invalid_argument("Start vertex not found");
        }

        HashTable<T, bool> visited;
        Stack<Vertex*> stack;
        DynamicArray<T> traversalResult;

        stack.push(vertexAccess.get(start));

        while (!stack.empty()) {
            Vertex* current = stack.top();
            stack.pop();

            if (visited.containsKey(current->data)) {
                if (visited.get(current->data)) {
                    continue;
                }
            }

            visited.add(current->data, true);
            traversalResult.append(current->data);

            // Чтобы порядок обхода был как раньше, идём с конца к началу
            for (int i = current->neighbors.getLength() - 1; i >= 0; --i) {
                Vertex* neighborVertex = current->neighbors[i].first;
                if (!visited.containsKey(neighborVertex->data)) {
                    stack.push(neighborVertex);
                }
            }
        }

        return traversalResult;
    }

    virtual HashTable<T, double> Dijkstra(const T& start) const {
        if (!vertexAccess.containsKey(start)) {
            throw std::invalid_argument("Start vertex not found");
        }

        HashTable<T, double> distances;
        HashTable<T, bool> visited;

        for (int i = 0; i < vertices.getLength(); ++i) {
            distances.add(vertices[i]->data, std::numeric_limits<double>::infinity());
            visited.add(vertices[i]->data, false);
        }

        distances[start] = 0;
        PriorityQueue<Pair<double, T>> pq;
        pq.push(Pair<double, T>(0, start), 0);

        while (!pq.empty()) {
            auto currentPair = pq.top();
            pq.pop();

            double currentDistance = currentPair.nodeItem.first;
            T currentVertex = currentPair.nodeItem.second;

            if (visited.get(currentVertex)) {
                continue;
            }
            visited[currentVertex] = true;

            Vertex* currentNode = vertexAccess.get(currentVertex);
            if (!currentNode) continue;

            for (int i = 0; i < currentNode->neighbors.getLength(); ++i) {
                Vertex* neighborVertex = currentNode->neighbors[i].first;
                double weight = currentNode->neighbors[i].second;

                double newDistance = currentDistance + weight;
                if (newDistance < distances.get(neighborVertex->data)) {
                    distances[neighborVertex->data] = newDistance;
                    pq.push(Pair<double, T>(newDistance, neighborVertex->data), newDistance);
                }
            }
        }
        return distances;
    }

    DynamicArray<Pair<Pair<T, T>, double>> getEdges() const {
        DynamicArray<Pair<Pair<T, T>, double>> edges;
        for (int i = 0; i < vertices.getLength(); ++i) {
            Vertex* vertex = vertices[i].get();
            for (int j = 0; j < vertex->neighbors.getLength(); ++j) {
                const auto& neighbor = vertex->neighbors[j];
                edges.append(Pair<Pair<T, T>, double>(Pair<T, T>(vertex->data, neighbor.first->data), neighbor.second));
            }
        }
        return edges;
    }

    DynamicArray<T> getVertices() const {
        DynamicArray<T> verticesList;
        for (int i = 0; i < vertices.getLength(); ++i) {
            verticesList.append(vertices[i]->data);
        }
        return verticesList;
    }

    DynamicArray<Pair<T, T>> findShortestPath(const T& start, const T& end) const {
        if (!vertexAccess.containsKey(start)) {
            throw std::invalid_argument("Start vertex not found");
        }
        if (!vertexAccess.containsKey(end)) {
            throw std::invalid_argument("End vertex not found");
        }

        // Таблица расстояний
        HashTable<T, double> distances;
        // Таблица предшественников
        HashTable<T, T> predecessors;
        // Таблица посещенных вершин
        HashTable<T, bool> visited;

        // Инициализация
        for (int i = 0; i < vertices.getLength(); ++i) {
            distances.add(vertices[i]->data, std::numeric_limits<double>::infinity());
            visited.add(vertices[i]->data, false);
        }

        distances[start] = 0;
        PriorityQueue<Pair<double, T>> pq;
        pq.push(Pair<double, T>(0, start), 0);

        // Алгоритм Дейкстры
        while (!pq.empty()) {
            auto currentPair = pq.top();
            pq.pop();

            double currentDistance = currentPair.nodeItem.first;
            T currentVertex = currentPair.nodeItem.second;

            if (visited.get(currentVertex)) {
                continue;
            }
            visited[currentVertex] = true;

            Vertex* currentNode = vertexAccess.get(currentVertex);
            if (!currentNode) continue;

            for (int i = 0; i < currentNode->neighbors.getLength(); ++i) {
                Vertex* neighborVertex = currentNode->neighbors[i].first;
                double weight = currentNode->neighbors[i].second;

                double newDistance = currentDistance + weight;
                if (newDistance < distances.get(neighborVertex->data)) {
                    distances[neighborVertex->data] = newDistance;
                    pq.push(Pair<double, T>(newDistance, neighborVertex->data), newDistance);

                    // Запоминаем предшественника
                    predecessors.add(neighborVertex->data, currentVertex);
                }
            }
        }

        // Восстановление пути
        DynamicArray<Pair<T, T>> pathEdges;
        T current = end;

        if (distances.get(end) == std::numeric_limits<double>::infinity()) {
            throw std::runtime_error("No path exists between the given vertices");
        }

        while (current != start) {
            T prev = predecessors.get(current);
            pathEdges.prepend(Pair<T, T>(prev, current));
            current = prev;
        }

        return pathEdges;
    }

    DynamicArray<Pair<T, int>> ErshovColoring() const {
        if (vertices.getLength() == 0) {
            throw std::runtime_error("Graph is empty");
        }

        // Шаг 1: Создание таблицы степеней вершин
        HashTable<Vertex*, int> ershovNumbers;
        for (int i = 0; i < vertices.getLength(); ++i) {
            Vertex* vertex = vertices[i].get();
            ershovNumbers.add(vertex, vertex->neighbors.getLength());
        }

        // Шаг 2: Сортировка вершин по убыванию степени
        DynamicArray<Vertex*> sortedVertices;
        for (int i = 0; i < vertices.getLength(); ++i) {
            sortedVertices.append(vertices[i].get());
        }

        for (int i = 1; i < sortedVertices.getLength(); ++i) {
            Vertex* key = sortedVertices[i];
            int keyDegree = ershovNumbers.get(key);
            int j = i - 1;

            // Сравниваем тоже по ershovNumbers
            while (j >= 0 && ershovNumbers.get(sortedVertices[j]) < keyDegree) {
                sortedVertices[j + 1] = sortedVertices[j];
                --j;
            }
            sortedVertices[j + 1] = key;
        }

        // Шаг 3: Раскраска вершин
        HashTable<Vertex*, int> vertexColors;
        int maxColor = 0;

        for (int i = 0; i < sortedVertices.getLength(); ++i) {
            Vertex* current = sortedVertices[i];
            HashTable<int, bool> usedColors;

            // Шаг 1: Определяем цвета, занятые соседями
            for (int j = 0; j < current->neighbors.getLength(); ++j) {
                Vertex* neighbor = current->neighbors[j].first;
                if (vertexColors.containsKey(neighbor)) {
                    usedColors.add(vertexColors[neighbor], true);
                }
            }

            // Шаг 2: Находим минимальный доступный цвет
            int color = 0;
            while (usedColors.containsKey(color)) {
                ++color;
            }

            // Шаг 3: Назначаем найденный цвет
            vertexColors.add(current, color);
            maxColor = std::max(maxColor, color);

        }
        // Шаг 4: Возвращаем результат
        DynamicArray<Pair<T, int>> coloringResult;
        for (int i = 0; i < vertices.getLength(); ++i) {
            Vertex* vertex = vertices[i].get();
            coloringResult.append(Pair<T, int>(vertex->data, vertexColors[vertex]));
        }

        return coloringResult;
    }



    virtual ~Graph() = default;
};



#endif // SEM3LAB4_GRAPH_H
