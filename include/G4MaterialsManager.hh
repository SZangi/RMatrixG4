#ifndef G4MATERIALSMANAGER_HH
#define G4MATERIALSMANAGER_HH

#include "G4Material.hh"

#include "G4MaterialsBuilder.hh"

///////////////////////
// Class declaration //
///////////////////////

class G4MaterialsManager{
  
public:
  G4MaterialsManager();
  ~G4MaterialsManager();

  static G4MaterialsManager *GetInstance();

  inline G4Material *GetStandardMaterial(G4String) const;
  inline G4Material *GetNISTMaterial(G4String) const;
  inline G4Material *GetPNNLMaterial(G4String) const;
  inline G4Material *GetOpticalMaterial(G4String) const;
  
private:
  static G4MaterialsManager *theMaterialsManager;
  
  G4MaterialsBuilder *theMaterialsBuilder;
};


//////////////////////
// Inline functions //
//////////////////////

inline G4Material *G4MaterialsManager::GetStandardMaterial(G4String name) const
{
  return theMaterialsBuilder->FindOrBuildStandardMaterial(name);
}


inline G4Material *G4MaterialsManager::GetNISTMaterial(G4String name) const
{
  return theMaterialsBuilder->FindOrBuildNISTMaterial(name);
}


inline G4Material *G4MaterialsManager::GetPNNLMaterial(G4String name) const
{
  return theMaterialsBuilder->FindOrBuildPNNLMaterial(name);
}


inline G4Material *G4MaterialsManager::GetOpticalMaterial(G4String name) const
{
  return theMaterialsBuilder->FindOrBuildOpticalMaterial(name);
}
  
#endif
