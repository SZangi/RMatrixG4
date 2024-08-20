#include "G4NistManager.hh"

#include "G4MaterialsManager.hh"


G4MaterialsManager *G4MaterialsManager::theMaterialsManager = 0;


G4MaterialsManager *G4MaterialsManager::GetInstance()
{ return theMaterialsManager; }


G4MaterialsManager::G4MaterialsManager()
{
  if(!theMaterialsManager)
    theMaterialsManager = this;
  theMaterialsBuilder = new G4MaterialsBuilder();
}


G4MaterialsManager::~G4MaterialsManager()
{
  delete theMaterialsManager;
  delete theMaterialsBuilder;
}
