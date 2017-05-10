/********************************************************************************
** Form generated from reading UI file 'GUI.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GUI
{
public:
    QAction *actionfff;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *mainLayout;
    QVBoxLayout *verticalLayout_6;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_5;
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
    QLabel *label_6;
    QSpinBox *m_GP_children;
    QDoubleSpinBox *m_GP_branchLenght;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *m_nearestPart;
    QCheckBox *m_GP_growtoLight;
    QGroupBox *particlesBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *m_particleSize;
    QComboBox *m_particleType;
    QLabel *label_2;
    QGroupBox *m_splitTypeBox;
    QGridLayout *gridLayout_2;
    QComboBox *m_splitType;
    QPushButton *m_LP_lightOff;
    QPushButton *m_LP_lightOn;
    QLabel *label_8;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *m_restart;
    QPushButton *m_cancel;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *displayBox;
    QGridLayout *gridLayout_4;
    QCheckBox *m_showConnections;
    QSlider *m_BColour;
    QComboBox *m_shadingType;
    QLabel *label_9;
    QSlider *m_GColour;
    QSlider *m_RColour;
    QLabel *label_7;
    QLabel *label_10;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QMainWindow *GUI)
    {
        if (GUI->objectName().isEmpty())
            GUI->setObjectName(QStringLiteral("GUI"));
        GUI->resize(1288, 698);
        actionfff = new QAction(GUI);
        actionfff->setObjectName(QStringLiteral("actionfff"));
        centralwidget = new QWidget(GUI);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout_5 = new QHBoxLayout(centralwidget);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        mainLayout = new QHBoxLayout();
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_5 = new QVBoxLayout(tab);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        m_particleTab = new QTabWidget(tab);
        m_particleTab->setObjectName(QStringLiteral("m_particleTab"));
        m_particleTab->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_particleTab->sizePolicy().hasHeightForWidth());
        m_particleTab->setSizePolicy(sizePolicy1);
        m_particleTab->setMinimumSize(QSize(2, 0));
        m_particleTab->setBaseSize(QSize(2, 0));
        m_linkedParticleTab = new QWidget();
        m_linkedParticleTab->setObjectName(QStringLiteral("m_linkedParticleTab"));
        gridLayout_7 = new QGridLayout(m_linkedParticleTab);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        m_LP_forces = new QGroupBox(m_linkedParticleTab);
        m_LP_forces->setObjectName(QStringLiteral("m_LP_forces"));
        m_LP_forces->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_LP_forces->sizePolicy().hasHeightForWidth());
        m_LP_forces->setSizePolicy(sizePolicy2);
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
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(m_growthParticleTab->sizePolicy().hasHeightForWidth());
        m_growthParticleTab->setSizePolicy(sizePolicy3);
        gridLayout_3 = new QGridLayout(m_growthParticleTab);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_6 = new QLabel(m_growthParticleTab);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        m_GP_children = new QSpinBox(m_growthParticleTab);
        m_GP_children->setObjectName(QStringLiteral("m_GP_children"));
        m_GP_children->setMinimum(1);
        m_GP_children->setValue(3);

        gridLayout_3->addWidget(m_GP_children, 1, 1, 1, 1);

        m_GP_branchLenght = new QDoubleSpinBox(m_growthParticleTab);
        m_GP_branchLenght->setObjectName(QStringLiteral("m_GP_branchLenght"));
        m_GP_branchLenght->setMinimum(1);
        m_GP_branchLenght->setSingleStep(0.5);
        m_GP_branchLenght->setValue(1);

        gridLayout_3->addWidget(m_GP_branchLenght, 2, 1, 1, 1);

        label_5 = new QLabel(m_growthParticleTab);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_3->addWidget(label_5, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 5, 0, 1, 1);

        m_nearestPart = new QCheckBox(m_growthParticleTab);
        m_nearestPart->setObjectName(QStringLiteral("m_nearestPart"));
        sizePolicy2.setHeightForWidth(m_nearestPart->sizePolicy().hasHeightForWidth());
        m_nearestPart->setSizePolicy(sizePolicy2);
        m_nearestPart->setChecked(true);

        gridLayout_3->addWidget(m_nearestPart, 4, 0, 1, 1);

        m_GP_growtoLight = new QCheckBox(m_growthParticleTab);
        m_GP_growtoLight->setObjectName(QStringLiteral("m_GP_growtoLight"));
        m_GP_growtoLight->setChecked(true);

        gridLayout_3->addWidget(m_GP_growtoLight, 3, 0, 1, 1);

        m_particleTab->addTab(m_growthParticleTab, QString());

        verticalLayout_5->addWidget(m_particleTab);

        particlesBox = new QGroupBox(tab);
        particlesBox->setObjectName(QStringLiteral("particlesBox"));
        sizePolicy2.setHeightForWidth(particlesBox->sizePolicy().hasHeightForWidth());
        particlesBox->setSizePolicy(sizePolicy2);
        particlesBox->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(particlesBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(particlesBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        m_particleSize = new QDoubleSpinBox(particlesBox);
        m_particleSize->setObjectName(QStringLiteral("m_particleSize"));
        sizePolicy2.setHeightForWidth(m_particleSize->sizePolicy().hasHeightForWidth());
        m_particleSize->setSizePolicy(sizePolicy2);
        m_particleSize->setMinimum(0.1);
        m_particleSize->setMaximum(100);
        m_particleSize->setValue(2);

        gridLayout->addWidget(m_particleSize, 1, 1, 1, 1);

        m_particleType = new QComboBox(particlesBox);
        m_particleType->setObjectName(QStringLiteral("m_particleType"));
        sizePolicy2.setHeightForWidth(m_particleType->sizePolicy().hasHeightForWidth());
        m_particleType->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(m_particleType, 0, 1, 1, 1);

        label_2 = new QLabel(particlesBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        verticalLayout_5->addWidget(particlesBox);

        m_splitTypeBox = new QGroupBox(tab);
        m_splitTypeBox->setObjectName(QStringLiteral("m_splitTypeBox"));
        m_splitTypeBox->setEnabled(true);
        sizePolicy2.setHeightForWidth(m_splitTypeBox->sizePolicy().hasHeightForWidth());
        m_splitTypeBox->setSizePolicy(sizePolicy2);
        m_splitTypeBox->setMinimumSize(QSize(0, 0));
        m_splitTypeBox->setBaseSize(QSize(0, 7));
        gridLayout_2 = new QGridLayout(m_splitTypeBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        m_splitType = new QComboBox(m_splitTypeBox);
        m_splitType->setObjectName(QStringLiteral("m_splitType"));
        sizePolicy2.setHeightForWidth(m_splitType->sizePolicy().hasHeightForWidth());
        m_splitType->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(m_splitType, 0, 1, 1, 1);

        m_LP_lightOff = new QPushButton(m_splitTypeBox);
        m_LP_lightOff->setObjectName(QStringLiteral("m_LP_lightOff"));
        m_LP_lightOff->setEnabled(false);
        sizePolicy2.setHeightForWidth(m_LP_lightOff->sizePolicy().hasHeightForWidth());
        m_LP_lightOff->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(m_LP_lightOff, 3, 1, 1, 1);

        m_LP_lightOn = new QPushButton(m_splitTypeBox);
        m_LP_lightOn->setObjectName(QStringLiteral("m_LP_lightOn"));
        m_LP_lightOn->setEnabled(false);
        sizePolicy2.setHeightForWidth(m_LP_lightOn->sizePolicy().hasHeightForWidth());
        m_LP_lightOn->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(m_LP_lightOn, 3, 0, 1, 1);

        label_8 = new QLabel(m_splitTypeBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy4);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);


        verticalLayout_5->addWidget(m_splitTypeBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        m_restart = new QPushButton(tab);
        m_restart->setObjectName(QStringLiteral("m_restart"));
        m_restart->setEnabled(true);

        horizontalLayout_3->addWidget(m_restart);

        m_cancel = new QPushButton(tab);
        m_cancel->setObjectName(QStringLiteral("m_cancel"));
        m_cancel->setEnabled(false);

        horizontalLayout_3->addWidget(m_cancel);


        verticalLayout_5->addLayout(horizontalLayout_3);

        tabWidget->addTab(tab, QString());
        particlesBox->raise();
        particlesBox->raise();
        m_particleTab->raise();
        m_splitTypeBox->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        displayBox = new QGroupBox(tab_2);
        displayBox->setObjectName(QStringLiteral("displayBox"));
        displayBox->setEnabled(true);
        sizePolicy1.setHeightForWidth(displayBox->sizePolicy().hasHeightForWidth());
        displayBox->setSizePolicy(sizePolicy1);
        displayBox->setFlat(false);
        displayBox->setCheckable(false);
        gridLayout_4 = new QGridLayout(displayBox);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        m_showConnections = new QCheckBox(displayBox);
        m_showConnections->setObjectName(QStringLiteral("m_showConnections"));
        m_showConnections->setEnabled(true);
        m_showConnections->setCheckable(true);
        m_showConnections->setChecked(true);
        m_showConnections->setTristate(false);

        gridLayout_4->addWidget(m_showConnections, 0, 0, 1, 1);

        m_BColour = new QSlider(displayBox);
        m_BColour->setObjectName(QStringLiteral("m_BColour"));
        m_BColour->setMaximum(255);
        m_BColour->setValue(255);
        m_BColour->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(m_BColour, 4, 1, 1, 1);

        m_shadingType = new QComboBox(displayBox);
        m_shadingType->setObjectName(QStringLiteral("m_shadingType"));

        gridLayout_4->addWidget(m_shadingType, 1, 1, 1, 1);

        label_9 = new QLabel(displayBox);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_4->addWidget(label_9, 1, 0, 1, 1);

        m_GColour = new QSlider(displayBox);
        m_GColour->setObjectName(QStringLiteral("m_GColour"));
        m_GColour->setMaximum(255);
        m_GColour->setValue(255);
        m_GColour->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(m_GColour, 3, 1, 1, 1);

        m_RColour = new QSlider(displayBox);
        m_RColour->setObjectName(QStringLiteral("m_RColour"));
        m_RColour->setMaximum(255);
        m_RColour->setSingleStep(1);
        m_RColour->setPageStep(10);
        m_RColour->setValue(255);
        m_RColour->setSliderPosition(255);
        m_RColour->setTracking(true);
        m_RColour->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(m_RColour, 2, 1, 1, 1);

        label_7 = new QLabel(displayBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_4->addWidget(label_7, 2, 0, 1, 1);

        label_10 = new QLabel(displayBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_4->addWidget(label_10, 3, 0, 1, 1);

        label_11 = new QLabel(displayBox);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_4->addWidget(label_11, 4, 0, 1, 1);


        verticalLayout_3->addWidget(displayBox);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        tabWidget->addTab(tab_2, QString());
        displayBox->raise();
        displayBox->raise();

        verticalLayout_6->addWidget(tabWidget);


        mainLayout->addLayout(verticalLayout_6);


        horizontalLayout_5->addLayout(mainLayout);

        GUI->setCentralWidget(centralwidget);
        tabWidget->raise();
        m_cancel->raise();
        m_restart->raise();

        retranslateUi(GUI);

        tabWidget->setCurrentIndex(0);
        m_particleTab->setCurrentIndex(0);
        m_splitType->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GUI);
    } // setupUi

    void retranslateUi(QMainWindow *GUI)
    {
        GUI->setWindowTitle(QApplication::translate("GUI", "MainWindow", Q_NULLPTR));
        actionfff->setText(QApplication::translate("GUI", "fff", Q_NULLPTR));
        m_LP_forces->setTitle(QApplication::translate("GUI", "Forces", Q_NULLPTR));
        label_3->setText(QApplication::translate("GUI", "Cohesion", Q_NULLPTR));
        label_4->setText(QApplication::translate("GUI", "Local Cohesion", Q_NULLPTR));
        LP_bulge->setText(QApplication::translate("GUI", "Bulge", Q_NULLPTR));
        m_LP_particleDeath->setText(QApplication::translate("GUI", "Particle Death", Q_NULLPTR));
        m_particleTab->setTabText(m_particleTab->indexOf(m_linkedParticleTab), QApplication::translate("GUI", "Linked Particles", Q_NULLPTR));
        label_6->setText(QApplication::translate("GUI", "Branches per particle", Q_NULLPTR));
        label_5->setText(QApplication::translate("GUI", "Branch Length", Q_NULLPTR));
        m_nearestPart->setText(QApplication::translate("GUI", "Nearest Particle to Light", Q_NULLPTR));
        m_GP_growtoLight->setText(QApplication::translate("GUI", "Grow Towards Light", Q_NULLPTR));
        m_particleTab->setTabText(m_particleTab->indexOf(m_growthParticleTab), QApplication::translate("GUI", "Growth Particles", Q_NULLPTR));
        particlesBox->setTitle(QApplication::translate("GUI", "Particle", Q_NULLPTR));
        label->setText(QApplication::translate("GUI", "Particle Type", Q_NULLPTR));
        m_particleType->clear();
        m_particleType->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Linked Particles", Q_NULLPTR)
         << QApplication::translate("GUI", "Growth Particles", Q_NULLPTR)
        );
        label_2->setText(QApplication::translate("GUI", "Particle Size", Q_NULLPTR));
        m_splitTypeBox->setTitle(QApplication::translate("GUI", "Splitting", Q_NULLPTR));
        m_splitType->clear();
        m_splitType->insertItems(0, QStringList()
         << QApplication::translate("GUI", "Controlled by Space Bar", Q_NULLPTR)
         << QApplication::translate("GUI", "Controlled by Light On/Off", Q_NULLPTR)
        );
        m_LP_lightOff->setText(QApplication::translate("GUI", "Light Off", Q_NULLPTR));
        m_LP_lightOn->setText(QApplication::translate("GUI", "Light On", Q_NULLPTR));
        label_8->setText(QApplication::translate("GUI", "Split Type", Q_NULLPTR));
        m_restart->setText(QApplication::translate("GUI", "Restart", Q_NULLPTR));
        m_cancel->setText(QApplication::translate("GUI", "Cancel", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("GUI", "Particles", Q_NULLPTR));
        displayBox->setTitle(QApplication::translate("GUI", "Display", Q_NULLPTR));
        m_showConnections->setText(QApplication::translate("GUI", "Show connections", Q_NULLPTR));
        m_shadingType->clear();
        m_shadingType->insertItems(0, QStringList()
         << QApplication::translate("GUI", "ADS", Q_NULLPTR)
         << QApplication::translate("GUI", "X Ray", Q_NULLPTR)
         << QApplication::translate("GUI", "Ambient Occlusion", Q_NULLPTR)
        );
        m_shadingType->setCurrentText(QApplication::translate("GUI", "ADS", Q_NULLPTR));
        label_9->setText(QApplication::translate("GUI", "Type of Shading", Q_NULLPTR));
        label_7->setText(QApplication::translate("GUI", "Red", Q_NULLPTR));
        label_10->setText(QApplication::translate("GUI", "Blue", Q_NULLPTR));
        label_11->setText(QApplication::translate("GUI", "Green", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("GUI", "Display", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GUI: public Ui_GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
