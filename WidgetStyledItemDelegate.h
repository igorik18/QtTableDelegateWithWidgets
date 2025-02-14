#pragma once

#include <QStyledItemDelegate>
#include <QTableView>
#include <QPainter>
#include <QPointer>
#include <QCheckBox>
#include <QStackedWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include "common.h"

// Этот делегат преобразует целочисленные значения в текст в зависимости от их значения.
class IntToTextDelegate : public QStyledItemDelegate
{
public:
    explicit IntToTextDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {}

    // Переопределяем метод displayText, который вызывается при отображении ячейки.
    QString displayText(const QVariant &value, const QLocale &locale) const override
    {
        bool ok = false;
        int intValue = value.toInt(&ok);
        if (ok) {
            // Здесь задаем соответствие числового значения и текстового представления.
            switch (static_cast<TypeServerParameter>(intValue)) {
            case TypeServerParameter::BoolTypeServerParameter:
                return "Логический 0 или 1";
            case TypeServerParameter::IntegerTypeServerParameter:
                return "Целочисленное значение";
            case TypeServerParameter::IpTypeServerParameter:
                return "Ip";
            case TypeServerParameter::PathToDirTypeServerParameter:
                return "Путь до папки";
            case TypeServerParameter::PathToFileTypeServerParameter:
                return "Путь до файла";
            case TypeServerParameter::TextTypeServerParameter:
                return "Текст";
            default:
                return "Неизвестно";
            }
        }
        // Если значение не int – возвращаем стандартное представление.
        return QStyledItemDelegate::displayText(value, locale);
    }
};


class WidgetStyledItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

private:
	void drawCheckBox(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void drawFrame(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;


	QWidget* createWidgetForCell(TypeServerParameter typeValue, QVariant dataToWidget, int currentRow) const;

	void updateDataInCell(QVariant dataToWidget, int currentRow) const;

public:
    WidgetStyledItemDelegate(QTableView* view, QObject* parent = nullptr);
	~WidgetStyledItemDelegate();

	QString displayText(const QVariant& value, const QLocale& locale) const override;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	int buttonsCount();
	void hideButtons();

public slots:
	void checkWidgets(int rowCount);

signals:
	void signalEditData(QStringList data);

private:
    // необходимый виджет и положение в таблице (строка + столбец). Необходимо для правильного отображения виджетов при обновлении и сортировке страницы
	mutable QVector <QPair <QPointer <QWidget>, ColumnValue>> _widgets; 
	QTableView* _tableView;
};
