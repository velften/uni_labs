#include "MainWindow.h"
#include <chrono>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSplitter>
#include <fstream>
#include <sstream>
#include <QVBoxLayout>
#include <QSpinBox>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent), cache(3) {
    setupUI();
}
void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // Верхняя панель управления
    QHBoxLayout* controlLayout = new QHBoxLayout();
    loadButton = new QPushButton("Загрузить файл");
    testButton = new QPushButton("Использовать тестовые данные");
    keySelector = new QComboBox();
    keySelector->addItems({"Имя", "Фамилия", "Возраст"});
    searchField = new QLineEdit();
    searchField->setPlaceholderText("Введите ключ для поиска...");
    searchButton = new QPushButton("Искать");

    // Настройка QSpinBox для ввода размера кэша
    cacheSizeField = new QSpinBox();
    cacheSizeField->setRange(1, 100); // Диапазон вместимости кэша
    cacheSizeField->setValue(3);      // Начальное значение
    cacheSizeField->setFixedWidth(50); // Устанавливаем компактную ширину

    // Добавление элементов управления в controlLayout
    controlLayout->addWidget(loadButton);
    controlLayout->addWidget(testButton);
    controlLayout->addWidget(keySelector);
    controlLayout->addWidget(searchField);
    controlLayout->addWidget(searchButton);
    controlLayout->addWidget(cacheSizeField); // Добавляем QSpinBox без подписи
    mainLayout->addLayout(controlLayout);

    // Основной интерфейс с разделителем
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // Левый виджет: Таблица данных
    QWidget* tableWidgetContainer = new QWidget(this);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableWidgetContainer);
    QLabel* tableLabel = new QLabel("Данные");
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({"Имя", "Фамилия", "Возраст"});

    tableLayout->addWidget(tableLabel);
    tableLayout->addWidget(tableWidget);
    splitter->addWidget(tableWidgetContainer);

    // Правый виджет: Таблица кэша
    QWidget* cacheWidgetContainer = new QWidget(this);
    QVBoxLayout* cacheLayout = new QVBoxLayout(cacheWidgetContainer);

    cacheSizeLabel = new QLabel("Текущая вместимость кэша: 3");
    QLabel* cacheLabel = new QLabel("Кэш (LRU)");
    cacheWidget = new QTableWidget(this);
    cacheWidget->setColumnCount(2);
    cacheWidget->setHorizontalHeaderLabels({"Ключ", "Значение"});

    cacheLayout->addWidget(cacheSizeLabel); // Добавляем label выше таблицы кэша
    cacheLayout->addWidget(cacheLabel);
    cacheLayout->addWidget(cacheWidget);
    splitter->addWidget(cacheWidgetContainer);

    mainLayout->addWidget(splitter);

    // Соединение сигналов и слотов
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(testButton, &QPushButton::clicked, this, &MainWindow::loadTestData);
    connect(searchButton, &QPushButton::clicked, [this]() {
        search(searchField->text().toStdString());
    });

    connect(cacheSizeField, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateCacheSize);
}

void MainWindow::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат JSON файла.");
        return;
    }

    QJsonArray jsonArray = doc.array();
    data.clear();

    for (const QJsonValue& value : jsonArray) {
        QJsonObject obj = value.toObject();
        Person person;
        person.firstName = obj["firstName"].toString().toStdString();
        person.lastName = obj["lastName"].toString().toStdString();
        person.age = obj["age"].toInt();
        data.push_back(person);
    }

    updateTableView();
    QMessageBox::information(this, "Загрузка", "Данные успешно загружены.");
}

void MainWindow::loadTestData() {
    data = {
            {"Alice", "Johnson", 25},
            {"Bob", "Smith", 30},
            {"Charlie", "Brown", 35},
            {"Diana", "Prince", 28},
            {"Eva", "Davis", 22},
            {"Frank", "Miller", 40},
            {"Grace", "Wilson", 32},
            {"Helen", "Taylor", 29},
            {"Ivan", "Petrov", 38},
            {"Julia", "Lee", 26},
            {"Kevin", "White", 42},
            {"Lily", "Hall", 24},
            {"Mike", "Brown", 36},
            {"Nina", "Martin", 31}
    };

    updateTableView();
    QMessageBox::information(this, "Тестовые данные", "Тестовые данные успешно загружены.");
}
void MainWindow::search(const std::string& key) {
    auto cacheStart = std::chrono::high_resolution_clock::now();
    try {
        auto& cacheResult = cache.get(key);
        auto cacheEnd = std::chrono::high_resolution_clock::now();
        auto cacheDuration = std::chrono::duration_cast<std::chrono::microseconds>(cacheEnd - cacheStart).count();

        // Ключ найден в кэше
        QMessageBox::information(this, "Результат", QString::fromStdString("Найдено в кэше: " + cacheResult.firstName));
        updateCacheView();
        std::cout << "Время поиска в кэше: " << cacheDuration << " мкс" << std::endl;
        return;
    } catch (const std::logic_error& e) {
        auto cacheEnd = std::chrono::high_resolution_clock::now();
        auto cacheDuration = std::chrono::duration_cast<std::chrono::microseconds>(cacheEnd - cacheStart).count();
        std::cout << "Ключ не найден в кэше. Время поиска в кэше: " << cacheDuration << " мкс" << std::endl;
    }

    // Измерение времени поиска в файле (или массиве данных)
    auto fileStart = std::chrono::high_resolution_clock::now();
    auto it = std::find_if(data.begin(), data.end(), [&](const Person& person) {
        if (keySelector->currentText() == "Имя") {
            return person.firstName == key;
        } else if (keySelector->currentText() == "Фамилия") {
            return person.lastName == key;
        } else if (keySelector->currentText() == "Возраст") {
            return std::to_string(person.age) == key;
        }
        return false;
    });
    auto fileEnd = std::chrono::high_resolution_clock::now();
    auto fileDuration = std::chrono::duration_cast<std::chrono::microseconds>(fileEnd - fileStart).count();

    if (it != data.end()) {
        cache.put(key, *it);
        QMessageBox::information(this, "Результат", QString::fromStdString("Найдено: " + it->firstName));
    } else {
        QMessageBox::warning(this, "Результат", "Ключ не найден.");
    }

    updateCacheView();
    std::cout << "Время поиска в файле/тестовых данных: " << fileDuration << " мкс" << std::endl;
}


void MainWindow::updateTableView() {
    tableWidget->clearContents();
    tableWidget->setRowCount(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        const Person& person = data[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(person.firstName)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(person.lastName)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
    }
}

void MainWindow::updateCacheSize(int newSize) {
    cache = LRUCache<std::string, Person>(newSize); // Пересоздаем кэш с новой вместимостью
    cacheSizeLabel->setText(QString("Текущая вместимость кэша: %1").arg(newSize));
    updateCacheView(); // Обновляем отображение кэша
}


void MainWindow::updateCacheView() {
    cacheWidget->clearContents();
    cacheWidget->setRowCount(0);

    for (auto it = cache.beginC(); it != cache.endC(); ++it) {
        const auto& pair = *it;
        const std::string& key = pair.first;
        const Person& person = pair.second;

        int row = cacheWidget->rowCount();
        cacheWidget->insertRow(row);
        cacheWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(key)));
        cacheWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(
                person.firstName + " " + person.lastName + " (" + std::to_string(person.age) + ")")));
    }
}

