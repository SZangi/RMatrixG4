#include "PhysicsList.hh"

PhysicsList::PhysicsList() 
{
  thePhysicsList = new QGSP_BIC_HP(0);

  theOpticalPhysics = new G4OpticalPhysics(0);
  auto theOpticalParameters = G4OpticalParameters::Instance();
  theOpticalParameters->SetScintByParticleType(true);
  thePhysicsList->RegisterPhysics(theOpticalPhysics);
}


PhysicsList::~PhysicsList()
{
  delete thePhysicsList;
  delete theOpticalPhysics;
}
