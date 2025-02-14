#include <QApplication>
#include <QGuiApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Указываем путь к QSS файлу (можно использовать ресурсный файл, если добавили его в .qrc)
    QFile styleFile(":/style/stylesheet.css");
    if(styleFile.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }
    else
    {
        qWarning() << "Не удалось открыть файл стиля!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
