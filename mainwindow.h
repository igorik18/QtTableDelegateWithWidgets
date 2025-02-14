#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "WidgetStyledItemDelegate.h"
#include "mytablemodel.h"
#include <QTimer>
//#include "mystyleddelegate.h"
#include <QLayout>
#include <QWindow>
#include <QHeaderView>

#include "common.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// protected:
//     // Переопределяем showEvent, чтобы отследить момент показа окна
//     void showEvent(QShowEvent *event) override;
// Для методов
private:
    // устанавливаем пользовательский интерфейс
    void setUI();
    void setConnects();
    // добавляем необходимые данные (количество строчек с рандомными данными)
    void setData(int rows);


// Для полей
private:
    QTableView * _tableView; // Таблица с представлением
    MyTableModel * _tableModel; // Модель данных таблицы
    WidgetStyledItemDelegate * _tableDelegate; // Делегат для добавления дополнительной "красоты" таблице
    IntToTextDelegate * _tableIntToTextDelegate;

    // Понимаю, что задание это по сути 1 виджет на главном окне, но на всякий случай сделал лэйаут
    QVBoxLayout* _mainLayout;
};
#endif // MAINWINDOW_H
