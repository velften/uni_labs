
#ifndef SEM3LAB4_GUI_H
#define SEM3LAB4_GUI_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QString>
#include <QCheckBox>
#include "../src/containers/DirectedGraph.h"
#include "../src/containers/UndirectedGraph.h"


class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadGraph();
    void saveGraph();
    void clearGraph();
    void runBFS();
    void runDFS();
    void runDijkstra();
    void addEdge();
    void deleteEdge();
    void deleteVertex();
    void addVertex();
    void createEmptyGraph();
    void highlightShortestPath();


private:
    // Визуальные элементы
    QGraphicsView *graphView;
    QGraphicsScene *graphScene;
    QTextEdit *logOutput;
    QRadioButton *directedButton;
    QRadioButton *undirectedButton;
    QRadioButton *weightedButton;
    QRadioButton *nonWeightedButton;
    QRadioButton *addVertexButton;
    QRadioButton *deleteVertexButton;
    QRadioButton *deleteEdgeButton;
    QRadioButton *addEdgeButton ;
    QRadioButton *createGraphButton;

    Graph<QString> *graph;
    QList<Pair<QString, QString>> highlightedEdges;
    QMap<QString, int> ershovColors;

    void renderGraph();
    void runErshovColoring();

    bool weighted;
};

#endif // GUI_H

