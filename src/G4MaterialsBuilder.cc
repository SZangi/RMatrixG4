#include "G4MaterialsBuilder.hh"
#include "G4StaticMaterialsData.hh"
#include <string>

#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"


G4MaterialsBuilder::G4MaterialsBuilder()
  : nMaterials(0), nComponents(0), nCurrent(0),
    materialComplete(true), elementComplete(true),
    verbose(2), nOptMaterials(0),scintYieldFactor(1)
{
  newMaterial = nullptr;
  elementBuilder = new G4NistElementBuilder(0);
  
  Initialize();
}


G4MaterialsBuilder::~G4MaterialsBuilder()
{
  delete newMaterial;
  delete elementBuilder;
}


G4Material *G4MaterialsBuilder::FindOrBuildStandardMaterial(G4String name)
{
  G4Material *mat = BuildMaterial(name);
  return mat;
}

G4Material *G4MaterialsBuilder::FindOrBuildNISTMaterial(G4String name)
{
  return G4NistManager::Instance()->FindOrBuildMaterial(name);
}


G4Material *G4MaterialsBuilder::FindOrBuildPNNLMaterial(G4String name)
{
  G4Material *mat = nullptr;
  return nullptr;
}


G4Material *G4MaterialsBuilder::FindOrBuildOpticalMaterial(G4String name)
{
  G4Material *mat = BuildMaterial(name);
  return mat;
}


void G4MaterialsBuilder::AddMaterial(const G4String &name, const G4String &formula,
				     G4double dens, G4int n, G4bool iso, G4bool opt,
				     G4State state, G4bool stp, G4double pot)
{
  // Here we add a material with a given name, formula, density, number of components,
  // "is isotopic", "is optical", state, stp, and potential to our material vectors 
  // These vectors will later be indexed through to build the actual material into an 
  // object the user has called later.

  if(!materialComplete){
    G4String msg = "Insufficient number of components added to material '" + name + "'";
    G4Exception("G4MaterialsBuilder::AddMaterial()",
		"G4MaterialsBuilder-001",
		FatalException,
		msg);
  }
  
  names.push_back(name);
  formulas.push_back(formula);
  densities.push_back(dens*CLHEP::g/CLHEP::cm3);
  components.push_back(n);
  isIsotopics.push_back(iso);
  isOpticals.push_back(opt);
  states.push_back(state);
  stps.push_back(stp);
  potentials.push_back(pot*CLHEP::eV);


  atomCounts.push_back(false);
  
  nCurrent = n;
  indexes.push_back(nComponents);
  
  nMaterials++;

  materialComplete = false;
  
  if(verbose > 1){
    if(not opt)
    G4cout << "\n"
	   << "G4MaterialsBuilder: Adding new material to the library!\n"
	   << "  Name    : " << name << "\n"
	   << "  Formula : " << formula << "\n"
	   << "  Density : " << dens << " [g/cm3]\n"
           << "  NComp   : " << n << "\n"
	   << G4endl;
    else
          G4cout << "\n"
	   << "G4MaterialsBuilder: Adding new optical material to the library!\n"
	   << "  Name    : " << name << "\n"
	   << "  Formula : " << formula << "\n"
	   << "  Density : " << dens << " [g/cm3]\n"
           << "  NComp   : " << n << "\n"
	   << G4endl;
  }
}


void G4MaterialsBuilder::AddElementByWeightFraction(G4int Z, G4double w)
{
  // Add element by weight fraction for a given Z, and weight fraction w

  elements.push_back(Z);
  elementFractions.push_back(w);

  --nCurrent;
  ++nComponents;

  if(nCurrent == 0){
    G4int n = nMaterials - 1;
    G4double sum = 0.;
    G4int iMin = indexes[n];
    G4int iMax = iMin + components[n];

    if(!atomCounts[n]){
      for(G4int i=iMin; i<iMax; i++){
        sum += elementFractions[i];
       }
       if(sum > 0.){
        for(G4int i=iMin; i<iMax; i++)
          elementFractions[i] /= sum;
       }
      }
    materialComplete = true;
  }
}

void G4MaterialsBuilder::AddElementByWeightFraction(G4int Z, G4double w, G4int isos, G4int &As, G4double &Abs)
{
  // Here we only add the isotopic information to the element vectors 
  // and let AddElementByWeightFraction(Z,w) handle adding elemental
  // information to save on complexity - might be slightly faster to do
  // it all in one to reduce function calls
  
  G4int n = nMaterials - 1;

  std::vector<G4int> isotopes;
  std::vector<G4double> abundances;

  if(isIsotopics[n]){
    for(G4int i=0; i<isos; i++){
      G4int iso_A = (&As)[i];
      isotopes.push_back(iso_A);
      G4double iso_abundance = (&Abs)[i];
      abundances.push_back(iso_abundance);
    }
  }
  elementIsotopeAs.push_back(isotopes);
  elementIsotopeWeights.push_back(abundances);

  AddElementByWeightFraction(Z,w);
}


void G4MaterialsBuilder::AddElementByWeightFraction(const G4String &name, G4double w)
{
  // Add an element by weight fraction if we have it's NIST name
  G4int Z = elementBuilder->GetZ(name);
  AddElementByWeightFraction(Z, w);
}


void G4MaterialsBuilder::AddElementByAtomCount(G4int Z, G4int n)
{
  // Add element by number density? 
  atomCounts[nMaterials-1] = true; // AddElementByWeightFraction knows to use n differently
  G4double w = (G4double)n;
  AddElementByWeightFraction(Z, w);
}


void G4MaterialsBuilder::AddElementByAtomCount(const G4String &name, G4int n)
{
  // add nist element by atom count
  atomCounts[nMaterials-1] = true;
  G4int Z = elementBuilder->GetZ(name);
  G4double w = (G4double)n;
  AddElementByWeightFraction(Z, w);
}


void G4MaterialsBuilder::AddElementByIsotopes(G4int Z, G4int n,
					      G4int &As, G4double &Ws)
{
  isIsotopics[nMaterials-1] = true;
  
  G4double weight = 0;
  for(G4int i=0; i<n; i++)
    weight += (&Ws)[i];
  
  G4double abundances[n];
  for(G4int i=0; i<n; i++){
    abundances[i] = (&Ws)[i]/weight;
  }
  AddElementByWeightFraction(Z, weight, n, As, *abundances);
}


void G4MaterialsBuilder::AddElementByIsotopes(const G4String &name,
					      G4int n,
					      G4int &As,
					      G4double &ws)
{
  G4int Z = elementBuilder->GetZ(name);
  AddElementByIsotopes(Z, n, As, ws);
}


void G4MaterialsBuilder::Initialize()
{
  StandardMaterials();
  PNNLMaterials();
  OpticalMaterials();
}


void G4MaterialsBuilder::StandardMaterials()
{
  // Highly Enriched Uranium
  G4int U_As_1[4] = {234, 235, 236, 238};
  G4double U_Ws_1[4] = {0.00980, 0.93155, 0.00450, 0.05415};
  
  AddMaterial("HighlyEnrichedUranium", "HEU", 18.724, 1, true);
  AddElementByIsotopes(92, 4, *U_As_1, *U_Ws_1);

  // Water
  G4int H_As_2[1] = {2};
  G4double H_Ws_2[1] = {0.201133};

  G4int O_As_2[3] = {16, 17, 18};
  G4double O_Ws_2[3] = {0.796703, 0.000323, 0.001842};

  AddMaterial("HeavyWater", "D2O", 1.1044, 2, true);
  AddElementByIsotopes(8, 3, *O_As_2, *O_Ws_2);
  AddElementByIsotopes(1, 1, *H_As_2, *H_Ws_2);
   
  /*
  AddMaterial("BoronCarbide", "B4C", 2.52, 2);
  AddElementByAtomCount("B", 4);
  AddElementByAtomCount("C", 1);
  
  AddMaterial("BoronNitride", "BN", 2.3, 2);
  AddElementByAtomCount("B", 1);
  AddElementByAtomCount("N", 1);
  
  AddMaterial("NiobiumTitanium", "NbTi", 5.7, 2);
  AddElementByAtomCount("Nb", 1);
  AddElementByAtomCount("Ti", 1);
  
  AddMaterial("NiobiumTin", "Nb3Sn", 5.7, 2);
  AddElementByAtomCount("Nb", 3);
  AddElementByAtomCount("Sn", 1);

  // Reference: https://www.electralloy.com/images/pdf/Product_Sheets/Electralloy/Nitronic-40.pdf
  // Comments: Using average of min/max range for alloying elements, max of trace contaminants
  AddMaterial("Nitronic40", "None", 7.83, 9); 
  AddElementByWeightFraction("C", 0.04);
  AddElementByWeightFraction("Cr", 20.25);
  AddElementByWeightFraction("Fe", 62.82);
  AddElementByWeightFraction("Mn", 9.0);
  AddElementByWeightFraction("N", 0.3);
  AddElementByWeightFraction("Ni", 6.5);
  AddElementByWeightFraction("P", 0.04);
  AddElementByWeightFraction("Si", 1.);
  AddElementByWeightFraction("S", 0.01);

  // Reference: https://www.electralloy.com/images/pdf/Literature/Nitronic50_main.pdf
  // Comments: Using average of min/max range for alloying elements, max of trace contaminants
  AddMaterial("Nitronic50", "None", 7.88, 12);
  AddElementByWeightFraction("C", 0.06);
  AddElementByWeightFraction("Cr", 22.0);
  AddElementByWeightFraction("Fe", 56.945);
  AddElementByWeightFraction("Mn", 5.0);
  AddElementByWeightFraction("Mo", 2.25);
  AddElementByWeightFraction("N", 0.3);
  AddElementByWeightFraction("Nb", 0.2);
  AddElementByWeightFraction("Ni", 12.5);
  AddElementByWeightFraction("P", 0.04);
  AddElementByWeightFraction("Si", 0.475);
  AddElementByWeightFraction("S", 0.03);
  AddElementByWeightFraction("V", 0.2);
  
  AddMaterial("TungstenCarbide", "WC", 15.63, 2);
  AddElementByAtomCount("W", 1);
  AddElementByAtomCount("C", 1);
  */
}


void G4MaterialsBuilder::PNNLMaterials()
{;}


void G4MaterialsBuilder::OpticalMaterials()
{
  AddMaterial("LanthanumBromide", "LaBr3", 5.08, 2, false,true);
  AddElementByAtomCount("La", 1);
  AddElementByAtomCount("Br", 3);
  AddOpticalPropertiesByName("LanthanumBromide");

  AddMaterial("EJ309","C435H543", 0.959, 2, false, true);
  AddElementByAtomCount("C", 435);
  AddElementByAtomCount("H", 543);
  AddOpticalPropertiesByName("EJ309");
}


G4Material* G4MaterialsBuilder::BuildMaterial(const G4String &name)
{
  newMaterial = nullptr;
  
  G4int matID = -1;
  for(G4int i=0; i<nMaterials; i++)
    if(name == names[i])
      matID = i;
  
  if(matID > -1){
    G4double temperature = NTP_Temperature;
    G4double pressure = STP_Pressure;
    
    newMaterial = new G4Material(names[matID], densities[matID], components[matID],
				 states[matID], temperature, pressure);
    G4int idx = indexes[matID];
    for(G4int i=0; i<components[matID]; i++){
      G4int Z = elements[idx+i];
      if(!isIsotopics[matID]){
        G4Element *E = elementBuilder->FindOrBuildElement(Z);
        if(E == nullptr){
	        G4Exception("G4MaterialsBuilder::BuildMaterial()",
		      "G4MaterialsBuilder-002",
		      FatalException,
		      "Invalid element Z specified; no valid element point available");
        }
        else{
	        if(atomCounts[matID])
	          newMaterial->AddElement(E, G4int(elementFractions[idx+i]));
	        else
	          newMaterial->AddElement(E, elementFractions[idx+i]);
        }
      }
      else{
        G4Element *E1 = elementBuilder->FindOrBuildElement(Z);
        if(E1 == nullptr){
	        G4Exception("G4MaterialsBuilder::BuildMaterial()",
		      "G4MaterialsBuilder-002",
		      FatalException,
		      "Invalid element Z specified; no valid element point available");
        }
        else{
          std::vector<G4int> isotopes = elementIsotopeAs[idx+i];
          std::vector<G4double> abundances = elementIsotopeWeights[idx+i];
          G4String elm_name = E1->GetName();
          G4String elm_symbol = E1->GetSymbol();
          delete E1;
          G4Element *E = new G4Element(elm_name, elm_symbol,G4int(isotopes.size()));
          for(G4int j=0; j<G4int(isotopes.size()); j++){
            G4int Iso_A = isotopes[j];
            G4double iso_weight = elementBuilder->GetAtomicMass(Z,Iso_A);
            // check that the isotope is valid
            if(iso_weight != 0){
              // Double check that the Geant collaboration allows std::to_string
              std::string Iso_A_name_std = std::to_string(Iso_A);
              G4String Iso_A_name = G4String(Iso_A_name_std);
              G4String I_Name = elm_symbol + "-" + Iso_A_name;
              G4Isotope *I = new G4Isotope(I_Name, Z, Iso_A, iso_weight *g/(mole*amu_c2));
              E -> AddIsotope(I, abundances[j] * 100 *perCent);
            }
            else{
              G4Exception("G4MaterialsBuilder::BuildMaterial()",
              "G4MaterialsBuilder-003",
              FatalException,
              "Invalid isotope A specified; no valid isotope point available");
            }
            
          }
          newMaterial -> AddElement(E, elementFractions[idx+i]);
        }
      }
    }
    if(isOpticals[matID]){
      AddOpticalProperties(name, newMaterial);
    }
  }
  else{
    G4Exception("G4MaterialsBuilder::BuildMaterial()",
		"G4MaterialsBuilder-004",
		FatalException,
		"Invalid material name; no valid material pointer returned");
  }
  return newMaterial;
}


void G4MaterialsBuilder::AddOpticalProperties(const G4String &name, G4Material *material)
{
  G4MaterialPropertiesTable *Mat_MPT = new G4MaterialPropertiesTable();
  const G4double nm2eV = 1239.583*eV*nm;
  // Find material, and index in storage vectors
  G4int optID = -1;
  for(G4int i=0; i<nOptMaterials; i++){
    if(name == opt_names[i])
      optID = i;
  }

  if (optID > -1){
  // Pull back in properties from storage vectors
    G4double rScale = materialRScale[optID];
    G4double timeConst1 = materialTimeConst1[optID];
    G4double timeConst2 = materialTimeConst2[optID];
    G4double timeConst3 = materialTimeConst3[optID];
    G4double scintYield = materialScintYield[optID];
    G4double yieldScale = materialScintYieldScale[optID];


    Mat_MPT->AddConstProperty("RESOLUTIONSCALE", rScale);
    if (timeConst1 != -1)
      Mat_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1",timeConst1);
    if (timeConst2 != -1)
      Mat_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2",timeConst2);
    if (timeConst3 != -1)
      Mat_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT3",timeConst3);
    
    std::vector<G4double> eSpectrum = materialEmitSpectrum[optID];
    std::vector<G4double> eProbability = materialEmitProbability[optID];

    std::vector<G4double> eDeposited = materialEnDeposited[optID];
    std::vector<G4double> eLightResponse = materialeLightResponse[optID];
    std::vector<G4double> pLightResponse = materialpLightResponse[optID];
    std::vector<G4double> aLightResponse = materialaLightResponse[optID];
    std::vector<G4double> iLightResponse = materialiLightResponse[optID];

    std::vector<G4double> rIndexSpectrum = materialRIndexSpectrum[optID];
    std::vector<G4double> rIndex = materialRIndex[optID];

    std::vector<G4double> absLengthSpectrum = materialAbsLengthSpectrum[optID];
    std::vector<G4double> absLength = materialAbsLength[optID];

    if(pLightResponse[0] == -1)
      Mat_MPT->AddConstProperty("SCINTILLATIONYIELD",scintYield * yieldScale);

    Mat_MPT->AddProperty("SCINTILLATIONCOMPONENT1", eSpectrum, eProbability);
    
    if(eLightResponse[0] != -1)
      Mat_MPT->AddProperty("ELECTRONSCINTILLATIONYIELD", eDeposited, eLightResponse);
    if(pLightResponse[0] != -1){
      Mat_MPT->AddProperty("PROTONSCINTILLATIONYIELD", eDeposited, pLightResponse);
      // At a later point, if data exists, deuteron and triton scintillation may be added
      Mat_MPT->AddProperty("DEUTERONSCINTILLATIONYIELD", eDeposited, pLightResponse);
      Mat_MPT->AddProperty("TRITONSCINTILLATIONYIELD", eDeposited, pLightResponse);
    }
    if(aLightResponse[0] != -1)
      Mat_MPT->AddProperty("ALPHASCINTILLATIONYIELD", eDeposited, aLightResponse);
    if(iLightResponse[0] != -1){
      Mat_MPT->AddProperty("IONSCINTILLATIONYIELD", eDeposited, iLightResponse);
      if(aLightResponse[0] == -1)
        Mat_MPT->AddProperty("ALPHASCINTILLATIONYIELD", eDeposited, iLightResponse);
    }

    Mat_MPT->AddProperty("RINDEX", rIndexSpectrum, rIndex);
    Mat_MPT->AddProperty("ABSLENGTH", absLengthSpectrum, absLength);

    material->SetMaterialPropertiesTable(Mat_MPT);
  }
  else{
    G4Exception("G4MaterialsBuilder::BuildMaterial()",
		"G4MaterialsBuilder-005",
		FatalException,
		"Invalid optical material name; no valid material pointer returned");
  }  
}

void G4MaterialsBuilder::AddOpticalPropertiesByName(const G4String name)
{
  const G4double nm2eV = 1239.583*eV*nm;

  std::vector<std::vector<G4double>> OptVectData = fOptVectorsDataTable.at(name);
  
  // Get constants from first vector
  G4double timeConst1 = OptVectData[0][0];
  G4double timeConst2 = OptVectData[0][1];
  G4double timeConst3 = OptVectData[0][2];

  G4double scintYield = OptVectData[0][3];

  G4double rScale = OptVectData[0][4];

  materialTimeConst1.push_back(timeConst1);
  materialTimeConst2.push_back(timeConst2);
  materialTimeConst3.push_back(timeConst3);
  materialScintYield.push_back(scintYield);
  materialScintYieldScale.push_back(scintYieldFactor);
  materialRScale.push_back(rScale);
  
  // Reset yield scale factor after we're done
  scintYieldFactor = 1;

  // Transform rIndex wavelength spectrum into energy spectrum 
  std::vector<G4double> rIndexWavelength = OptVectData[1];
  std::vector<G4double> rIndex = OptVectData[2];
  std::vector<G4double> rIndexSpectrum;
  G4int rIndex_Entries = rIndex.size();
  for(G4int i=0; i<rIndex_Entries; i++)
    rIndexSpectrum.push_back(nm2eV / (rIndexWavelength[i]));  
  
  std::reverse(rIndexSpectrum.begin(), rIndexSpectrum.end());
  std::reverse(rIndex.begin(), rIndex.end());

  if (name == "EJ309"){
    // EJ309 rIndex data is already in energy, and so shouldn't be reversed
    rIndexSpectrum = rIndexWavelength;
    rIndex = OptVectData[2];
  }

  // Get Absorption Energy Lengths
  std::vector<G4double> absLengthSpectrum = OptVectData[3]; // [eV]
  std::vector<G4double> absLength = OptVectData[4]; // [m]

  // Get emitted light data
  std::vector<G4double> eWavelength = OptVectData[5];
  std::vector<G4double> eWavelengthProb = OptVectData[6];
  std::vector<G4double> eSpectrum;
  G4int eWavelength_Entries = eWavelength.size();
  // Transform the wavelength spectrum [nm] into an energy spectrum [eV]
  for(G4int i=0; i<eWavelength_Entries;i ++)
    eSpectrum.push_back(nm2eV / eWavelength[i]);

  // Ensure energy vector (along with corresponding emission
  // probability vector) is sorted from low-to-high energy as
  // required by the G4 MPT
  std::reverse(eSpectrum.begin(),eSpectrum.end());
  std::reverse(eWavelengthProb.begin(),eWavelengthProb.end());
  
  // Get light reponse functions
  std::vector<G4double> eDeposited = OptVectData[7];
  std::vector<G4double> eLightResponse = {-1};
  std::vector<G4double> pLightResponse = {-1};
  std::vector<G4double> iLightResponse = {-1};
  std::vector<G4double> aLightResponse = {-1};
  if(name == "LanthanumBromide"){
    eLightResponse = LaBrResponseFunction(eDeposited, scintYield);
  }
  else if(name == "EJ309"){
    eLightResponse = EJ309ResponseFunction(eDeposited, scintYield, "electron");
    pLightResponse = EJ309ResponseFunction(eDeposited, scintYield, "proton");
    aLightResponse = EJ309ResponseFunction(eDeposited, scintYield, "alpha");
    iLightResponse = EJ309ResponseFunction(eDeposited, scintYield, "ion");
  }
  
  std::vector<G4double> LightResponses = {eLightResponse[0],
                                          pLightResponse[0],
                                          aLightResponse[0],
                                          iLightResponse[0]};
  std::vector<G4String> scintillators = {"Electrons","Protons","Alphas","Ions"};
  std::vector<G4String> scintEnabled;
  for(G4int i=0; i<scintillators.size();i++){
    if (LightResponses[i] != -1)
      scintEnabled.push_back(scintillators[i]);
  }
  G4int nScintEnabled = scintEnabled.size();

  materialRIndex.push_back(rIndex);
  materialRIndexSpectrum.push_back(rIndexSpectrum);
  materialAbsLengthSpectrum.push_back(absLengthSpectrum);
  materialAbsLength.push_back(absLength);
  materialEmitSpectrum.push_back(eSpectrum);
  materialEmitProbability.push_back(eWavelengthProb);
  materialEnDeposited.push_back(eDeposited);
  materialeLightResponse.push_back(eLightResponse);
  materialpLightResponse.push_back(pLightResponse);
  materialiLightResponse.push_back(iLightResponse);
  materialaLightResponse.push_back(aLightResponse);
  opt_names.push_back(name);

  nOptMaterials++;

  if(verbose > 1)
    G4cout <<"  Scintillation enabled for:"
     << G4endl;
     for (G4int i =0; i < nScintEnabled; i++){
      G4cout <<"  " << scintEnabled[i] << G4endl;
     }
    G4cout <<"\n" <<G4endl;
	   
}

void G4MaterialsBuilder::AddOpticalPropertiesByName(const G4String name, G4double yieldScaleFactor){
  scintYieldFactor = yieldScaleFactor;
  
  AddOpticalPropertiesByName(name);
}

std::vector<G4double> G4MaterialsBuilder::LaBrResponseFunction(std::vector<G4double> eSpectrum, G4double yield)
{
  std::vector<G4double> LightResponse;
  G4int eSpectrum_Entries = eSpectrum.size();
  for(G4int i=0; i<eSpectrum_Entries;i++)
    LightResponse.push_back(eSpectrum[i] * yield / MeV);

  return LightResponse;
}

std::vector<G4double> G4MaterialsBuilder::EJ309ResponseFunction(std::vector<G4double> eSpectrum, G4double yield, G4String part)
{
  std::vector<G4double> LightResponse = {};
  G4int eSpectrum_Entries = eSpectrum.size();
  if (part == "electron"){
    for (G4int i =0; i < eSpectrum_Entries; i++)
      LightResponse.push_back(eSpectrum[i] * yield /MeV);
  }
  else if (part == "proton"){
    for (G4int i =0; i < eSpectrum_Entries; i++)
      LightResponse.push_back(0.9*(pow(eSpectrum[i],2)) / (eSpectrum[i] + 5.95) * yield /MeV);
  }
  else{
    for (G4int i =0; i < eSpectrum_Entries; i++)
      LightResponse.push_back((eSpectrum[i] * 0.013 - 0.084 )* yield /MeV);
  }

  return LightResponse;
}