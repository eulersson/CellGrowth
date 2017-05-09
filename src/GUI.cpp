#include "include/GUI.h"
#include "ui_GUI.h"

GUI::GUI(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::GUI)
{
  m_ui->setupUi(this);
  m_gl = new GLWindow(this);
  m_ui->MainWindow_gridLayout->addWidget(m_gl,0,0,1,1);

  connect(m_ui->m_particleSize,SIGNAL(valueChanged(double)),m_gl,SLOT(setParticleSize(double)));
  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setParticleType(int)));
  connect(m_ui->m_cancel,SIGNAL(pressed()),m_gl,SLOT(cancel()));
  connect(m_ui->m_showConnections,SIGNAL(toggled(bool)),m_gl,SLOT(showConnections(bool)));
  connect(m_ui->m_shadingType,SIGNAL(currentIndexChanged(QString)),m_gl,SLOT(setShading(QString)));
  connect(m_ui->m_LP_forces,SIGNAL(toggled(bool)),m_gl,SLOT(toggleForces(bool)));
  connect(m_ui->m_LP_particleDeath,SIGNAL(toggled(bool)),m_gl,SLOT(toggleParticleDeath(bool)));
  connect(m_ui->m_LP_cohesion,SIGNAL(valueChanged(int)),m_gl,SLOT(setCohesion(int)));
  connect(m_ui->LP_bulge,SIGNAL(released()),m_gl,SLOT(bulge()));
  connect(m_ui->m_LP_lightOn,SIGNAL(released()),m_gl,SLOT(lightOn()));
  connect(m_ui->m_LP_lightOff,SIGNAL(released()),m_gl,SLOT(lightOff()));
  connect(m_ui->m_LP_localCohesion,SIGNAL(valueChanged(int)),m_gl,SLOT(setLocalCohesion(int)));
  connect(m_ui->m_GP_children,SIGNAL(valueChanged(int)),m_gl,SLOT(setChildThreshold(int)));
  connect(m_ui->m_restart,SIGNAL(pressed()),m_gl,SLOT(restart()));
  connect(m_ui->m_splitType,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setSplitType(int)));
  connect(m_ui->m_GP_branchLenght,SIGNAL(valueChanged(double)),m_gl,SLOT(setBranchLength(double)));
  connect(m_ui->m_nearestPart,SIGNAL(clicked(bool)),m_gl,SLOT(setNearestParticle(bool)));
  connect(m_ui->m_GP_growtoLight,SIGNAL(clicked(bool)),m_gl,SLOT(setGrowToLight(bool)));

  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_ui->m_particleTab,SLOT(setCurrentIndex(int)));


  connect(m_gl,SIGNAL(resetBranchLength(double)),m_ui->m_GP_branchLenght,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetChildrenThreshold(int)),m_ui->m_GP_children,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetCohesion(int)),m_ui->m_LP_cohesion,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetForces(bool)),m_ui->m_LP_forces,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetParticleDeath(bool)),m_ui->m_LP_particleDeath,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetParticleSize(double)),m_ui->m_particleSize,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetParticleTap(int)),m_ui->m_particleTab,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetParticleType(int)),m_ui->m_particleType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetSplitType(int)),m_ui->m_splitType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetLocalCohesion(int)),m_ui->m_LP_localCohesion,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(enableGrowthParticle(bool)),m_ui->m_growthParticleTab,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLinkedParticle(bool)),m_ui->m_linkedParticleTab,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableSplitType(bool)),m_ui->m_splitTypeBox,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(changedShadingType(int)),m_ui->m_shadingType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(setConnectionState(bool)),m_ui->m_showConnections,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(enableBulge(bool)),m_ui->LP_bulge,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLightOn(bool)),m_ui->m_LP_lightOn,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLightOff(bool)),m_ui->m_LP_lightOff,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(resetNearestParticle(bool)),m_ui->m_nearestPart,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetGrowToLight(bool)),m_ui->m_GP_growtoLight,SLOT(setChecked(bool)));
}

GUI::~GUI()
{
  delete m_gl;
  delete m_ui;
}
