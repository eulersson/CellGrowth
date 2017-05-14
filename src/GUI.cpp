#include "include/GUI.h"
#include "ui_GUI.h"

GUI::GUI(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::GUI)
{

  /*Setting colour to white to match the slides.
   Othervise it will not correctly display light colour unless R, G and B slide is moved.*/
  m_colour_light.setRgb(255, 255, 255);
  m_colour_material.setRgb(255, 255, 255);

  m_ui->setupUi(this);
  m_gl = new GLWindow(this);
  m_gl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_ui->mainLayout->insertWidget(0, m_gl);
  setWindowTitle("Cell Growth");

  connect(m_ui->m_particleSize,SIGNAL(valueChanged(double)),m_gl,SLOT(setParticleSize(double)));
  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setParticleType(int)));
  connect(m_ui->m_showConnections,SIGNAL(toggled(bool)),m_gl,SLOT(showConnections(bool)));
  connect(m_ui->m_shadingType,SIGNAL(currentIndexChanged(QString)),m_gl,SLOT(setShading(QString)));
  connect(m_ui->m_LP_forces,SIGNAL(toggled(bool)),m_gl,SLOT(toggleForces(bool)));
  connect(m_ui->m_LP_particleDeath,SIGNAL(toggled(bool)),m_gl,SLOT(toggleParticleDeath(bool)));
  connect(m_ui->m_LP_cohesion,SIGNAL(valueChanged(int)),m_gl,SLOT(setCohesion(int)));
  connect(m_ui->LP_bulge,SIGNAL(released()),m_gl,SLOT(bulge()));
  connect(m_ui->m_LP_lightOn,SIGNAL(released()),m_gl,SLOT(lightOn()));
  connect(m_ui->m_LP_lightOff,SIGNAL(released()),m_gl,SLOT(lightOff()));
  connect(m_ui->m_LP_localCohesion,SIGNAL(valueChanged(int)),m_gl,SLOT(setLocalCohesion(int)));
  connect(m_ui->m_AP_radius,SIGNAL(valueChanged(int)),m_gl,SLOT(setAutomataRadius(int)));
  connect(m_ui->m_AP_time,SIGNAL(valueChanged(int)),m_gl,SLOT(setAutomataTime(int)));
  connect(m_ui->m_GP_children,SIGNAL(valueChanged(int)),m_gl,SLOT(setChildThreshold(int)));
  connect(m_ui->m_restart,SIGNAL(pressed()),m_gl,SLOT(restart()));
  connect(m_ui->m_splitType,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setSplitType(int)));
  connect(m_ui->m_GP_branchLength,SIGNAL(valueChanged(double)),m_gl,SLOT(setBranchLength(double)));
  connect(m_ui->m_nearestPart,SIGNAL(clicked(bool)),m_gl,SLOT(setNearestParticle(bool)));
  connect(m_ui->m_GP_growtoLight,SIGNAL(clicked(bool)),m_gl,SLOT(setGrowToLight(bool)));
  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_ui->m_particleTab,SLOT(setCurrentIndex(int)));

  //Setting the RGB values inputted by the user on either slides or spinbox.
  connect(m_ui->m_RColour,SIGNAL(valueChanged(int)),m_gl,SLOT(setRcolour(int)));
  connect(m_ui->m_GColour,SIGNAL(valueChanged(int)),m_gl,SLOT(setGcolour(int)));
  connect(m_ui->m_BColour,SIGNAL(valueChanged(int)),m_gl,SLOT(setBcolour(int)));
  connect(m_ui->m_RColourSpinBox, SIGNAL(valueChanged(int)),SLOT(onRedLight(int)));
  connect(m_ui->m_GColourSpinBox, SIGNAL(valueChanged(int)),SLOT(onGreenLight(int)));
  connect(m_ui->m_BColourSpinBox, SIGNAL(valueChanged(int)),SLOT(onBlueLight(int)));

  connect(m_ui->m_lightAmbient,SIGNAL(valueChanged(int)),m_gl,SLOT(setAmbientLight(int)));
  connect(m_ui->m_specularLight,SIGNAL(valueChanged(int)),m_gl,SLOT(setSpecularLight(int)));
  connect(m_ui->m_FillLight,SIGNAL(valueChanged(int)),m_gl,SLOT(setFillLight(int)));


  //Materials
  connect(m_ui->m_MaterialRColour,SIGNAL(valueChanged(int)),m_gl,SLOT(setRcolourMaterial(int)));
  connect(m_ui->m_MaterialGColour,SIGNAL(valueChanged(int)),m_gl,SLOT(setGcolourMaterial(int)));
  connect(m_ui->m_MaterialBColour,SIGNAL(valueChanged(int)),m_gl,SLOT(setBcolourMaterial(int)));
  connect(m_ui->m_materialRSpinBox, SIGNAL(valueChanged(int)),SLOT(onRedMaterial(int)));
  connect(m_ui->m_materialGSpinBox, SIGNAL(valueChanged(int)),SLOT(onGreenMaterial(int)));
  connect(m_ui->m_materialBSpinBox, SIGNAL(valueChanged(int)),SLOT(onBlueMaterial(int)));

  //AO Variables
  connect(m_ui->m_RadiusSB,SIGNAL(valueChanged(double)),m_gl,SLOT(setSSAORadius(double)));
  connect(m_ui->m_BiasSB,SIGNAL(valueChanged(double)),m_gl,SLOT(setSSAOBias(double)));

  connect(m_gl,SIGNAL(resetBranchLength(double)),m_ui->m_GP_branchLength,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetChildrenThreshold(int)),m_ui->m_GP_children,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetCohesion(int)),m_ui->m_LP_cohesion,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetForces(bool)),m_ui->m_LP_forces,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetParticleDeath(bool)),m_ui->m_LP_particleDeath,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetParticleSize(double)),m_ui->m_particleSize,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetParticleTab(int)),m_ui->m_particleTab,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetParticleType(int)),m_ui->m_particleType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetSplitType(int)),m_ui->m_splitType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetLocalCohesion(int)),m_ui->m_LP_localCohesion,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(enableGrowthParticle(bool)),m_ui->m_growthParticleTab,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLinkedParticle(bool)),m_ui->m_linkedParticleTab,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableAutomataParticle(bool)),m_ui->m_automataParticleTab,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableSplitType(bool)),m_ui->m_splitTypeBox,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(changedShadingType(int)),m_ui->m_shadingType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(setConnectionState(bool)),m_ui->m_showConnections,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(enableBulge(bool)),m_ui->LP_bulge,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLightOn(bool)),m_ui->m_LP_lightOn,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLightOff(bool)),m_ui->m_LP_lightOff,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(resetNearestParticle(bool)),m_ui->m_nearestPart,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetGrowToLight(bool)),m_ui->m_GP_growtoLight,SLOT(setChecked(bool)));

  //Setting the RGB value for the light diffuse.
  connect(m_gl,SIGNAL(resetRColour(int)),m_ui->m_RColour,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetGColour(int)),m_ui->m_GColour,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetBColour(int)),m_ui->m_BColour,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetRColour(int)),m_ui->m_MaterialRColour,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetGColour(int)),m_ui->m_MaterialGColour,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetBColour(int)),m_ui->m_MaterialBColour,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetAmbientLight(int)),m_ui->m_lightAmbient,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetSpecularLight(int)),m_ui->m_specularLight,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetFillLight(int)),m_ui->m_FillLight,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(enableConnections(bool)),m_ui->m_showConnections,SLOT(setEnabled(bool)));

  //AO Variables
  connect(m_gl,SIGNAL(resetAORadius(double)),m_ui->m_RadiusSB,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetAOBias(double)),m_ui->m_BiasSB,SLOT(setValue(double)));

  // ===========================================================================
  // Help status bar
  // ===========================================================================
  QString str_particle_type =
    "Different algorithms have been coded show different behaviours. This option"
    "swaps between them.";
  QString str_particle_size =
    "Determines the radius of the particle.";
  QString str_particle_death =
    "Forces stop working after a time threshold.";
  QString str_cohesion =
    "How tight or loose the overall system is";
  QString str_local_cohesion =
    "How strong or weak are the links between particles that are directly connected";
  QString str_bulge =
    "A repulsion force is triggered.";
  QString str_split_type =
    "There are two modes of controlling how particles split, one is by pressing"
    "spacebar key and the other one is by toggling the splitting state, in other"
    "words by turning on or off the light";
  QString str_light_on =
    "For this option to work the splitting type has to be set to be controlled by"
    "light. It toggles the splitting state.";
  QString str_light_off =
    "For this option to work the splitting type has to be set to be controlled by"
    "light. It toggles the splitting state.";
  QString str_GP_branches =
    "How many branches per particle.";
  QString str_GP_branch_length =
    "Length of a branch.";
  QString str_grow_to_light =
    "Whether or not the light should be taken into account";
  QString str_nearest_particle =
    "Toggles either splitting randomly a particle or the newest.";
  QString str_AP_radius =
    "Cellular automata cell radius.";
  QString str_AP_time =
    "Cellular automata cell time step.";
  QString str_restart =
    "Resets the particle system.";

  // Particles tab
  m_ui->label_particle_type->setToolTip(str_particle_type);
  m_ui->label_particle_size->setToolTip(str_particle_size);
  m_ui->m_LP_particleDeath->setToolTip(str_particle_death);
  m_ui->m_LP_cohesion->setToolTip(str_cohesion);
  m_ui->m_LP_localCohesion->setToolTip(str_local_cohesion);
  m_ui->LP_bulge->setToolTip(str_bulge);
  m_ui->label_split_type->setToolTip(str_split_type);
  m_ui->m_LP_lightOn->setToolTip(str_light_on);
  m_ui->m_LP_lightOff->setToolTip(str_light_off);
  m_ui->label_GP_branches->setToolTip(str_GP_branches);
  m_ui->label_GP_branch_length->setToolTip(str_GP_branch_length);
  m_ui->m_GP_growtoLight->setToolTip(str_grow_to_light);
  m_ui->m_nearestPart->setToolTip(str_nearest_particle);
  m_ui->label_AP_radius->setToolTip(str_AP_radius);
  m_ui->label_AP_time->setToolTip(str_AP_time);
  m_ui->m_restart->setToolTip(str_restart);

  // Display tab
  m_ui->m_showConnections->setToolTip("Show connections.");
  m_ui->label_shading_type->setToolTip("Shading type.");
  m_ui->label_light_color_r->setToolTip("Red component of the light colour.");
  m_ui->label_light_color_g->setToolTip("Green component of the light colour.");
  m_ui->label_light_color_b->setToolTip("Blue component of the light colour.");
  m_ui->label_ambient->setToolTip("Ambient contribution.");
  m_ui->label_specular->setToolTip("Specular contribution.");
  m_ui->label_fill_light_impact->setToolTip("Fill light impact.");
  m_ui->label_material_color_r->setToolTip("Red component of the material color.");
  m_ui->label_material_color_g->setToolTip("Green component of the material color.");
  m_ui->label_material_color_b->setToolTip("Blue component of the material color.");
  m_ui->label_AO_radius->setToolTip("How far to check for occluded regions.");
  m_ui->label_AO_bias->setToolTip("Acts like a fit range on the occlusion factor.");
}

GUI::~GUI()
{
  delete m_gl;
  delete m_ui;
}


void GUI::onRedLight(int _red)
{
        m_colour_light.setRed(_red);
        m_ui->m_colourDisplay->setPalette(QPalette(m_colour_light));
}
void GUI::onGreenLight(int _green)
{
    m_colour_light.setGreen(_green);
    m_ui->m_colourDisplay->setPalette(QPalette(m_colour_light));
}

void GUI::onBlueLight(int _blue)
{
    m_colour_light.setBlue(_blue);
    m_ui->m_colourDisplay->setPalette(QPalette(m_colour_light));
}

void GUI::onRedMaterial(int _r)
{
    m_colour_material.setRed(_r);
    m_ui->m_colourDisplayMaterial->setPalette(QPalette(m_colour_material));
}

void GUI::onGreenMaterial(int _g)
{
    m_colour_material.setGreen(_g);
    m_ui->m_colourDisplayMaterial->setPalette(QPalette(m_colour_material));
}

void GUI::onBlueMaterial(int _b)
{
    m_colour_material.setBlue(_b);
    m_ui->m_colourDisplayMaterial->setPalette(QPalette(m_colour_material));
}
