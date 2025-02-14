#include "mainwindow.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Задаём минимальный размер окна 1280x720
    this->setMinimumSize(1280, 720);

    // Инициализируем UI (создаём модель, таблицу и делегат)
    setUI();

    setConnects();



    // Заполняем таблицу тестовыми данными
    // setData(10);
}

MainWindow::~MainWindow() {}

void MainWindow::setUI()
{
    _mainLayout = new QVBoxLayout; // Создаём вертикальный лэйаут
    QWidget* mainWidget = new QWidget; // Основной контейнер для центрального виджета
    mainWidget->setLayout(_mainLayout); // Привязываем лэйаут к контейнеру

    // Создаём модель данных, таблицу и делегат для стилизации
    _tableModel = new MyTableModel;
    _tableView = new QTableView;


    // Устанавливаем поведение выделения строк в таблице
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Выделение всей строки при клике
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection); // Можно выделить только одну строку

    // Привязываем модель и делегат к таблице
    _tableView->setModel(_tableModel);

    setData(30);



    _tableDelegate = new WidgetStyledItemDelegate(_tableView);
    _tableIntToTextDelegate = new IntToTextDelegate;

    _tableView->setItemDelegateForColumn(ServerSettingsTableValue, _tableDelegate);
    _tableView->setItemDelegateForColumn(ServerSettingsTableTypeValue, _tableIntToTextDelegate);

    // Вариант 2: Автоматическое изменение размера столбцов
    for (int col = 0; col < _tableModel->columnCount() - 1; ++col) {
        if (col == ServerSettingsTableValue) continue;
        _tableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::ResizeToContents);
    }
    _tableView->horizontalHeader()->resizeSection(ServerSettingsTableValue, 500);
    _tableView->horizontalHeader()->setSectionResizeMode(_tableModel->columnCount() - 1, QHeaderView::Stretch);
    // Добавляем таблицу в основной лэйаут
    _mainLayout->addWidget(_tableView);




    // Делаем `mainWidget` центральным виджетом окна
    setCentralWidget(mainWidget);

    // Запускаем таймер, который обновляет viewport таблицы.
    QPointer<QTimer> updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        // for (int col = 0; col < _tableModel->columnCount() - 1; ++col) {
        //     _tableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::ResizeToContents);
        // }
        _tableView->viewport()->update();
    });
    updateTimer->start(100);
}

void MainWindow::setConnects()
{
    // В конструкторе MainWindow после создания _tableDelegate и установки его в QTableView
    connect(_tableDelegate, &WidgetStyledItemDelegate::signalEditData,
            this, [this](const QStringList &dataToEdit) {
        // Если список пуст, ничего не делаем
        if (dataToEdit.isEmpty())
            return;

        // Первый элемент – идентификатор (значение из столбца ServerSettingsTableParameter)
        QString identifier = dataToEdit.first();

        // Ищем строку в модели, где столбец ServerSettingsTableParameter равен identifier
        int rowToUpdate = -1;
        for (int row = 0; row < _tableModel->rowCount(); ++row) {
            QModelIndex idx = _tableModel->index(row, ServerSettingsTableParameter);
            if (_tableModel->data(idx).toString() == identifier) {
                rowToUpdate = row;
                break;
            }
        }

        // Если строка не найдена, выходим
        if (rowToUpdate == -1)
            return;

        QString newValue = dataToEdit.value(ServerSettingsTableValue);

        // Обновляем значение в модели в найденной строке в столбце ServerSettingsTableValue.
        _tableModel->setCellData(rowToUpdate, ServerSettingsTableValue, newValue);
    });
}

void MainWindow::setData(int rows)
{
    // Проходим по каждой строке, которую нужно добавить
    for (int i = 0; i < rows; ++i)
    {
        // 1. Генерируем короткое название параметра на английском языке.
        // Пример: "param1", "param2", ...
        QString englishParam = QString("param%1").arg(i + 1);

        // 2. Генерируем короткое название параметра на русском языке.
        // Пример: "Параметр 1", "Параметр 2", ...
        QString russianParam = QString("Параметр %1").arg(i + 1);

        // 3. Случайным образом выбираем тип параметра из перечисления TypeServerParameter.
        // Используем QRandomGenerator для генерации числа в диапазоне от IntegerTypeServerParameter
        // до TextTypeServerParameter включительно.
        int type = QRandomGenerator::global()->bounded(IntegerTypeServerParameter, TextTypeServerParameter + 1);

        // 4. В зависимости от выбранного типа, генерируем соответствующее значение.
        QVariant value;
        switch (type)
        {
        case IntegerTypeServerParameter:
        {
            // Генерируем случайное целое число от 0 до 1000
            int intValue = QRandomGenerator::global()->bounded(0, 1001);
            value = intValue;
            break;
        }
        case BoolTypeServerParameter:
        {
            // Генерируем случайное булево значение (true или false)
            bool boolValue = QRandomGenerator::global()->bounded(0, 2) == 1;
            value = boolValue;
            break;
        }
        case PathToFileTypeServerParameter:
        {
            // Генерируем случайный номер файла
            int fileNumber = QRandomGenerator::global()->bounded(1, 100);

            // Получаем текущую директорию проекта
            QDir currentDir(QDir::currentPath());

            // Формируем относительный путь до файла, например "file42.txt"
            QString filePath = currentDir.absoluteFilePath(QString("file%1.txt").arg(fileNumber));

            value = filePath;
            break;
        }
        case PathToDirTypeServerParameter:
        {
            // Генерируем случайный номер директории
            int dirNumber = QRandomGenerator::global()->bounded(1, 100);

            // Получаем текущую директорию проекта
            QDir currentDir(QDir::currentPath());

            // Формируем относительный путь до директории, например "dir42"
            QString dirPath = currentDir.absoluteFilePath(QString("dir%1").arg(dirNumber));

            value = dirPath;
            break;
        }
        case IpTypeServerParameter:
        {
            // Генерируем случайный IP-адрес.
            // Каждый октет от 0 до 255
            int octet1 = QRandomGenerator::global()->bounded(0, 256);
            int octet2 = QRandomGenerator::global()->bounded(0, 256);
            int octet3 = QRandomGenerator::global()->bounded(0, 256);
            int octet4 = QRandomGenerator::global()->bounded(0, 256);
            value = QString("%1.%2.%3.%4").arg(octet1).arg(octet2).arg(octet3).arg(octet4);
            break;
        }
        case TextTypeServerParameter:
        {
            // Генерируем случайный текст.
            // Пример: "Some random text 1"
            value = QString("Описание параметра %1").arg(i + 1);
            break;
        }
        default:
        {
            // Если тип не распознан, оставляем значение пустым.
            value = QVariant();
            break;
        }
        }

        // 5. Генерируем описание параметра на русском языке.
        // Пример: "Описание параметра 1"
        QString description = QString("Описание параметра %1").arg(i + 1);

        // 6. Устанавливаем сгенерированные данные в модель таблицы по соответствующим столбцам:
        _tableModel->setCellData(i, ServerSettingsTableParameter, englishParam);         // Столбец с английским названием параметра
        _tableModel->setCellData(i, ServerSettingsTableName, russianParam);                // Столбец с русским названием параметра
        _tableModel->setCellData(i, ServerSettingsTableValue, value);                      // Столбец со значением параметра
        _tableModel->setCellData(i, ServerSettingsTableTypeValue, type);                   // Столбец с типом параметра (enum в виде int)
        _tableModel->setCellData(i, ServerSettingsTableDescription, description);          // Столбец с описанием параметра
    }
}

