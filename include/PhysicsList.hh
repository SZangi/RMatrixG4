#ifndef PhysicsList_hh
#define PhysicsList_hh 1

#include "QGSP_BIC.hh"
#include "QGSP_BIC_HP.hh"
#include "G4OpticalPhysics.hh"
#include "G4VModularPhysicsList.hh"

class PhysicsList
{
public:

  PhysicsList();
  ~PhysicsList();
  
  G4VModularPhysicsList *GetPhysicsList(){return thePhysicsList;}
  G4OpticalPhysics *GetOpticalPhysits(){return theOpticalPhysics;}
  
private:
  G4VModularPhysicsList *thePhysicsList;
  G4OpticalPhysics *theOpticalPhysics;
  
};

#endif