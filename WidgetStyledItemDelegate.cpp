#include "WidgetStyledItemDelegate.h"


WidgetStyledItemDelegate::WidgetStyledItemDelegate(QTableView* view, QObject* parent)
    : QStyledItemDelegate(parent), _tableView(view)
{
    for (int i = 0; i < view->model()->rowCount(); i++)
    {
        QPointer<QWidget> widget = new QWidget(_tableView->viewport());
        _widgets.append(QPair <QPointer <QWidget>, ColumnValue>(widget, ColumnValue(-1, -1)));

        widget->setObjectName("viewPortWidget");
        widget->installEventFilter(this);
        //widget->hide();
    }
}

WidgetStyledItemDelegate::~WidgetStyledItemDelegate()
{
}

QString WidgetStyledItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
    //return QStyledItemDelegate::displayText(value, locale);
    return "";
}



void WidgetStyledItemDelegate::drawCheckBox(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionButton checkBoxStyleOpt;

    QRect r = option.rect;//getting the rect of the cell

    // геометрия, чтобы поставить кнопку скрая посередине. Если придумаете что-то попроще - welcome!)
    int w = r.height() + 4;
    int h = r.height();

    int x = r.center().x() - w / 2 - 2;//the X coordinate
    int y = r.top();//the Y coordinate

    checkBoxStyleOpt.rect = QRect(x, y, w, h);
    checkBoxStyleOpt.palette = option.palette;

    if (index.row() >= _widgets.size()) return;

    QPointer<QCheckBox> checkBox = dynamic_cast<QCheckBox*>(_widgets[index.row()].first.data());
    if (checkBox.isNull()) return;

    checkBox->setMinimumSize(checkBoxStyleOpt.rect.size());
    checkBox->resize(checkBoxStyleOpt.rect.height(), checkBoxStyleOpt.rect.height());

    checkBox->move(checkBoxStyleOpt.rect.topLeft().x(), checkBoxStyleOpt.rect.topLeft().y());
}

void WidgetStyledItemDelegate::drawFrame(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionButton checkBoxStyleOpt;

    QRect rect = option.rect;//getting the rect of the cell

    if (index.row() >= _widgets.size()) return;

    QPointer<QFrame> frame = dynamic_cast<QFrame*>(_widgets[index.row()].first.data());
    if (frame.isNull()) return;

    frame->setMinimumSize(rect.size());
    frame->resize(rect.size());

    frame->move(rect.topLeft().x(), rect.topLeft().y());
}

QWidget* WidgetStyledItemDelegate::createWidgetForCell(TypeServerParameter typeValue, QVariant dataToWidget, int currentRow) const
{
    QWidget* resultWidget = nullptr;
    switch (typeValue)
    {
    case IntegerTypeServerParameter:
    case IpTypeServerParameter:
    case TextTypeServerParameter:
    {
        QStackedWidget* inputStackedWidget = new QStackedWidget(_tableView->viewport());
        inputStackedWidget->setContentsMargins(0, 0, 0, 0);

        QFrame* viewFrame = new QFrame;
        viewFrame->setContentsMargins(3, 0, 6, 0);
        QHBoxLayout* viewLabelLayout = new QHBoxLayout;
        viewLabelLayout->setContentsMargins(0, 0, 0, 0);
        QLabel* valueLabel = new QLabel;
        if (currentRow == _tableView->selectionModel()->currentIndex().row())
        {
            valueLabel->setObjectName(TEXT_ON_TABLE_SELECTED);
            inputStackedWidget->setObjectName("StackedWidgetSelected");
        }
        else
        {
            valueLabel->setObjectName(TEXT_ON_TABLE_DESELECTED);
            inputStackedWidget->setObjectName("StackedWidgetDeselected");
        }
        valueLabel->setText(dataToWidget.toString());
        QPushButton* editButton = new QPushButton;
        editButton->setFlat(true);
        editButton->setObjectName(BUTTON_EDIT);
        editButton->setMaximumSize(_ICON_HEIGHT, _ICON_HEIGHT);
        editButton->setCursor(Qt::PointingHandCursor);

        viewLabelLayout->addWidget(valueLabel);
        viewLabelLayout->addWidget(editButton);

        viewFrame->setLayout(viewLabelLayout);

        inputStackedWidget->addWidget(viewFrame);

        QFrame* editFrame = new QFrame;
        editFrame->setContentsMargins(0, 0, 6, 0);
        QHBoxLayout* editLineEditLayout = new QHBoxLayout;
        editLineEditLayout->setSpacing(0);
        editLineEditLayout->setContentsMargins(0, 0, 0, 0);
        QLineEdit* valueLineEdit = new QLineEdit;
        valueLineEdit->setMinimumHeight(_LINE_EDIT_HEIGHT - 2);
        if (typeValue == IntegerTypeServerParameter)
            valueLineEdit->setValidator(new QIntValidator);
        else if (typeValue == IpTypeServerParameter)
            valueLineEdit->setInputMask("000.000.000.000");
        valueLineEdit->setText(dataToWidget.toString());
        QPushButton* okButton = new QPushButton;
        okButton->setMinimumSize(_ICON_HEIGHT, _ICON_HEIGHT);
        okButton->setMaximumSize(_ICON_HEIGHT, _ICON_HEIGHT);
        okButton->setFlat(true);
        okButton->setObjectName(BUTTON_ALLOW);
        okButton->setCursor(Qt::PointingHandCursor);
        QPushButton* cancelButton = new QPushButton;
        cancelButton->setMinimumSize(_ICON_HEIGHT, _ICON_HEIGHT);
        cancelButton->setMaximumSize(_ICON_HEIGHT, _ICON_HEIGHT);
        cancelButton->setFlat(true);
        cancelButton->setObjectName(BUTTON_DENY);
        cancelButton->setCursor(Qt::PointingHandCursor);

        editLineEditLayout->addWidget(valueLineEdit);
        editLineEditLayout->addWidget(okButton);
        editLineEditLayout->addWidget(cancelButton);

        editFrame->setLayout(editLineEditLayout);

        inputStackedWidget->addWidget(editFrame);

        resultWidget = inputStackedWidget;

        connect(okButton, &QPushButton::clicked, [=]()
        {
            updateDataInCell(valueLineEdit->text(), currentRow);
            inputStackedWidget->setCurrentIndex(0);
        });

        connect(editButton, &QPushButton::clicked, [=]()
        {
            QModelIndex curIndex = _tableView->currentIndex();
            valueLineEdit->setText(valueLabel->text());
            inputStackedWidget->setCurrentIndex(1);
            _tableView->setCurrentIndex(curIndex);
        });

        connect(cancelButton, &QPushButton::clicked, [=]()
        {
            QModelIndex curIndex = _tableView->currentIndex();
            inputStackedWidget->setCurrentIndex(0);
            _tableView->setCurrentIndex(curIndex);
        });

    }
        break;
    case BoolTypeServerParameter:
    {
        QCheckBox* boolCheckBox = new QCheckBox(_tableView->viewport());
        boolCheckBox->setChecked(dataToWidget.toBool());
        boolCheckBox->setObjectName("viewPortCheckBox");

        resultWidget = boolCheckBox;

        connect(boolCheckBox, &QCheckBox::clicked, [=]()
        {
            updateDataInCell(((boolCheckBox->isChecked()) ? "true" : "false"), currentRow);
            //_tableView->selectRow(currentRow);
        });

    }
        break;
    case PathToFileTypeServerParameter:
    case PathToDirTypeServerParameter:
    {
        QFrame* pathToFrame = new QFrame(_tableView->viewport());
        pathToFrame->setContentsMargins(3, 0, 6, 0);
        QHBoxLayout* pathLayout = new QHBoxLayout;
        pathLayout->setContentsMargins(0, 0, 0, 0);
        pathToFrame->setLayout(pathLayout);
        QLabel* pathLabel = new QLabel;
        if (currentRow == _tableView->selectionModel()->currentIndex().row())
        {
            pathLabel->setObjectName(TEXT_ON_TABLE_SELECTED);
            pathToFrame->setObjectName("PathToFrameSelected");
        }
        else
        {
            pathLabel->setObjectName(TEXT_ON_TABLE_DESELECTED);
            pathToFrame->setObjectName("PathToFrameDeselected");
        }
        pathLabel->setText(dataToWidget.toString());
        QPushButton* pathPushButton = new QPushButton;
        pathPushButton->setFlat(true);
        pathPushButton->setObjectName("pathButton");
        pathPushButton->setCursor(Qt::PointingHandCursor);
        pathPushButton->setMaximumSize(_ICON_HEIGHT, _ICON_HEIGHT);

        pathLayout->addWidget(pathLabel);
        pathLayout->addWidget(pathPushButton);

        resultWidget = pathToFrame;

        connect(pathPushButton, &QPushButton::clicked, [=]()
        {
            if (typeValue == PathToFileTypeServerParameter)
            {
                QString filePath = QFileDialog::getOpenFileName(_tableView, tr("Выберите файл"), "", tr("Все файлы (*)"));
                if (filePath.isEmpty())
                {

                }
                else
                {
                    updateDataInCell(filePath, currentRow);
                }
            }
            else if (typeValue == PathToDirTypeServerParameter)
            {
                QString filePath = QFileDialog::getExistingDirectory(_tableView, tr("Выберите директорию"));
                if (filePath.isEmpty())
                {

                }
                else
                {
                    updateDataInCell(filePath, currentRow);
                }
            }
        });

        
    }
        break;
    default:
        break;
    }
    return resultWidget;
}

void WidgetStyledItemDelegate::updateDataInCell(QVariant dataToWidget, int currentRow) const
{
    QStringList dataToEdit;
    QAbstractItemModel* model = _tableView->model();

    for (int i = ServerSettingsTableParameter; i <= ServerSettingsTableDescription; i++)
    {
        if (i == ServerSettingsTableValue)
        {
            dataToEdit << dataToWidget.toString();
        }
        else
        {
            dataToEdit << model->index(currentRow, i).data().toString();
        }
    }
    const_cast<WidgetStyledItemDelegate*>(this)->emit signalEditData(dataToEdit);
}


void WidgetStyledItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    QStyledItemDelegate::paint(painter, option, index);
    painter->restore();
    QAbstractItemModel* model = const_cast<QAbstractItemModel*>(index.model());
    QVariant dataFromCell;
    int indexRow = index.row();
    
    // Проверка, что строка находится в пределах диапазона модели и контейнера
    if (index.row() >= 0 && index.row() < model->rowCount() && index.row() < _widgets.size())
    {
        QModelIndex typeIndex = model->index(index.row(), ServerSettingsTableTypeValue);
        TypeServerParameter typeValue = static_cast<TypeServerParameter>(typeIndex.data().toInt());
        QPointer<QWidget> currentWidget = _widgets[index.row()].first;
        if (!currentWidget.isNull()) {
            currentWidget->setVisible(true);

            dataFromCell = model->index(index.row(), ServerSettingsTableValue).data();
            switch (typeValue)
            {
            case IntegerTypeServerParameter:
            case TextTypeServerParameter:
            case IpTypeServerParameter:
            {
                if ((index.row() != _tableView->selectionModel()->currentIndex().row() && currentWidget->objectName() != "StackedWidgetDeselected") ||
                    (index.row() == _tableView->selectionModel()->currentIndex().row() && currentWidget->objectName() != "StackedWidgetSelected") ||
                    dataFromCell != _widgets[index.row()].second.second)
                {
                    delete currentWidget;
                    currentWidget.clear();
                    _widgets[index.row()].first.clear();
                    _widgets[index.row()].first = createWidgetForCell(typeValue, dataFromCell, index.row());
                }
                drawFrame(painter, option, index);
            }
            break;
            case BoolTypeServerParameter:
            {
                if (currentWidget->objectName() != "viewPortCheckBox" || dataFromCell != _widgets[index.row()].second.second)
                {
                    delete currentWidget;
                    currentWidget.clear();
                    _widgets[index.row()].first.clear();
                    _widgets[index.row()].first = createWidgetForCell(typeValue, dataFromCell, index.row());
                }
                drawCheckBox(painter, option, index);
            }
            break;
            case PathToFileTypeServerParameter:
            case PathToDirTypeServerParameter:
            {
                if ((index.row() != _tableView->selectionModel()->currentIndex().row() && currentWidget->objectName() != "PathToFrameDeselected") ||
                    (index.row() == _tableView->selectionModel()->currentIndex().row() && currentWidget->objectName() != "PathToFrameSelected") ||
                    dataFromCell != _widgets[index.row()].second.second)
                {
                    delete currentWidget;
                    currentWidget.clear();
                    _widgets[index.row()].first.clear();
                    _widgets[index.row()].first = createWidgetForCell(typeValue, dataFromCell, index.row());
                }
                drawFrame(painter, option, index);
            }
            break;
            default:
                break;
            }
            _widgets[index.row()].second = ColumnValue(index.row(), dataFromCell);
        }
        
    }
    else if (index.row() < _widgets.size())
    { 
        QPointer<QWidget> curWidget = _widgets[index.row()].first;
        if (!curWidget.isNull()) {
            curWidget->setVisible(true);
        }
        _widgets[index.row()].second = ColumnValue(-1, -1);
    }
}

int WidgetStyledItemDelegate::buttonsCount()
{
    return _widgets.size();
}

void WidgetStyledItemDelegate::hideButtons()
{
    for (auto& buttonPair : _widgets) {
        if (!buttonPair.first.isNull()) { // Проверяем, не был ли объект удален
            buttonPair.first->hide();
        }
    }
}

void WidgetStyledItemDelegate::checkWidgets(int rowCount)
{
    int countButtons = _widgets.count();
    if (rowCount > countButtons)
    {
        // Если в модели больше строк, чем чекбоксов, добавляем новые чекбоксы.
        for (int i = 0; i < rowCount - countButtons; i++)
        {
            QPointer<QWidget> widget = new QWidget(_tableView->viewport());
            _widgets.append(QPair <QPointer <QWidget>, ColumnValue>(widget, ColumnValue(-1, -1)));

            widget->setObjectName("viewPortWidget");
        }
        _tableView->viewport()->repaint();
    }
    else if (rowCount < countButtons)
    {
        // Если в модели меньше строк, чем виджетов, удаляем лишние виджеты.
        for (int i = 0; i < countButtons - rowCount; i++)
        {
            // Не нужно вызывать delete, так как QPointer автоматически обрабатывает удаление.
            delete _widgets[countButtons - 1 - i].first;
            _widgets.removeLast();
        }
    }
}
