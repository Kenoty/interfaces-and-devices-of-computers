#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "BatteryMonitor.h"
#include "SpriteAnimation.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : BackgroundWindow(":/res/background2000.png", parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Лабораторные работы");
    setFixedSize(800, 600);
    setupMainMenu();
}

void MainWindow::setupMainMenu()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QWidget *buttonsContainer = new QWidget();
    QHBoxLayout *columnsLayout = new QHBoxLayout(buttonsContainer);

    QVBoxLayout *leftColumn = new QVBoxLayout();
    QVBoxLayout *rightColumn = new QVBoxLayout();

    for (int i = 1; i <= 6; ++i) {
        QPushButton *labButton = new QPushButton(QString("Лабораторная работа %1").arg(i));
        labButton->setMinimumHeight(40);
        labButton->setMinimumWidth(200);
        labButton->setStyleSheet(
            "QPushButton {"
            "    font-size: 10pt;"
            "    background-color: #4CAF50;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 5px;"
            "    margin: 5px;"
            "    padding: 8px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #45a049;"
            "}"
            );

        if (i <= 3) {
            connect(labButton, &QPushButton::clicked, this, [this, i]() {
                switch (i) {
                case 1: showLab1(); break;
                case 2: showLab2(); break;
                case 3: showLab3(); break;
                }
            });
            leftColumn->addWidget(labButton);
        } else {
            connect(labButton, &QPushButton::clicked, this, [this, i]() {
                switch (i) {
                case 4: showLab4(); break;
                case 5: showLab5(); break;
                case 6: showLab6(); break;
                }
            });
            rightColumn->addWidget(labButton);
        }
    }

    columnsLayout->addLayout(leftColumn);
    columnsLayout->addStretch();
    columnsLayout->addLayout(rightColumn);

    mainLayout->addWidget(buttonsContainer);

    SpriteAnimation *spriteAnimation = new SpriteAnimation(centralWidget);
    spriteAnimation->loadSpriteSheet(":/res/StandSpriteSheet.png", 64, 64, 4, 5.0f);
    spriteAnimation->startAnimation(8);
    spriteAnimation->setPosition(spriteAnimation->x() + 250, spriteAnimation->y() + 200);

    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLab1()
{
    BackgroundWindow *batteryWindow = new BackgroundWindow(":/res/background2000.png");
    batteryWindow->setWindowTitle("Лабораторная работа 1 - Монитор батареи");
    batteryWindow->setFixedSize(800, 600);

    BatteryMonitor *batteryMonitor = new BatteryMonitor();
    batteryMonitor->updateStatus();

    bool initialIsOnACPower = batteryMonitor->isOnACPower();
    QString initialSprite = initialIsOnACPower ? ":/res/RunSpriteSheet.png" : ":/res/StandSpriteSheet.png";
    int initialFrameCount = initialIsOnACPower ? 8 : 4;

    SpriteAnimation *batteryAnimation = new SpriteAnimation();
    batteryAnimation->loadSpriteSheet(initialSprite, 64, 64, initialFrameCount, 5.0f);
    batteryAnimation->startAnimation(8);
    batteryAnimation->setPosition(250, 200);
    batteryWindow->setSpriteAnimation(batteryAnimation);

    QWidget *centralWidget = new QWidget();
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);
    batteryWindow->setCentralWidget(centralWidget);

    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Лабораторная работа 1");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: white; background-color: rgba(0, 0, 0, 120); border-radius: 5px; padding: 8px;");
    titleLabel->setFixedSize(600, 40);
    centralLayout->addWidget(titleLabel);
    centralLayout->setAlignment(titleLabel, Qt::AlignHCenter);

    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    mainContentLayout->setContentsMargins(20, 0, 0, 0);
    mainContentLayout->setAlignment(Qt::AlignLeft);

    QWidget *textWidget = new QWidget();
    textWidget->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 8px; padding: 0px;");

    mainContentLayout->addWidget(textWidget);
    centralLayout->addLayout(mainContentLayout);

    QVBoxLayout *layout = new QVBoxLayout(textWidget);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(4);

    QLabel *acStatusLabel = new QLabel("Питание: --");
    acStatusLabel->setStyleSheet("font-size: 9pt; margin: 1px; color: #333333;");
    layout->addWidget(acStatusLabel);

    QLabel *batteryPercentLabel = new QLabel("Заряд: --%");
    batteryPercentLabel->setStyleSheet("font-size: 9pt; margin: 1px; color: #333333;");
    layout->addWidget(batteryPercentLabel);

    QLabel *batteryChemistryLabel = new QLabel("Тип: --");
    batteryChemistryLabel->setStyleSheet("font-size: 9pt; margin: 1px; color: #333333;");
    layout->addWidget(batteryChemistryLabel);

    QLabel *powerSaverLabel = new QLabel("Энергосбережение: --");
    powerSaverLabel->setStyleSheet("font-size: 9pt; margin: 1px; color: #333333;");
    layout->addWidget(powerSaverLabel);

    QLabel *batteryTimeLabel = new QLabel("Осталось: --");
    batteryTimeLabel->setStyleSheet("font-size: 9pt; margin: 1px; color: #333333;");
    layout->addWidget(batteryTimeLabel);

    QLabel *batteryFullTimeLabel = new QLabel("Полное время: --");
    batteryFullTimeLabel->setStyleSheet("font-size: 9pt; margin: 1px; color: #333333;");
    layout->addWidget(batteryFullTimeLabel);

    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setFixedHeight(1);
    frame->setStyleSheet("background-color: #888888; margin: 4px 0px;");
    layout->addWidget(frame);

    QLabel *controlLabel = new QLabel("Управление питанием:");
    controlLabel->setStyleSheet("font-size: 10pt; margin: 3px; color: #222222;");
    layout->addWidget(controlLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(6);

    QPushButton *sleepButton = new QPushButton("Сон");
    QPushButton *hibernateButton = new QPushButton("Гибернация");

    QString buttonStyle =
        "QPushButton {"
        "    font-size: 9pt;"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    margin: 1px;"
        "    padding: 4px 8px;"
        "    min-width: 80px;"
        "    min-height: 25px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}";

    sleepButton->setStyleSheet(buttonStyle);
    hibernateButton->setStyleSheet(buttonStyle);

    connect(sleepButton, &QPushButton::clicked, batteryWindow, [batteryWindow]() {
        QMessageBox::StandardButton reply = QMessageBox::question(
            batteryWindow, "Переход в спящий режим",
            "Вы уверены, что хотите перевести компьютер в спящий режим?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (BatteryMonitor::enterSleep()) {
                QMessageBox::information(batteryWindow, "Успех", "Компьютер переходит в спящий режим");
            } else {
                QMessageBox::warning(batteryWindow, "Ошибка", "Не удалось перевести компьютер в спящий режим");
            }
        }
    });

    connect(hibernateButton, &QPushButton::clicked, batteryWindow, [batteryWindow]() {
        QMessageBox::StandardButton reply = QMessageBox::question(
            batteryWindow, "Переход в гибернацию",
            "Вы уверены, что хотите перевести компьютер в режим гибернации?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (BatteryMonitor::enterHibernate()) {
                QMessageBox::information(batteryWindow, "Успех", "Компьютер переходит в режим гибернации");
            } else {
                QMessageBox::warning(batteryWindow, "Ошибка",
                                     "Не удалось перевести компьютер в режим гибернации. Возможно, эта функция не поддерживается.");
            }
        }
    });

    buttonLayout->addWidget(sleepButton);
    buttonLayout->addWidget(hibernateButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    layout->addLayout(buttonLayout);

    QTimer *updateTimer = new QTimer(batteryWindow);
    connect(updateTimer, &QTimer::timeout, [batteryMonitor, acStatusLabel, batteryChemistryLabel,
                                            batteryPercentLabel, batteryTimeLabel,
                                            batteryFullTimeLabel, powerSaverLabel, batteryAnimation]() {
        if (batteryMonitor->updateStatus()) {

            bool isOnACPower = batteryMonitor->isOnACPower();
            static bool wasOnACPower = isOnACPower;

            if (isOnACPower != wasOnACPower) {
                if (isOnACPower) {
                    batteryAnimation->loadSpriteSheet(":/res/RunSpriteSheet.png", 64, 64, 8, 5.0f);
                } else {
                    batteryAnimation->loadSpriteSheet(":/res/StandSpriteSheet.png", 64, 64, 4, 5.0f);
                }
                batteryAnimation->startAnimation(8);
                wasOnACPower = isOnACPower;
            }

            acStatusLabel->setText("Питание: " + QString::fromStdString(batteryMonitor->getPowerStatusString()));

            std::vector<std::string> chemistries = batteryMonitor->getBatteryType();
            QString chemText = chemistries.empty() ?
                                   "Тип: Неизвестен" :
                                   "Тип: " + QString::fromStdString(chemistries[0]);
            batteryChemistryLabel->setText(chemText);

            batteryPercentLabel->setText(QString("Заряд: %1%").arg(batteryMonitor->getBatteryPercent()));

            DWORD batteryTime = batteryMonitor->getBatteryLifeTime();
            QString timeText = (batteryTime == BATTERY_LIFE_UNKNOWN) ?
                                   "Осталось: неизвестно" :
                                   QString("Осталось: %1ч %2м").arg(batteryTime / 3600).arg((batteryTime % 3600) / 60);
            batteryTimeLabel->setText(timeText);

            DWORD batteryFullTime = batteryMonitor->getBatteryFullLifeTime();
            QString fullTimeText = (batteryFullTime == BATTERY_LIFE_UNKNOWN) ?
                                       "Полное время: неизвестно" :
                                       QString("Полное время: %1ч %2м").arg(batteryFullTime / 3600).arg((batteryFullTime % 3600) / 60);
            batteryFullTimeLabel->setText(fullTimeText);

            QString saverStatus = batteryMonitor->isBatterySaverOn() ? "вкл" : "выкл";
            powerSaverLabel->setText(QString("Энергосбережение: %1").arg(saverStatus));
        }
    });
    updateTimer->start(250);

    connect(batteryWindow, &QWidget::destroyed, [batteryMonitor]() {
        delete batteryMonitor;
    });

    batteryWindow->show();
}

void MainWindow::showLab2()
{
    BackgroundWindow *pciWindow = new BackgroundWindow(":/res/background2000.png");
    pciWindow->setWindowTitle("Лабораторная работа 2 - PCI устройства");
    pciWindow->setFixedSize(800, 600);

    SpriteAnimation *pciAnimation = new SpriteAnimation();
    pciAnimation->loadSpriteSheet(":/res/RunSpriteSheet.png", 64, 64, 4, 5.0f);
    pciAnimation->startAnimation(8);
    pciAnimation->setPosition(250, 200);
    pciWindow->setSpriteAnimation(pciAnimation);

    QWidget *centralWidget = new QWidget();
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);
    pciWindow->setCentralWidget(centralWidget);

    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Лабораторная работа 2 - PCI устройства");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: white; background-color: rgba(0, 0, 0, 120); border-radius: 5px; padding: 8px;");
    titleLabel->setFixedSize(600, 40);
    centralLayout->addWidget(titleLabel);
    centralLayout->setAlignment(titleLabel, Qt::AlignHCenter);

    QTableWidget *tableWidget = new QTableWidget();
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Bus" << "Device" << "Function" << "VendorID" << "DeviceID");
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "    background-color: transparent;"
        "    border: 1px solid rgba(255, 255, 255, 100);"
        "    border-radius: 8px;"
        "    padding: 5px;"
        "    font-size: 9pt;"
        "    color: white;"
        "    gridline-color: rgba(255, 255, 255, 80);"
        "}"
        "QTableWidget::item {"
        "    background-color: rgba(255, 255, 255, 30);"
        "    color: white;"
        "    padding: 3px;"
        "    border-bottom: 1px solid rgba(255, 255, 255, 60);"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: rgba(76, 175, 80, 150);"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: rgba(76, 175, 80, 180);"
        "    color: white;"
        "    padding: 4px;"
        "    border: none;"
        "    font-weight: bold;"
        "    font-size: 9pt;"
        "}"
        "QTableCornerButton::section {"
        "    background-color: rgba(76, 175, 80, 180);"
        "    border: none;"
        "}"
        );

    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    centralLayout->addWidget(tableWidget);

    auto loadPCIData = [tableWidget]() {
        tableWidget->setRowCount(0);

        QFile file("D:/5_сем/Интерфейсы/shared/pci_devices.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(tableWidget, "Ошибка",
                                 "Не удалось открыть файл pci_devices.txt\n\n"
                                 "Убедитесь, что файл существует в директории программы.\n"
                                 "Для создания файла запустите программу PCI Scanner.");
            return;
        }

        QTextStream in(&file);
        int row = 0;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() == 5) {
                tableWidget->insertRow(row);

                // Bus
                QTableWidgetItem *busItem = new QTableWidgetItem(parts[0]);
                busItem->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(row, 0, busItem);

                // Device
                QTableWidgetItem *deviceItem = new QTableWidgetItem(parts[1]);
                deviceItem->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(row, 1, deviceItem);

                // Function
                QTableWidgetItem *functionItem = new QTableWidgetItem(parts[2]);
                functionItem->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(row, 2, functionItem);

                // VendorID (hex)
                QTableWidgetItem *vendorItem = new QTableWidgetItem("0x" + parts[3].toUpper());
                vendorItem->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(row, 3, vendorItem);

                // DeviceID (hex)
                QTableWidgetItem *deviceIDItem = new QTableWidgetItem("0x" + parts[4].toUpper());
                deviceIDItem->setTextAlignment(Qt::AlignCenter);
                tableWidget->setItem(row, 4, deviceIDItem);

                row++;
            }
        }

        file.close();

        tableWidget->resizeColumnsToContents();

        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    };

    loadPCIData();

    pciWindow->show();
}

void MainWindow::showLab3() { showLabWindow("Лабораторная работа 3"); }
void MainWindow::showLab4() { showLabWindow("Лабораторная работа 4"); }
void MainWindow::showLab5() { showLabWindow("Лабораторная работа 5"); }
void MainWindow::showLab6() { showLabWindow("Лабораторная работа 6"); }

void MainWindow::showLabWindow(const QString &title)
{
    QWidget *labWindow = new QWidget();
    labWindow->setWindowTitle(title);
    labWindow->setMinimumSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(labWindow);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; margin: 20px;");
    layout->addWidget(titleLabel);

    QLabel *placeholder = new QLabel("Эта лабораторная работа еще в разработке");
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setStyleSheet("font-size: 12pt; margin: 20px;");
    layout->addWidget(placeholder);

    QPushButton *closeButton = new QPushButton("Закрыть");
    connect(closeButton, &QPushButton::clicked, labWindow, &QWidget::close);
    layout->addWidget(closeButton);

    labWindow->show();
}
