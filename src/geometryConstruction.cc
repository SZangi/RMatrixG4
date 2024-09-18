#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"

#include "geometryConstruction.hh"
#include "G4MaterialsManager.hh"

geometryConstruction::geometryConstruction()
{;}

geometryConstruction::~geometryConstruction()
{;}


G4VPhysicalVolume *geometryConstruction::Construct()
{
  ////////////////////////
  // G4MaterialsManager //
  ////////////////////////

  G4MaterialsManager *theMaterialsManager = new G4MaterialsManager;
  
  ///////////////
  // The World //
  ///////////////

  G4double worldX = 10*cm;
  G4double worldY = 10*cm;
  G4double worldZ = 20*cm;
  
  G4Box *world_S = new G4Box("world_S",worldX,worldY,worldZ);

  G4LogicalVolume *world_L = new G4LogicalVolume(world_S,
						 G4MaterialsManager::GetInstance()->GetNISTMaterial("G4_AIR"),
						 "world_L");
  
  G4VPhysicalVolume *world_P = new G4PVPlacement(0, 
						 G4ThreeVector(), 
						 world_L,
						 "world_P",
						 0,
						 false,
						 0);
  
  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour());
  worldVisAtt->SetVisibility(1);
  world_L->SetVisAttributes(worldVisAtt);

  
  ///////////////
  // The Block //
  ///////////////
  G4double Scint_rMin = 0.*cm;
  G4double Scint_rMax = 0.5*2.54*cm;
  G4double Scint_z = 0.5*2.54*cm;
  G4double Scint_sPhi = 0;
  G4double Scint_dPhi = 2*pi;

  G4double Scint_posX = 0.*cm;
  G4double Scint_posY = 0.*cm;
  G4double Scint_posZ = -10 *cm;
  
  G4Tubs *block_S = new G4Tubs("block_S",
		       Scint_rMin,
		       Scint_rMax,
		       Scint_z,
		       Scint_sPhi,
		       Scint_dPhi);
  
  // In this example we use the newly created G4MaterialsManager to handle 
  // creation and implementation of optical properties for various pre-defined
  // optical materials. 

  // Further documentation on how G4MaterialsManager works can be seen in the
  // MaterialsManager header files.

  G4LogicalVolume *block_L = new G4LogicalVolume(block_S,
						 G4MaterialsManager::GetInstance()->GetOpticalMaterial("EJ301"),
						 "block_L");
  
  G4VPhysicalVolume *block_P = new G4PVPlacement(new G4RotationMatrix(),
						 G4ThreeVector(0., 0., -10.*cm),
						 block_L,
						 "block_P",
						 world_L,
						 false,
						 0);
  
  G4VisAttributes *blockVisAtt = new G4VisAttributes(G4Colour(0., 0., 1., 1.));
  blockVisAtt->SetForceSolid(1);
  block_L->SetVisAttributes(blockVisAtt);

  //G4cout << "ScintMaterial Properties \n" << G4endl;
  //G4Material *ScintMat = block_L->GetMaterial();
  //G4MaterialPropertiesTable *Material_PT =  ScintMat->GetMaterialPropertiesTable();
  //Material_PT->DumpTable();

  return world_P;
}