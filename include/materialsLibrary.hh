#ifndef materialsLibrary_hh
#define materialsLibrary_hh 1

#include "G4Material.hh"

namespace materialsLibrary
{
  ////////////////
  // Scintillators

  G4Material *EJ301();
  //G4Material *EJ309();

  //////////////////////
  // Shielding materials

  G4Material *BoratedHDPE();
  G4Material *Aluminum();
  
  // Optical materials
  G4Material *SiPM(G4String);
}

#endif
