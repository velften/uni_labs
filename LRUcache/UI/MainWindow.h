
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <vector>
#include <string>
#include <QSpinBox>
#include "../src/datamanagment/Person.h"
#include "../src/objects/LRUcache.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void loadFile();
    void loadTestData();
    void search(const std::string& key);

private:
    void setupUI();
    void updateTableView();
    void updateCacheView();
    void updateCacheSize(int);

    QPushButton* loadButton;
    QPushButton* testButton;
    QComboBox* keySelector;
    QLineEdit* searchField;
    QPushButton* searchButton;
    QTableWidget* tableWidget;
    QTableWidget* cacheWidget;
    QSpinBox* cacheSizeField;
    QLabel* cacheSizeLabel;

    std::vector<Person> data; // Хранение данных из JSON
    LRUCache<std::string, Person> cache; // Кэш для поиска
};

#endif // MAINWINDOW_H
