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
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QTabWidget *m_particleTab;
    QWidget *m_linkedParticleTab;
    QGridLayout *gridLayout_7;
    QGroupBox *m_LP_forces;
    QGridLayout *gridLayout_5;
    QLabel *label_3;
    QSlider *m_LP_cohesion;
    QSlider *m_LP_localCohesion;
    QLabel *label_4;
    QPushButton *LP_bulge;
    QCheckBox *m_LP_particleDeath;
    QWidget *m_growthParticleTab;
    QGridLayout *gridLayout_3;
    QLabel *label_7;
    QSpinBox *m_GP_growRadius;
    QSpinBox *m_GP_children;
    QLabel *label_6;
    QLabel *label_5;
    QDoubleSpinBox *m_GP_branchLenght;
    QGroupBox *m_splitTypeBox;
    QGridLayout *gridLayout_2;
    QComboBox *m_splitType;
    QLabel *label_8;
    QPushButton *m_LP_lightOff;
    QPushButton *m_LP_lightOn;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QDoubleSpinBox *m_particleSize;
    QLabel *label_2;
    QLabel *label;
    QComboBox *m_particleType;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_8;
    QPushButton *m_restart;
    QPushButton *m_cancel;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QCheckBox *m_showConnections;
    QLabel *label_9;
    QComboBox *m_shadingType;
    QMenuBar *menubar;
    QMenu *menuCells_ControlUI;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GUI)
    {
        if (GUI->objectName().isEmpty())
            GUI->setObjectName(QStringLiteral("GUI"));
        GUI->resize(1122, 584);
        centralwidget = new QWidget(GUI);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MainWindow_gridLayout = new QGridLayout(centralwidget);
        MainWindow_gridLayout->setObjectName(QStringLiteral("MainWindow_gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        MainWindow_gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        m_particleTab = new QTabWidget(groupBox_3);
        m_particleTab->setObjectName(QStringLiteral("m_particleTab"));
        m_particleTab->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_particleTab->sizePolicy().hasHeightForWidth());
        m_particleTab->setSizePolicy(sizePolicy);
        m_particleTab->setMinimumSize(QSize(2, 0));
        m_particleTab->setBaseSize(QSize(2, 0));
        m_linkedParticleTab = new QWidget();
        m_linkedParticleTab->setObjectName(QStringLiteral("m_linkedParticleTab"));
        gridLayout_7 = new QGridLayout(m_linkedParticleTab);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        m_LP_forces = new QGroupBox(m_linkedParticleTab);
        m_LP_forces->setObjectName(QStringLiteral("m_LP_forces"));
        m_LP_forces->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_LP_forces->sizePolicy().hasHeightForWidth());
        m_LP_forces->setSizePolicy(sizePolicy1);
        m_LP_forces->setCheckable(true);
        gridLayout_5 = new QGridLayout(m_LP_forces);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_3 = new QLabel(m_LP_forces);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_5->addWidget(label_3, 1, 0, 1, 1);

        m_LP_cohesion = new QSlider(m_LP_forces);
        m_LP_cohesion->setObjectName(QStringLiteral("m_LP_cohesion"));
        m_LP_cohesion->setAutoFillBackground(false);
        m_LP_cohesion->setMinimum(0);
        m_LP_cohesion->setMaximum(90);
        m_LP_cohesion->setValue(5);
        m_LP_cohesion->setOrientation(Qt::Horizontal);
        m_LP_cohesion->setInvertedAppearance(false);
        m_LP_cohesion->setInvertedControls(false);

        gridLayout_5->addWidget(m_LP_cohesion, 1, 1, 1, 1);

        m_LP_localCohesion = new QSlider(m_LP_forces);
        m_LP_localCohesion->setObjectName(QStringLiteral("m_LP_localCohesion"));
        m_LP_localCohesion->setAutoFillBackground(false);
        m_LP_localCohesion->setMinimum(0);
        m_LP_localCohesion->setMaximum(90);
        m_LP_localCohesion->setValue(80);
        m_LP_localCohesion->setOrientation(Qt::Horizontal);
        m_LP_localCohesion->setInvertedAppearance(false);
        m_LP_localCohesion->setInvertedControls(false);

        gridLayout_5->addWidget(m_LP_localCohesion, 2, 1, 1, 1);

        label_4 = new QLabel(m_LP_forces);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_5->addWidget(label_4, 2, 0, 1, 1);

        LP_bulge = new QPushButton(m_LP_forces);
        LP_bulge->setObjectName(QStringLiteral("LP_bulge"));

        gridLayout_5->addWidget(LP_bulge, 3, 0, 1, 2);

        m_LP_particleDeath = new QCheckBox(m_LP_forces);
        m_LP_particleDeath->setObjectName(QStringLiteral("m_LP_particleDeath"));

        gridLayout_5->addWidget(m_LP_particleDeath, 0, 0, 1, 1);


        gridLayout_7->addWidget(m_LP_forces, 0, 0, 1, 1);

        m_particleTab->addTab(m_linkedParticleTab, QString());
        m_growthParticleTab = new QWidget();
        m_growthParticleTab->setObjectName(QStringLiteral("m_growthParticleTab"));
        m_growthParticleTab->setEnabled(false);
        gridLayout_3 = new QGridLayout(m_growthParticleTab);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_7 = new QLabel(m_growthParticleTab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setEnabled(false);

        gridLayout_3->addWidget(label_7, 3, 0, 1, 1);

        m_GP_growRadius = new QSpinBox(m_growthParticleTab);
        m_GP_growRadius->setObjectName(QStringLiteral("m_GP_growRadius"));
        m_GP_growRadius->setEnabled(false);

        gridLayout_3->addWidget(m_GP_growRadius, 3, 1, 1, 1);

        m_GP_children = new QSpinBox(m_growthParticleTab);
        m_GP_children->setObjectName(QStringLiteral("m_GP_children"));
        m_GP_children->setMinimum(1);
        m_GP_children->setValue(3);

        gridLayout_3->addWidget(m_GP_children, 2, 1, 1, 1);

        label_6 = new QLabel(m_growthParticleTab);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 2, 0, 1, 1);

        label_5 = new QLabel(m_growthParticleTab);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_3->addWidget(label_5, 4, 0, 1, 1);

        m_GP_branchLenght = new QDoubleSpinBox(m_growthParticleTab);
        m_GP_branchLenght->setObjectName(QStringLiteral("m_GP_branchLenght"));
        m_GP_branchLenght->setValue(3);

        gridLayout_3->addWidget(m_GP_branchLenght, 4, 1, 1, 1);

        m_particleTab->addTab(m_growthParticleTab, QString());

        gridLayout_6->addWidget(m_particleTab, 1, 0, 1, 1);

        m_splitTypeBox = new QGroupBox(groupBox_3);
        m_splitTypeBox->setObjectName(QStringLiteral("m_splitTypeBox"));
        m_splitTypeBox->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_splitTypeBox->sizePolicy().hasHeightForWidth());
        m_splitTypeBox->setSizePolicy(sizePolicy2);
        m_splitTypeBox->setMinimumSize(QSize(0, 8));
        m_splitTypeBox->setBaseSize(QSize(0, 7));
        gridLayout_2 = new QGridLayout(m_splitTypeBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        m_splitType = new QComboBox(m_splitTypeBox);
        m_splitType->setObjectName(QStringLiteral("m_splitType"));

        gridLayout_2->addWidget(m_splitType, 0, 2, 1, 1);

        label_8 = new QLabel(m_splitTypeBox);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

        m_LP_lightOff = new QPushButton(m_splitTypeBox);
        m_LP_lightOff->setObjectName(QStringLiteral("m_LP_lightOff"));
        m_LP_lightOff->setEnabled(false);

        gridLayout_2->addWidget(m_LP_lightOff, 3, 2, 1, 1);

        m_LP_lightOn = new QPushButton(m_splitTypeBox);
        m_LP_lightOn->setObjectName(QStringLiteral("m_LP_lightOn"));
        m_LP_lightOn->setEnabled(false);

        gridLayout_2->addWidget(m_LP_lightOn, 3, 1, 1, 1);


        gridLayout_6->addWidget(m_splitTypeBox, 2, 0, 1, 2);

        groupBox = new QGroupBox(groupBox_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(10, 18));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        m_particleSize = new QDoubleSpinBox(groupBox);
        m_particleSize->setObjectName(QStringLiteral("m_particleSize"));
        m_particleSize->setMinimum(0.1);
        m_particleSize->setMaximum(100);
        m_particleSize->setValue(2);

        gridLayout->addWidget(m_particleSize, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        m_particleType = new QComboBox(groupBox);
        m_particleType->setObjectName(QStringLiteral("m_particleType"));

        gridLayout->addWidget(m_particleType, 0, 1, 1, 1);


        gridLayout_6->addWidget(groupBox, 0, 0, 1, 2);

        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        gridLayout_8 = new QGridLayout(groupBox_5);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        m_restart = new QPushButton(groupBox_5);
        m_restart->setObjectName(QStringLiteral("m_restart"));
        m_restart->setEnabled(true);

        gridLayout_8->addWidget(m_restart, 0, 0, 1, 1);

        m_cancel = new QPushButton(groupBox_5);
        m_cancel->setObjectName(QStringLiteral("m_cancel"));
        m_cancel->setEnabled(false);

        gridLayout_8->addWidget(m_cancel, 0, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_5, 4, 0, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setEnabled(true);
        sizePolicy1.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy1);
        groupBox_4->setFlat(false);
        groupBox_4->setCheckable(false);
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        m_showConnections = new QCheckBox(groupBox_4);
        m_showConnections->setObjectName(QStringLiteral("m_showConnections"));
        m_showConnections->setEnabled(true);
        m_showConnections->setCheckable(true);
        m_showConnections->setChecked(true);
        m_showConnections->setTristate(false);

        gridLayout_4->addWidget(m_showConnections, 0, 0, 1, 1);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_4->addWidget(label_9, 1, 0, 1, 1);

        m_shadingType = new QComboBox(groupBox_4);
        m_shadingType->setObjectName(QStringLiteral("m_shadingType"));

        gridLayout_4->addWidget(m_shadingType, 1, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_4, 3, 0, 1, 2);


        MainWindow_gridLayout->addWidget(groupBox_3, 0, 2, 1, 1);

        GUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GUI);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1122, 19));
        menuCells_ControlUI = new QMenu(menubar);
        menuCells_ControlUI->setObjectName(QStringLiteral("menuCells_ControlUI"));
        GUI->setMenuBar(menubar);
        statusbar = new QStatusBar(GUI);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        GUI->setStatusBar(statusbar);

        menubar->addAction(menuCells_ControlUI->menuAction());

        retranslateUi(GUI);

        m_particleTab->setCurrentIndex(0);
        m_splitType->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GUI);
    } // setupUi

    void retranslateUi(QMainWindow *GUI)
    {
        GUI->setWindowTitle(QApplication::translate("GUI", "MainWindow", 0));
        groupBox_3->setTitle(QApplication::translate("GUI", "GroupBox", 0));
        m_LP_forces->setTitle(QApplication::translate("GUI", "Forces", 0));
        label_3->setText(QApplication::translate("GUI", "Cohesion", 0));
        label_4->setText(QApplication::translate("GUI", "Local Cohesion", 0));
        LP_bulge->setText(QApplication::translate("GUI", "Bulge", 0));
        m_LP_particleDeath->setText(QApplication::translate("GUI", "Particle Death", 0));
        m_particleTab->setTabText(m_particleTab->indexOf(m_linkedParticleTab), QApplication::translate("GUI", "Linked Particles", 0));
        label_7->setText(QApplication::translate("GUI", "Radius for Branch Growth", 0));
        label_6->setText(QApplication::translate("GUI", "Branches per particle", 0));
        label_5->setText(QApplication::translate("GUI", "Branch Length", 0));
        m_particleTab->setTabText(m_particleTab->indexOf(m_growthParticleTab), QApplication::translate("GUI", "Growth Particles", 0));
        m_splitTypeBox->setTitle(QString());
        m_splitType->clear();
        m_splitType->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Controlled by Space Bar", 0)
         << QApplication::translate("GUI", "Controlled by Light On/Off", 0)
        );
        label_8->setText(QApplication::translate("GUI", "Split Type", 0));
        m_LP_lightOff->setText(QApplication::translate("GUI", "Light Off", 0));
        m_LP_lightOn->setText(QApplication::translate("GUI", "Light On", 0));
        groupBox->setTitle(QApplication::translate("GUI", "Particle", 0));
        label_2->setText(QApplication::translate("GUI", "Particle Size", 0));
        label->setText(QApplication::translate("GUI", "Particle Type", 0));
        m_particleType->clear();
        m_particleType->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Linked Particles", 0)
         << QApplication::translate("GUI", "Growth Particles", 0)
        );
        groupBox_5->setTitle(QString());
        m_restart->setText(QApplication::translate("GUI", "Restart", 0));
        m_cancel->setText(QApplication::translate("GUI", "Cancel", 0));
        groupBox_4->setTitle(QApplication::translate("GUI", "Display Options", 0));
        m_showConnections->setText(QApplication::translate("GUI", "Show connections", 0));
        label_9->setText(QApplication::translate("GUI", "Type of Shading", 0));
        m_shadingType->clear();
        m_shadingType->insertItems(0, QStringList()
         << QApplication::translate("GUI", "ADS", 0)
         << QApplication::translate("GUI", "X Ray", 0)
         << QApplication::translate("GUI", "Ambient Occlusion", 0)
        );
        m_shadingType->setCurrentText(QApplication::translate("GUI", "ADS", 0));
        menuCells_ControlUI->setTitle(QApplication::translate("GUI", "Cells ControlUI", 0));
    } // retranslateUi

};

namespace Ui {
    class GUI: public Ui_GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
