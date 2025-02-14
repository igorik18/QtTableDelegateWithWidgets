#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include "common.h"

class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyTableModel(QObject *parent = nullptr);

    // Обязательный метод: возвращает количество строк
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // Обязательный метод: возвращает количество столбцов
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Обязательный метод: возвращает данные для отображения
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Переопределяем метод для задания заголовков столбцов и строк
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Дополнительный метод для динамического добавления данных
    void setCellData(int row, int column, const QVariant &value);

    // Сюда можно добавить ещё функции для удаления строчек и столбцов, либо приведение таблицы к нужному размеру
    // Но мне показалось, что добавлять их нет смысла, так как проверить эти функции программа по тз не может
    // Поэтому сделал минимально работающий проект с нужными требованиями

private:
    // Двумерный вектор для хранения данных модели
    QVector<QVector<QVariant>> m_data;
};

#endif // MYTABLEMODEL_H
