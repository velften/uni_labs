#include <QApplication>
#include "UI/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Создаем главное окно
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Hash Table and Cache Viewer");
    mainWindow.resize(1300, 800); // Устанавливаем размер окна
    mainWindow.show();

    // Запускаем основной цикл приложения
    return app.exec();
}
