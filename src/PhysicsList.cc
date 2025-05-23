#include "PhysicsList.hh"
#include "G4StepLimiterPhysics.hh"

PhysicsList::PhysicsList() 
{
  thePhysicsList = new QGSP_BIC_HP(0);

  theOpticalPhysics = new G4OpticalPhysics(0);
  auto theOpticalParameters = G4OpticalParameters::Instance();
  theOpticalParameters->SetScintByParticleType(true);
  thePhysicsList->RegisterPhysics(theOpticalPhysics);
  thePhysicsList->RegisterPhysics(new G4StepLimiterPhysics());
}


PhysicsList::~PhysicsList()
{
  delete thePhysicsList;
  delete theOpticalPhysics;
}
