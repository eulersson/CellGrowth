/********************************************************************************
** Form generated from reading UI file 'GUI.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GUI
{
public:
    QWidget *centralwidget;
    QGridLayout *MainWindow_gridLayout;
    QPushButton *pushButton;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_2;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QComboBox *comboBox_2;
    QLabel *label_8;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QCheckBox *checkBox;
    QLabel *label_9;
    QComboBox *comboBox_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QLabel *label_5;
    QLabel *label_3;
    QSlider *horizontalSlider;
    QSlider *horizontalSlider_2;
    QSlider *horizontalSlider_3;
    QLabel *label_4;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QLabel *label_7;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QMenu *menuCells_ControlUI;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GUI)
    {
        if (GUI->objectName().isEmpty())
            GUI->setObjectName(QStringLiteral("GUI"));
        GUI->resize(932, 528);
        centralwidget = new QWidget(GUI);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MainWindow_gridLayout = new QGridLayout(centralwidget);
        MainWindow_gridLayout->setObjectName(QStringLiteral("MainWindow_gridLayout"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        MainWindow_gridLayout->addWidget(pushButton, 5, 1, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(10, 18));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        doubleSpinBox = new QDoubleSpinBox(groupBox);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setMinimum(0.01);
        doubleSpinBox->setValue(1);

        gridLayout->addWidget(doubleSpinBox, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);


        MainWindow_gridLayout->addWidget(groupBox, 0, 1, 1, 2);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        MainWindow_gridLayout->addWidget(pushButton_2, 5, 2, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMinimumSize(QSize(0, 8));
        groupBox_2->setBaseSize(QSize(0, 7));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        gridLayout_2->addWidget(comboBox_2, 0, 1, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);


        MainWindow_gridLayout->addWidget(groupBox_2, 2, 1, 1, 2);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        groupBox_4->setFlat(false);
        groupBox_4->setCheckable(false);
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        checkBox = new QCheckBox(groupBox_4);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout_4->addWidget(checkBox, 0, 0, 1, 1);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_4->addWidget(label_9, 1, 0, 1, 1);

        comboBox_3 = new QComboBox(groupBox_4);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));

        gridLayout_4->addWidget(comboBox_3, 1, 1, 1, 1);


        MainWindow_gridLayout->addWidget(groupBox_4, 3, 1, 1, 2);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tabWidget->setMinimumSize(QSize(2, 0));
        tabWidget->setBaseSize(QSize(2, 0));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_7 = new QGridLayout(tab);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setEnabled(true);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        groupBox_3->setCheckable(true);
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_5->addWidget(label_5, 2, 0, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

        horizontalSlider = new QSlider(groupBox_3);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(horizontalSlider, 0, 1, 1, 1);

        horizontalSlider_2 = new QSlider(groupBox_3);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(horizontalSlider_2, 1, 1, 1, 1);

        horizontalSlider_3 = new QSlider(groupBox_3);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(horizontalSlider_3, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_5->addWidget(label_4, 1, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_3, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 3, 0, 1, 1);

        spinBox_2 = new QSpinBox(tab_2);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));

        gridLayout_3->addWidget(spinBox_2, 3, 1, 1, 1);

        spinBox = new QSpinBox(tab_2);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        gridLayout_3->addWidget(spinBox, 2, 1, 1, 1);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 2, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        MainWindow_gridLayout->addWidget(tabWidget, 1, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        MainWindow_gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        GUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GUI);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 932, 19));
        menuCells_ControlUI = new QMenu(menubar);
        menuCells_ControlUI->setObjectName(QStringLiteral("menuCells_ControlUI"));
        GUI->setMenuBar(menubar);
        statusbar = new QStatusBar(GUI);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        GUI->setStatusBar(statusbar);

        menubar->addAction(menuCells_ControlUI->menuAction());

        retranslateUi(GUI);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(GUI);
    } // setupUi

    void retranslateUi(QMainWindow *GUI)
    {
        GUI->setWindowTitle(QApplication::translate("GUI", "MainWindow", 0));
        pushButton->setText(QApplication::translate("GUI", "Restart", 0));
        groupBox->setTitle(QApplication::translate("GUI", "Particle", 0));
        label_2->setText(QApplication::translate("GUI", "Particle Size", 0));
        label->setText(QApplication::translate("GUI", "Particle Type", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Linked Particles", 0)
         << QApplication::translate("GUI", "Growth Particles", 0)
        );
        pushButton_2->setText(QApplication::translate("GUI", "Cancle", 0));
        groupBox_2->setTitle(QApplication::translate("GUI", "Interaction", 0));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Controlled by Light", 0)
         << QApplication::translate("GUI", "Controlled by Click", 0)
         << QApplication::translate("GUI", "Random", 0)
        );
        label_8->setText(QApplication::translate("GUI", "Split Type", 0));
        groupBox_4->setTitle(QApplication::translate("GUI", "Display Options", 0));
        checkBox->setText(QApplication::translate("GUI", "Show connecions", 0));
        label_9->setText(QApplication::translate("GUI", "Type of Shading", 0));
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Ambient Occlusion", 0)
         << QApplication::translate("GUI", "X Ray", 0)
         << QApplication::translate("GUI", "ADS", 0)
        );
        groupBox_3->setTitle(QApplication::translate("GUI", "Forces", 0));
        label_5->setText(QApplication::translate("GUI", "Bulge", 0));
        label_3->setText(QApplication::translate("GUI", "Cohesion", 0));
        label_4->setText(QApplication::translate("GUI", "Spring", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("GUI", "Linked Particles", 0));
        label_7->setText(QApplication::translate("GUI", "Radius for Branch Growth", 0));
        label_6->setText(QApplication::translate("GUI", "Branches per particle", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("GUI", "Growth Particles", 0));
        menuCells_ControlUI->setTitle(QApplication::translate("GUI", "Cells ControlUI", 0));
    } // retranslateUi

};

namespace Ui {
    class GUI: public Ui_GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
