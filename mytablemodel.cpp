#include "mytablemodel.h"

MyTableModel::MyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MyTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent); // Игнорируем параметр parent, т.к. модель не иерархическая (без вложенных данных)
    return m_data.size(); // Возвращаем количество строк, равное размеру внешнего QVector (число вложенных векторов)
}

int MyTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent); // Игнорируем parent по той же причине, что и в rowCount()

    return m_data.isEmpty() ? 0 : m_data.first().size(); // Если данных нет — 0 столбцов, иначе размер первой строки
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) // Проверяем, что индекс корректный
        return QVariant(); // Если нет — возвращаем пустое значение

    if (role == Qt::DisplayRole) { // Если роль — отображение данных в UI
        return m_data.at(index.row()).at(index.column()); // Возвращаем значение из соответствующей ячейки
    }

    return QVariant(); // По умолчанию возвращаем пустой QVariant (для других ролей)
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) // Поддерживаем только отображение заголовков
        return QVariant();

    if (orientation == Qt::Horizontal) { // Если заголовок для столбцов

        QString nameColumn = "";
        switch (static_cast<ServerSettingsTableColumn>(section)) {
        case ServerSettingsTableColumn::ServerSettingsTableParameter:
            nameColumn = "Параметр";
            break;
        case ServerSettingsTableColumn::ServerSettingsTableName:
            nameColumn = "Название параметра";
            break;
        case ServerSettingsTableColumn::ServerSettingsTableValue:
            nameColumn = "Значение";
            break;
        case ServerSettingsTableColumn::ServerSettingsTableTypeValue:
            nameColumn = "Тип значения";
            break;
        case ServerSettingsTableColumn::ServerSettingsTableDescription:
            nameColumn = "Описание";
            break;
        default:
            nameColumn = "Неизвестно";
            break;
        }


        return nameColumn;
    } else { // Если заголовок для строк
        return QString("V: %1").arg(section + 1);
    }
}

void MyTableModel::setCellData(int row, int column, const QVariant &value)
{
   // Получаем текущее количество строк и столбцов
   int currentRowCount = rowCount();
   int currentColumnCount = columnCount();

   // Если нужно добавить новые строки
   if (row >= currentRowCount) {
       beginInsertRows(QModelIndex(), currentRowCount, row); // Уведомляем представление

       // Добавляем недостающие строки, сразу с нужным количеством столбцов
       int newColumnCount = qMax(column + 1, currentColumnCount); // Учитываем текущие столбцы
       while (m_data.size() <= row) {
           m_data.append(QVector<QVariant>(newColumnCount));
       }

       endInsertRows();
   }

   // Если нужно добавить новые столбцы
   if (column >= currentColumnCount) {
       beginInsertColumns(QModelIndex(), currentColumnCount, column); // Уведомляем представление

       // Увеличиваем размер всех существующих строк
       for (auto &rowData : m_data) {
           rowData.resize(column + 1);
       }

       endInsertColumns();
   }

   // Устанавливаем значение в ячейку (row, column)
   m_data[row][column] = value;

   // Уведомляем представление, что данные изменились
   QModelIndex idx = index(row, column);
   emit dataChanged(idx, idx, {Qt::DisplayRole});
}
