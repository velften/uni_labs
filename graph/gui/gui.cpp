#include "gui.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsTextItem>
#include <stdexcept>


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          graph(nullptr)
{

    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout;

    auto *graphTypeLayout = new QHBoxLayout;
    directedButton = new QRadioButton("Directed graph");
    undirectedButton = new QRadioButton("Undirected graph");
    undirectedButton->setChecked(true);
    weightedButton = new QRadioButton("Weighted");
    nonWeightedButton = new QRadioButton("Unweighted");
    nonWeightedButton->setChecked(true);

    graphTypeLayout->addWidget(directedButton);
    graphTypeLayout->addWidget(undirectedButton);
    graphTypeLayout->addWidget(weightedButton);
    graphTypeLayout->addWidget(nonWeightedButton);
    auto *columnsLayout = new QHBoxLayout;

    auto *leftColumn = new QVBoxLayout;
    auto *createGraphButton = new QPushButton("Create empty graph");
    auto *loadButton = new QPushButton("Load graph from JSON");
    auto *saveButton = new QPushButton("Save graph in JSON");
    auto *deleteEdgeButton = new QPushButton("Delete edge");
    leftColumn->addWidget(createGraphButton);
    leftColumn->addWidget(loadButton);
    leftColumn->addWidget(saveButton);
    leftColumn->addWidget(deleteEdgeButton);

    auto *rightColumn = new QVBoxLayout;
    auto *addVertexButton = new QPushButton("Add vertex");
    auto *addEdgeButton = new QPushButton("Add edge");
    auto *deleteVertexButton = new QPushButton("Delete vertex");
    auto *clearButton = new QPushButton("Clear graph");
    rightColumn->addWidget(addVertexButton);
    rightColumn->addWidget(deleteVertexButton);
    rightColumn->addWidget(addEdgeButton);
    rightColumn->addWidget(clearButton);

    columnsLayout->addLayout(leftColumn);
    columnsLayout->addLayout(rightColumn);

    graphView = new QGraphicsView;
    graphScene = new QGraphicsScene;
    graphView->setScene(graphScene);
    mainLayout->addLayout(graphTypeLayout);
    mainLayout->addLayout(columnsLayout);
    mainLayout->addWidget(graphView, 1);
    auto *algorithmLayout = new QHBoxLayout;
    auto *bfsButton = new QPushButton("Run BFS");
    auto *dfsButton = new QPushButton("Run DFS");
    auto *dijkstraButton = new QPushButton("Run Dijkstra");
    auto *shortestPathButton = new QPushButton("Find shortest path");
    auto *ershovColoringButton = new QPushButton("Colouring of graph");
    algorithmLayout->addWidget(bfsButton);
    algorithmLayout->addWidget(dfsButton);
    algorithmLayout->addWidget(dijkstraButton);
    algorithmLayout->addWidget(shortestPathButton);
    algorithmLayout->addWidget(ershovColoringButton);
    logOutput = new QTextEdit;
    logOutput->setReadOnly(true);
    mainLayout->addLayout(algorithmLayout);
    mainLayout->addWidget(logOutput);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(createGraphButton, &QPushButton::clicked, this, &MainWindow::createEmptyGraph);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadGraph);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveGraph);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearGraph);
    connect(addVertexButton, &QPushButton::clicked, this, &MainWindow::addVertex);
    connect(deleteVertexButton, &QPushButton::clicked, this, &::MainWindow::deleteVertex);
    connect(deleteEdgeButton, &QPushButton::clicked, this, &::MainWindow::deleteEdge);
    connect(addEdgeButton, &QPushButton::clicked, this, &MainWindow::addEdge);
    connect(bfsButton, &QPushButton::clicked, this, &MainWindow::runBFS);
    connect(dfsButton, &QPushButton::clicked, this, &MainWindow::runDFS);
    connect(dijkstraButton, &QPushButton::clicked, this, &MainWindow::runDijkstra);
    connect(shortestPathButton, &QPushButton::clicked, this, &MainWindow::highlightShortestPath);
    connect(ershovColoringButton, &QPushButton::clicked, this, &MainWindow::runErshovColoring);

}

void MainWindow::createEmptyGraph()
{
    delete graph;
    if (directedButton->isChecked()) {
        if (weightedButton->isChecked()) {
            graph = new DirectedGraph<QString>();
        } else {
            graph = new DirectedGraph<QString>();
        }
    }
    else if (undirectedButton->isChecked()) {
        if (weightedButton->isChecked()) {
            graph = new UndirectedGraph<QString>();
        } else {
            graph = new UndirectedGraph<QString>();
        }
    } else {
        logOutput->append("Error: Unknown type of graph.");
        return;
    }

    logOutput->append("Empty graph inited.");
    renderGraph();
}
// Деструктор
MainWindow::~MainWindow() {
    delete graph;
}

// Загрузка графа из JSON
void MainWindow::loadGraph() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Graph JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        logOutput->append("Error: Unable to open file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        logOutput->append("Error: Invalid JSON format.");
        return;
    }

    QJsonObject json = doc.object();
    QString type = json["type"].toString();

    delete graph;
    if (type == "directed") {
        graph = new DirectedGraph<QString>;
        directedButton->setChecked(true);
    } else if (type == "undirected") {
        graph = new UndirectedGraph<QString>;
        undirectedButton->setChecked(true);
    } else {
        logOutput->append("Error: Unknown graph type.");
        return;
    }

    QJsonArray vertices = json["vertices"].toArray();
    for (int i = 0; i < vertices.size(); ++i) {
        graph->addVertex(vertices[i].toString());
    }

    QJsonArray edges = json["edges"].toArray();
    for (int i = 0; i < edges.size(); ++i) {
        QJsonObject edge = edges[i].toObject();
        QString from = edge["from"].toString();
        QString to = edge["to"].toString();
        double weight = edge["weight"].toDouble();
        graph->addEdge(from, to, weight);
    }

    logOutput->append("Graph loaded successfully.");
    renderGraph();
}

// Сохранение графа в JSON
void MainWindow::saveGraph() {
    if (!graph) {
        logOutput->append("Error: No graph to save.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Graph JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        logOutput->append("Error: Unable to save file.");
        return;
    }

    QJsonObject json;
    json["type"] = directedButton->isChecked() ? "directed" : "undirected";

    QJsonArray vertices;
    DynamicArray<QString> vertexList = graph->getVertices();
    for (int i = 0; i < vertexList.getLength(); ++i) {
        vertices.append(vertexList[i]);
    }
    json["vertices"] = vertices;

    QJsonArray edges;
    DynamicArray<Pair<Pair<QString, QString>, double>> edgeList = graph->getEdges();
    for (int i = 0; i < edgeList.getLength(); ++i) {
        QJsonObject edgeObj;
        edgeObj["from"] = edgeList[i].first.first;
        edgeObj["to"] = edgeList[i].first.second;
        edgeObj["weight"] = edgeList[i].second;
        edges.append(edgeObj);
    }
    json["edges"] = edges;

    QJsonDocument doc(json);
    file.write(doc.toJson());

    logOutput->append("Graph saved successfully.");
}

// Очистка графа
void MainWindow::clearGraph() {
    delete graph;
    graph = nullptr;
    graphScene->clear();
    logOutput->append("Graph cleared.");
}

void MainWindow::runBFS()
{
    if (!graph) {
        logOutput->append("Error: graph is not loaded.");
        return;
    }
    if (weightedButton->isChecked()) {
        logOutput->append("BFS is unable for weighted graph.");
        return;
    }

    QString start = QInputDialog::getText(this, "BFS", "Enter start vertex:");
    if (start.isEmpty()) return;

    try {
        DynamicArray<QString> result = graph->BFS(start);
        QStringList resultList;
        for (int i = 0; i < result.getLength(); ++i) {
            resultList.append(result[i]);
        }
        logOutput->append("Result of BFS: " + resultList.join(", "));
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::runDFS()
{
    if (!graph) {
        logOutput->append("Error: graph is not loaded.");
        return;
    }

    if (weightedButton->isChecked()) {
        logOutput->append("DFS is unable for weighted graph.");
        return;
    }

    QString start = QInputDialog::getText(this, "DFS", "Enter start vertex:");
    if (start.isEmpty()) return;

    try {
        DynamicArray<QString> result = graph->DFS(start);
        QStringList resultList;
        for (int i = 0; i < result.getLength(); ++i) {
            resultList.append(result[i]);
        }
        logOutput->append("Результат DFS: " + resultList.join(", "));
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::runDijkstra() {
    if (!graph) {
        logOutput->append("Error: No graph loaded.");
        return;
    }


    QString start = QInputDialog::getText(this, "Dijkstra", "Enter start vertex:");
    if (start.isEmpty()) return;

    try {
        auto result = graph->Dijkstra(start);
        QString output = "Dijkstra result:\n";
        for (auto it = result.begin(); it != result.end(); ++it) {
            output += it.key() + ": " + QString::number(*it) + "\n";
        }
        logOutput->append(output);
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::addVertex() {
    if (!graph) {
        logOutput->append("Error: Initialize a graph first using 'Create Empty Graph'.");
        return;
    }

    QString vertex = QInputDialog::getText(this, "Add Vertex", "Enter vertex name:");
    if (vertex.isEmpty()) return;

    try {
        graph->addVertex(vertex);
        logOutput->append("Vertex added: " + vertex);
        renderGraph();
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::addEdge() {
    if (!graph) {
        logOutput->append("Error: Initialize a graph first using 'Create Empty Graph'.");
        return;
    }

    QString from = QInputDialog::getText(this, "Add Edge", "Enter 'from' vertex:");
    if (from.isEmpty()) return;

    QString to = QInputDialog::getText(this, "Add Edge", "Enter 'to' vertex:");
    if (to.isEmpty()) return;

    double weight = QInputDialog::getDouble(this, "Add Edge", "Enter weight:", 1.0, 0.0, 10000.0, 2);

    try {
        graph->addEdge(from, to, weight);
        logOutput->append("Edge added: " + from + " -> " + to + " (weight: " + QString::number(weight) + ")");
        renderGraph();
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::renderGraph()
{
    graphScene->clear();

    if (!graph) {
        logOutput->append("Error: No graph to render.");
        return;
    }

    const auto &vertices = graph->getVertices();
    const auto &edges = graph->getEdges();

    QMap<QString, QPointF> vertexPositions;

    const int radius = 300;
    const int nodeRadius = 20;
    const double angleStep = 2 * M_PI / vertices.getLength();


    for (int i = 0; i < vertices.getLength(); ++i) {
        const QString &vertex = vertices[i];

        double angle = i * angleStep;
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);
        vertexPositions[vertex] = QPointF(x, y);

        QBrush brushColor(Qt::white);
        if (ershovColors.contains(vertex)) {
            int colorIndex = ershovColors[vertex];
            // Простейшая формула HSV, дающая разные оттенки
            // (чем больше colorIndex, тем сильнее сдвиг по цветовому кругу)
            QColor hsvColor = QColor::fromHsv((colorIndex * 40) % 360, 255, 255);
            brushColor = QBrush(hsvColor);
        }
        graphScene->addEllipse(
                x - nodeRadius,     // смещение, чтобы круг был "по центру" вершины
                y - nodeRadius,
                2 * nodeRadius,
                2 * nodeRadius,
                QPen(Qt::black),    // рамка (Pen)
                brushColor          // закраска (Brush)
        );

        auto *label = graphScene->addText(vertex);
        label->setPos(
                x - label->boundingRect().width() / 2,
                y - label->boundingRect().height() / 2
        );
    }

    for (int i = 0; i < edges.getLength(); ++i) {
        const auto &edge = edges[i];
        const QString &from = edge.first.first;
        const QString &to   = edge.first.second;
        double weight       = edge.second;
        if (!vertexPositions.contains(from) || !vertexPositions.contains(to)) {
            logOutput->append("Error: Edge refers to non-existent vertex.");
            continue;
        }

        QPointF fromPos = vertexPositions[from];
        QPointF toPos   = vertexPositions[to];

        bool isHighlighted = false;
        for (const auto &highlightedEdge : highlightedEdges) {
            if ((highlightedEdge.first == from && highlightedEdge.second == to) ||
                (!directedButton->isChecked() &&
                 highlightedEdge.first == to && highlightedEdge.second == from))
            {
                isHighlighted = true;
                break;
            }
        }
        QPen edgePen = isHighlighted ? QPen(Qt::red, 2) : QPen(Qt::blue, 1);

        graphScene->addLine(QLineF(fromPos, toPos), edgePen);

        QPointF midPoint = (fromPos + toPos) / 2;
        auto *weightLabel = graphScene->addText(QString::number(weight));
        weightLabel->setPos(
                midPoint.x() - weightLabel->boundingRect().width() / 2,
                midPoint.y() - weightLabel->boundingRect().height() / 2
        );

        if (directedButton->isChecked()) {
            QPointF direction = (toPos - fromPos);
            double length = std::sqrt(direction.x() * direction.x()
                                      + direction.y() * direction.y());
            QPointF unitDirection = direction / length;

            QPointF arrowP1 = toPos - unitDirection * nodeRadius
                              + QPointF(-unitDirection.y(), unitDirection.x()) * 10;
            QPointF arrowP2 = toPos - unitDirection * nodeRadius
                              + QPointF(unitDirection.y(), -unitDirection.x()) * 10;

            QPolygonF arrowHead;
            arrowHead << toPos << arrowP1 << arrowP2;
            graphScene->addPolygon(arrowHead, QPen(Qt::black), QBrush(Qt::black));
        }
    }

    logOutput->append("Graph rendered successfully.");
}

void MainWindow::highlightShortestPath() {
    if (!graph) {
        logOutput->append("Error: No graph loaded.");
        return;
    }

    QString start = QInputDialog::getText(this, "Shortest Path", "Enter start vertex:");
    if (start.isEmpty()) return;

    QString end = QInputDialog::getText(this, "Shortest Path", "Enter end vertex:");
    if (end.isEmpty()) return;

    try {
        // Получаем рёбра кратчайшего пути
        auto shortestPathEdges = graph->findShortestPath(start, end); // Метод Graph

        if (shortestPathEdges.getLength() == 0) {
            logOutput->append("No path found between " + start + " and " + end + ".");
            return;
        }

        // Сохраняем рёбра кратчайшего пути
        highlightedEdges.clear();
        for (int i = 0; i < shortestPathEdges.getLength(); ++i) {
            highlightedEdges.append(shortestPathEdges[i]);
        }

        logOutput->append("Shortest path highlighted from " + start + " to " + end + ".");
        renderGraph(); // Перерисовываем граф с выделением
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}


void MainWindow::runErshovColoring()
{
    if (!graph) {
        logOutput->append("Ошибка: Граф не загружен или не создан.");
        return;
    }

    try {
        auto coloringResult = graph->ErshovColoring();

        ershovColors.clear(); // ershovColors: QMap<QString,int>

        for (int i = 0; i < coloringResult.getLength(); ++i) {
            const auto &pair = coloringResult[i];
            ershovColors[pair.first] = pair.second;
        }

        logOutput->append("Ershov colouring: ");
        for (auto it = ershovColors.begin(); it != ershovColors.end(); ++it) {
            logOutput->append(it.key() + " : " + QString::number(it.value()));
        }
        renderGraph();

    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::deleteVertex() {
    if (!graph) {
        logOutput->append("Error: Initialize a graph first using 'Create Empty Graph'.");
        return;
    }

    QString vertex = QInputDialog::getText(this, "Delete Vertex", "Enter vertex name:");
    if (vertex.isEmpty()) return;

    try {
        graph->removeVertex(vertex);
        logOutput->append("Vertex removed: " + vertex);
        renderGraph();
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::deleteEdge() {
    if (!graph) {
        logOutput->append("Error: Initialize a graph first using 'Create Empty Graph'.");
        return;
    }

    QString vertex1 = QInputDialog::getText(this, "Vertex from", "Enter vertex name:");
    QString vertex2 = QInputDialog::getText(this, "Vertex to", "Enter vertex name:");
    if (vertex1.isEmpty() || vertex2.isEmpty()) return;
    try {
        graph->removeEdge(vertex1, vertex2);
        logOutput->append("Edge from " + vertex1 + " to " + vertex2 + " removed");
        renderGraph();
    } catch (const std::exception &e) {
        logOutput->append("Error: " + QString::fromStdString(e.what()));
    }
}
