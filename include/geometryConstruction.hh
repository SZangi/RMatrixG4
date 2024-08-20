#ifndef __geometryConstruction_hh__
#define __geometryConstruction_hh__ 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"


class geometryConstruction : public G4VUserDetectorConstruction
{
public:
  geometryConstruction();
  ~geometryConstruction();
  
  // Main function
  G4VPhysicalVolume *Construct();
};

#endif
