#ifndef G4MATERIALSBUILDER_HH
#define G4MATERIALSBUILDER_HH

#include "G4Material.hh"
#include "G4NistElementBuilder.hh"
#include <map>
#include "G4SystemOfUnits.hh"

class G4MaterialsBuilder
{
public:
  G4MaterialsBuilder();
  ~G4MaterialsBuilder();
  
  G4Material *FindOrBuildStandardMaterial(G4String);
  G4Material *FindOrBuildNISTMaterial(G4String);
  G4Material *FindOrBuildPNNLMaterial(G4String);
  G4Material *FindOrBuildOpticalMaterial(G4String);

private:
  void Initialize();
  void StandardMaterials();
  void PNNLMaterials();
  void OpticalMaterials();

  void AddMaterial(const G4String &name,
		   const G4String &formulas,
		   G4double dens,
		   G4int nComp,
		   G4bool iso=false,
		   G4bool opt=false,
		   G4State state=kStateSolid,
		   G4bool stp=true,
		   G4double pot=0.);

  void AddElementByWeightFraction(G4int Z, G4double w, G4int isos, G4int &As, G4double &Abs);
  void AddElementByWeightFraction(G4int Z, G4double w);
  void AddElementByWeightFraction(const G4String &name, G4double w);
  
  void AddElementByAtomCount(G4int Z, G4int n);
  void AddElementByAtomCount(const G4String &name, G4int n);
  
  void AddElementByIsotopes(G4int Z, G4int n, G4int &As, G4double &Ws);
  void AddElementByIsotopes(const G4String &name, G4int n,
			    G4int &As, G4double &Ws);
  
  G4Material *BuildMaterial(const G4String &name);
  void AddOpticalProperties(const G4String &name, G4Material *material);
  
  void AddOpticalPropertiesByName(const G4String name, G4double yieldScaleFactor);
  void AddOpticalPropertiesByName(const G4String name);

  std::vector<G4double> LaBrResponseFunction(std::vector<G4double> eSpectrum, G4double yield);
  std::vector<G4double> EJ309ResponseFunction(std::vector<G4double> eSpectrum, G4double yield, G4String part);
  std::vector<G4double> EJ301ResponseFunction(std::vector<G4double> eSpectrum, G4double yield, G4String part, std::vector<G4double> pCreated);
  
private:
  G4Material* newMaterial;
  
  G4int nMaterials;
  G4int nOptMaterials;

  G4int nComponents, nCurrent;
  G4bool materialComplete, elementComplete;

  G4double scintYieldFactor;

  std::vector<G4int> indexes;
  
  std::vector<G4String> names;
  std::vector<G4String> formulas;
  std::vector<G4double> densities;
  std::vector<G4int> components;
  std::vector<G4bool> isIsotopics;
  std::vector<G4bool> isOpticals;
  std::vector<G4State> states;
  std::vector<G4bool> stps;
  std::vector<G4double> potentials;
  
  std::vector<G4bool> atomCounts;
  
  std::vector<G4int> elements;
  std::vector<G4double> elementFractions;
  std::vector<G4double> elementWeights;
  std::vector<std::vector<G4int> > elementIsotopeAs;
  std::vector<std::vector<G4double> > elementIsotopeWeights;

  std::vector<G4String> opt_names;
  std::vector<G4double> materialTimeConst1;
  std::vector<G4double> materialTimeConst2;
  std::vector<G4double> materialTimeConst3;
  std::vector<G4double> materialScintYield;
  std::vector<G4double> materialRScale;
  std::vector<G4double> materialScintYieldScale;
  std::vector<std::vector<G4double> > materialRIndexSpectrum;
  std::vector<std::vector<G4double> > materialRIndex;
  std::vector<std::vector<G4double> > materialEmitSpectrum;
  std::vector<std::vector<G4double> > materialEmitProbability;
  std::vector<std::vector<G4double> > materialAbsLengthSpectrum;
  std::vector<std::vector<G4double> > materialAbsLength;
  std::vector<std::vector<G4double> > materialEnDeposited;
  std::vector<std::vector<G4double> > materialeLightResponse;
  std::vector<std::vector<G4double> > materialpLightResponse;
  std::vector<std::vector<G4double> > materialiLightResponse;
  std::vector<std::vector<G4double> > materialaLightResponse;
  

  static const std::map<G4String,std::vector<std::vector<G4double> > > fOptVectorsDataTable;
  std::map<G4String,std::vector<std::vector<G4double> > >::const_iterator iter;

  //static const std::map<G4String,std::vector<G4double>> fOptConstDataTable;
  //std::map<G4String,std::vector<G4double>>::const_iterator constants_iter;
  G4int verbose;

  G4NistElementBuilder *elementBuilder;
};

#endif