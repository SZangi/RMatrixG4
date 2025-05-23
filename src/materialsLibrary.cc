#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4SystemOfUnits.hh"
#include "cmath"

#include "materialsLibrary.hh"
//#include "CsIOpticalData.hh"
//#include "EJ276OpticalData.hh"
#include "EJ301OpticalData.hh"
//#include "EJ309OpticalData.hh"
//#include "LaBr3OpticalData.hh"
//#include "NaIOpticalData.hh"
//#include "SrI2OpticalData.hh"

//using namespace CsI;
//using namespace EJ276;
using namespace EJ301;
//using namespace EJ309;
//using namespace LaBr3;
//using namespace NaI;
//using namespace SrI2;

G4Material *materialsLibrary::SiPM(G4String SiPMType)
{
  
  static G4bool init = true;
  static G4Material *SiPM = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  static G4MaterialPropertiesTable *SiPM_MPT = new G4MaterialPropertiesTable();
  
  if(init){

    // https://www.hamamatsu.com/content/dam/hamamatsu-photonics/sites/documents/99_SALES_LIBRARY/ssd/s14160_s14161_series_kapd1064e.pdf
    
    if(SiPMType=="S14160"){
      
      // Optical photon detection (PD) wavelength [nm]
      std::vector<G4double> PDWavelength = {283.4,
					    290.7,
					    292.3,
					    294.4,
					    296.6,
					    299.7,
					    303.9,
					    310.9,
					    330.1,
					    350.0,
					    374.6,
					    384.0,
					    396.9,
					    408.1,
					    424.8,
					    442.2,
					    459.7,
					    475.6,
					    487.4,
					    497.5,
					    521.4,
					    540.0,
					    558.6,
					    579.2,
					    609.0,
					    633.8,
					    656.0,
					    682.1,
					    709.5,
					    743.1,
					    778.0,
					    812.9,
					    847.7,
					    882.6};
      
      // SiPM photon detection (PD) efficiency [%]
      std::vector<G4double> PDEfficiency = {3.2,
					    7.7,
					    13.1,
					    17.3,
					    21.9,
					    25.7,
					    29.2,
					    32.8,
					    36.5,
					    39.4,
					    42.0,
					    44.6,
					    46.5,
					    48.3,
					    49.9,
					    50.8,
					    50.8,
					    49.9,
					    48.4,
					    47.0,
					    42.1,
					    39.0,
					    36.0,
					    32.9,
					    28.5,
					    25.0,
					    22.1,
					    19.1,
					    16.4,
					    13.6,
					    11.1,
					    8.8,
					    6.8,
					    5.0};

      // Optical photon detection (PD) energy [eV]
      std::vector<G4double> PDEnergy;
      
      // SiPM reflectivity [%] 
      std::vector<G4double> PDReflectivity; 

      const G4double nm2eV = 1239.583*eV*nm;
      
      for(size_t i=0; i<PDWavelength.size(); i++){
	PDEnergy.push_back(nm2eV / PDWavelength.at(i));
	PDReflectivity.push_back(100-PDEfficiency.at(i));
      }

      // Ensure energy vector (along with corresponding vectors) is
      // sorted from low-to-high energy as required by the G4 MPT

      std::reverse(PDEnergy.begin(), PDEnergy.end());
      std::reverse(PDEfficiency.begin(), PDEfficiency.end());
      std::reverse(PDReflectivity.begin(), PDReflectivity.end());

      // Set the efficiency and reflectivity of the Si-APD material
      SiPM_MPT->AddProperty("EFFICIENCY", PDEnergy, PDEfficiency);
      SiPM_MPT->AddProperty("REFLECTIVITY", PDEnergy, PDReflectivity);
      
    }
    else
      G4Exception("materialsLibrary::SiPM()",
		  "materialException001",
		  FatalException,
		  "SiPM type not found! Present choices are: 'S14160'");

    // Refractive index (RI) photon energy
    std::vector<G4double> RIEnergy= {0.1*eV, 0.5*eV, 1.0*eV, 1.5*eV, 2.0*eV, 2.5*eV,
				     3.0*eV, 3.1*eV, 3.2*eV, 3.3*eV, 3.4*eV, 3.5*eV,
				     3.9*eV, 9.9*eV};

    // Refractive index (RI)
    std::vector<G4double> RI = {3.421, 3.443, 3.517, 3.660, 3.898, 4.318,
				5.270, 5.632, 6.197, 6.888, 6.557, 5.675,
				5.125, 5.125};
    
    // Set the refractive index silicon
    SiPM_MPT->AddProperty("RINDEX", RIEnergy, RI);
    
    // Set the MPT to the material
    SiPM->SetMaterialPropertiesTable(SiPM_MPT);
    
    init = false;
  }
  return SiPM;
}


G4Material *materialsLibrary::EJ301()
{
  static G4bool init = true;
  static G4Material *EJ301 = new G4Material("EJ301", EJ301_density,2);
  static G4MaterialPropertiesTable *EJ301_MPT = new G4MaterialPropertiesTable();

  if(init){

    EJ301->AddElement(G4NistManager::Instance()->FindOrBuildElement(6), EJ301_CAtoms);
    EJ301->AddElement(G4NistManager::Instance()->FindOrBuildElement(1), EJ301_HAtoms);
    
    // Transform the wavelength spectrum [nm] into energy spectrum [eV]

    const G4double nm2eV = 1239.583*eV*nm;
    for(G4int i=0; i<EJ301_entries; i++)
      EJ301_eSpectrum.push_back(nm2eV / EJ301_eWavelength[i]);

    // Ensure energy vector (along with corresponding emission
    // probability vector) is sorted from low-to-high energy as
    // required by the G4 MPT

    std::reverse(EJ301_eSpectrum.begin(), EJ301_eSpectrum.end());
    std::reverse(EJ301_eProb.begin(), EJ301_eProb.end());
    
    // Scale the scintillation light yield by the user-set factor

    std::vector<G4double> EJ301_eScaledLight, EJ301_pScaledLight, EJ301_aScaledLight, EJ301_cScaledLight;

    for(G4int i=0; i<EJ301_lightEntries; i++){
      EJ301_eScaledLight.push_back(EJ301_eLight[i] * EJ301_yieldScaleFactor);
      EJ301_pScaledLight.push_back(EJ301_pLight[i] * EJ301_yieldScaleFactor);
      EJ301_aScaledLight.push_back(EJ301_aLight[i] * EJ301_yieldScaleFactor);
      EJ301_cScaledLight.push_back(EJ301_cLight[i] * EJ301_yieldScaleFactor);
    }
    
    // Add properties independent of photon energy
    
    EJ301_MPT->AddConstProperty("RESOLUTIONSCALE",EJ301_rScale);
    EJ301_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", EJ301_timeConst1);
    EJ301_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", EJ301_timeConst2);
    EJ301_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT3", EJ301_timeConst3);
    EJ301_MPT->AddConstProperty("SCINTILLATIONYIELD1", 1);
    //EJ301_MPT->AddConstProperty("ELECTRONSCINTILLATIONYIELD2", 0.131);
    //EJ301_MPT->AddConstProperty("ELECTRONSCINTILLATIONYIELD3", 0.013);
    //EJ301_MPT->AddConstProperty("PROTONSCINTILLATIONYIELD1", 0.625);
    //EJ301_MPT->AddConstProperty("PROTONSCINTILLATIONYIELD2", 0.162);
    //EJ301_MPT->AddConstProperty("PROTONSCINTILLATIONYIELD3", 0.108);


    // Add properties dependent on photon energy

    EJ301_MPT->AddProperty("RINDEX", EJ301_rIndexSpectrum, EJ301_rIndex);
    EJ301_MPT->AddProperty("ABSLENGTH", EJ301_absLengthSpectrum, EJ301_absLength);
    EJ301_MPT->AddProperty("SCINTILLATIONCOMPONENT1", EJ301_eSpectrum, EJ301_eProb);
    EJ301_MPT->AddProperty("SCINTILLATIONCOMPONENT2", EJ301_eSpectrum, EJ301_eProb);
    EJ301_MPT->AddProperty("SCINTILLATIONCOMPONENT3", EJ301_eSpectrum, EJ301_eProb);
    

    EJ301_MPT->AddProperty("ELECTRONSCINTILLATIONYIELD", EJ301_energyDep, EJ301_eLight);
    EJ301_MPT->AddProperty("PROTONSCINTILLATIONYIELD",   EJ301_energyDep, EJ301_pLight);
    EJ301_MPT->AddProperty("DEUTERONSCINTILLATIONYIELD", EJ301_energyDep, EJ301_pLight);
    EJ301_MPT->AddProperty("TRITONSCINTILLATIONYIELD",   EJ301_energyDep, EJ301_pLight);
    EJ301_MPT->AddProperty("ALPHASCINTILLATIONYIELD",    EJ301_energyDep, EJ301_aLight);
    EJ301_MPT->AddProperty("IONSCINTILLATIONYIELD",      EJ301_energyDep, EJ301_cLight);

    EJ301->SetMaterialPropertiesTable(EJ301_MPT);
    
    init = false;
  }
  return EJ301;
}

G4Material *materialsLibrary::BoratedHDPE()
{
  static G4bool init = true;
  static G4Material *BoratedHDPE = new G4Material("BoratedHDPE", 1.0*g/cm3, 3);
  
  if (init){
    BoratedHDPE->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_H"), 0.125355);
    BoratedHDPE->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_B"), 0.100000);
    BoratedHDPE->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_C"), 0.774645);

    init = false;
  }
  return BoratedHDPE;
}

G4Material *materialsLibrary::Aluminum()
{
  static G4bool init = true;
  static G4Material *Aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  if (init){
    std::vector<G4double> energySmall = { 2.0 * eV, 3.47 * eV };
    std::vector<G4double> refractiveIndexAluminum = { 1.314, 0.382 };
    std::vector<G4double> absClad = { 20.0 * m, 20.0 * m };

    static G4MaterialPropertiesTable *Al_MPT = new G4MaterialPropertiesTable();

    Al_MPT->AddProperty("RINDEX", energySmall, refractiveIndexAluminum);
    Al_MPT->AddProperty("ABSLENGTH",energySmall, absClad);

    Aluminum->SetMaterialPropertiesTable(Al_MPT);

    init = false;
  }

  return Aluminum;

}
