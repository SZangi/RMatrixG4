#include "PhysicsList.hh"

PhysicsList::PhysicsList() 
{
  thePhysicsList = new QGSP_BIC_HP;

  theOpticalPhysics = new G4OpticalPhysics;
  auto theOpticalParameters = G4OpticalParameters::Instance();
  theOpticalParameters->SetScintByParticleType(true);
  thePhysicsList->RegisterPhysics(theOpticalPhysics);
}


PhysicsList::~PhysicsList()
{
  delete thePhysicsList;
  delete theOpticalPhysics;
}
