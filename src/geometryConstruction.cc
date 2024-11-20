#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4SDManager.hh"

#include "geometryConstruction.hh"
#include "G4MaterialsManager.hh"
#include "ASIMPhotodetectorSD.hh"

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

  G4MaterialPropertiesTable * WorldMPT = new G4MaterialPropertiesTable();
  WorldMPT->AddProperty("RINDEX",{2.36*eV,4.01*eV},{0.1,0.1},2);
  WorldMPT->AddProperty("ABSLENGTH",{2.36*eV,4.01*eV},{1000000,3000000},2);

  G4Material *WorldMat = G4MaterialsManager::GetInstance()->GetNISTMaterial("G4_AIR");
  WorldMat->SetMaterialPropertiesTable(WorldMPT);

  G4LogicalVolume *world_L = new G4LogicalVolume(world_S,
						 WorldMat,
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
  G4double Scint_rMax = 2.54*cm;
  G4double Scint_z = 2.5*cm;
  G4double Scint_sPhi = 0;
  G4double Scint_dPhi = 2*pi;

  G4double Scint_posX = 0.*cm;
  G4double Scint_posY = 0.*cm;
  G4double Scint_posZ = -10 *cm;

  const G4String Scint_name = "EJ301";

  G4Material *Scint_Mat = G4MaterialsManager::GetInstance()->GetOpticalMaterial(Scint_name);

  G4Material *PMT_Mat = G4MaterialsManager::GetInstance()->GetNISTMaterial("G4_GLASS_PLATE");

  std::vector<G4double> glass_AbsLength = { 420. * cm, 420. * cm, 420. * cm };
  G4MaterialPropertiesTable* glass_mt   = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH", { 2.0 * eV, 3.07 * eV, 4.14 * eV }, glass_AbsLength);
  glass_mt->AddProperty("RINDEX", "Fused Silica");

  PMT_Mat->SetMaterialPropertiesTable(glass_mt);

  G4int No_Pancakes = 3;
  G4int No_Rings = 3;
  
  G4Tubs *block_S = new G4Tubs("block_S",
		       Scint_rMin,
		       Scint_rMax,
		       Scint_z,
		       Scint_sPhi, // start phi
		       Scint_dPhi); // delta phi
  
  // In this example we use the newly created G4MaterialsManager to handle 
  // creation and implementation of optical properties for various pre-defined
  // optical materials. 

  // Further documentation on how G4MaterialsManager works can be seen in the
  // MaterialsManager header files.

  G4LogicalVolume *block_L = new G4LogicalVolume(block_S,
						 Scint_Mat,
						 "block_L");
  
  G4VPhysicalVolume *block_P = new G4PVPlacement(new G4RotationMatrix(),
						 G4ThreeVector(Scint_posX,Scint_posY,Scint_posZ),
						 block_L,
						 "block_P",
						 world_L,
						 false,
						 0);


  // This should... define the optical boundary of the casing between
  // the detector and the world volume
  G4OpticalSurface* Scint_Wrap = new G4OpticalSurface("ScintWrap");

  Scint_Wrap->SetType(dielectric_metal);
  Scint_Wrap->SetModel(unified);
  Scint_Wrap->SetFinish(polished);

  const G4int num_points = 2;
  G4double pp1[num_points] = {2.038*eV, 4.144*eV};

  G4double specularlobe[num_points] = {0.3, 0.3};
  G4double specularspike[num_points] = {0.2, 0.2};
  G4double backscatter[num_points] = {0.5, 0.5};
  G4double rindex[num_points] = {1.35, 1.40};

  G4double reflectivity[num_points] = {1.0, 1.0};



  G4MaterialPropertiesTable *wrapSurfaceProperty = new G4MaterialPropertiesTable();

  wrapSurfaceProperty -> AddProperty("RINDEX",pp1, rindex, num_points);
  wrapSurfaceProperty ->AddProperty("SPECULARLOBECONSTANT",pp1,specularlobe,num_points);
  wrapSurfaceProperty -> AddProperty("SPECULARSPIKECONSTANT",pp1,specularspike,num_points);
  wrapSurfaceProperty -> AddProperty("BACKSCATTERCONSTANT",pp1,backscatter,num_points);

  wrapSurfaceProperty->AddProperty("REFLECTIVITY",pp1,reflectivity,num_points);
  Scint_Wrap->SetMaterialPropertiesTable(wrapSurfaceProperty);

  new G4LogicalBorderSurface("Scint_Wrap", block_P,world_P,Scint_Wrap);

  // Parameterizing the geometry whoot whoot!
  // The mesh must be formed uniquely for each detector geometry, so this one 
  // is set up for a cylindrical detector.
  // Our "voxcel mesh" consists of 3 evenly sized pancake slices through the
  // cylinder, each then divided into 3 concentric rings. May increase these 
  // numbers as the simulation gets more advanced. Concentric rings for a
  // cylinder is used as there is not likely to be much axial variance in the
  // arrival time distribution. 

  G4double pancake_Z = Scint_z/No_Pancakes;

  // first we break it into pancakes
  G4Tubs *pancake_S = new G4Tubs("pancake_S",
		       Scint_rMin,
		       Scint_rMax,
		       pancake_Z, // this argument is the half width
		       Scint_sPhi,
		       Scint_dPhi);
  G4LogicalVolume *pancake_L = new G4LogicalVolume(pancake_S,
						 Scint_Mat,
						 "pancake_L");
             // here the last argument is the full width
  new G4PVReplica("DetectorPancakePhysical",pancake_L,block_L,kZAxis,No_Pancakes,pancake_Z*2);

  // now we create the rings within the pancake
  G4Tubs *rings_S = new G4Tubs("ring_S",
           Scint_rMin,
		       Scint_rMax/No_Rings,
		       pancake_Z,
		       Scint_sPhi,
		       Scint_dPhi);
  G4LogicalVolume *rings_L = new G4LogicalVolume(rings_S,
              Scint_Mat,
              "rings_L");
  new G4PVReplica("DetectorRingsPhysical",rings_L,pancake_L,kRho,No_Rings,Scint_rMax/No_Rings,0);

  G4double pmt_Z = 2.5*cm;

  G4Tubs *PMT_S = new G4Tubs("pmt_S",
            Scint_rMin,
            Scint_rMax,
            pmt_Z,
            Scint_sPhi,
            Scint_dPhi);
  
  G4LogicalVolume *PMT_L = new G4LogicalVolume(PMT_S,
            PMT_Mat,
            "pmt_L");
  
  new G4PVPlacement(new G4RotationMatrix(),     // rotation matrix
              G4ThreeVector(0., 0., -(-Scint_posZ+Scint_z+pmt_Z)), // Placement
              PMT_L, // Logical volume
              "PMT_p", // name
              world_L, // mother volume
              false,   // replication axis
              0);      // copy number

  // Attach Sensitive Detector to the PMT volume

  pmt_SD = new ASIMPhotodetectorSD("pmt");
  pmt_SD->SetHitRGBA(1.0, 1.0, 0.0, 0.5);
  pmt_SD->SetHitSize(8);
  
  // Tell the sensitive detector manager about the detector
  G4SDManager::GetSDMpointer()->AddNewDetector(pmt_SD);
  
  // assign the detector to the PMT volume
  PMT_L->SetSensitiveDetector(pmt_SD);


  G4Tubs *PhotoCath_S = new G4Tubs("photocath_S",
            Scint_rMin,
            Scint_rMax,
            0.5*cm,
            Scint_sPhi,
            Scint_dPhi);

  G4LogicalVolume *PhotoCath_L = new G4LogicalVolume(PhotoCath_S,
          G4MaterialsManager::GetInstance()->GetNISTMaterial("G4_Al"),
          "photocath_L");

  new G4PVPlacement(new G4RotationMatrix(),
            G4ThreeVector(0.,0.,-(-Scint_posZ+Scint_z+pmt_Z*2+0.5*cm)),
            PhotoCath_L,
            "photocath_P",
            world_L,
            false,
            0);
  
  
  G4VisAttributes *blockVisAtt = new G4VisAttributes(G4Colour(0., 0., 1., 1.));
  blockVisAtt->SetForceWireframe(1);
  block_L->SetVisAttributes(blockVisAtt);

  G4VisAttributes *segmentVisAtt = new G4VisAttributes(G4Color(0.,0.5,0.5,0.5));
  segmentVisAtt->SetForceSolid(1);
  pancake_L->SetVisAttributes(segmentVisAtt);

  G4VisAttributes *pmtVisAtt = new G4VisAttributes(G4Color(0.,1.0,0.0,0.2));
  pmtVisAtt->SetForceSolid(1);
  PMT_L->SetVisAttributes(pmtVisAtt);

  G4VisAttributes *photocathVisAtt = new G4VisAttributes(G4Color(0.5,0.5,0,0.7));
  photocathVisAtt->SetForceSolid(1);
  PhotoCath_L->SetVisAttributes(photocathVisAtt);

  //G4cout << "ScintMaterial Properties \n" << G4endl;
  //G4Material *ScintMat = block_L->GetMaterial();
  //G4MaterialPropertiesTable *Material_PT =  ScintMat->GetMaterialPropertiesTable();
  //Material_PT->DumpTable();

  return world_P;
}