// Author: Arthur Zangi
// email: szangi@mit.edu
// Date: 05/06/2024


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// class description 
// 
// This is a file containing all optical materials information needed to 
// construct optical materials using the G4MaterialsManager class. It contains 
// emission spectrum data in the fOptVectorsDataTable, in the format, 
// name::[emissionWaveLengths, emissionProbability]. !!! To add other vectorizable
// information later, ie. rIndexWavelength, rIndex, electron emission energy, response
// function data if applicable. 

// Format is: {"Name",
//              rIndexWavelength,
//              rIndex,
//              absLengthSpectrum,
//              absLength,
//              emissionWavelength,
//              emissionProbability,
//              electronEnergy,
//              electronLightOutput
// }

// Coefficient data, ie. Time constants, yield, yield scale factors, rScale,
// will be stored in the fOptConstantsDataTable

// All Isotope material data is taken from the G4NistElementBuilder.cc which
// gets all data from the NIST DB on Atomic Weights and Isotope Compositions:
// http://physics.nist.gov/PhysRefData/Compositions/index.html

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#include "G4MaterialsBuilder.hh"

std::vector<G4double> LaBrConstProperties = {
    // Time Const 1
    16*ns,
    // Time Const 2
    -1,
    // Time Const 3
    -1,
    // yield /MeV
    63000 /MeV,
    // R-scale or Gaussian Broadening
    0.42
};

std::vector<std::vector<G4double> > LaBrVectorProperties = {
    // G4double Properties 0
    LaBrConstProperties,
    // rIndexWaveLength [nm] 1
    std::vector<G4double> {349 *nm, 
                            375*nm,
                            399*nm,
                            424*nm,
                            449*nm,
                            499*nm,
                            549*nm,
                            600*nm,
                            649*nm,
                            700*nm},
    // rIndex [unitless] 2
    std::vector<G4double> {2.77,   
                            2.30,
                            2.23,
                            2.20,
                            2.16,
                            2.11,
                            2.08,
                            2.06,
                            2.04,
                            2.03},
    // Absorption Length Energy Spectrum [eV] 3
    std::vector<G4double> {1.*eV,15.*eV},
    // Absorption Length [m] 4
    std::vector<G4double> {3.*m, 3.*m},
    // Emission Wavelength Spectrum [nm] 5
    std::vector<G4double> {330.00000*CLHEP::nm,
                            331.81818*CLHEP::nm,
                            339.83957*CLHEP::nm,
                            351.87164*CLHEP::nm,
                            375.93582*CLHEP::nm,
                            382.35294*CLHEP::nm,
                            387.16577*CLHEP::nm,
                            395.18716*CLHEP::nm,
                            401.60428*CLHEP::nm,
                            409.62567*CLHEP::nm,
                            419.25134*CLHEP::nm,
                            434.49197*CLHEP::nm,
                            629.41174*CLHEP::nm},
    // Emission Wavelength Probability [unitless] 6
    std::vector<G4double> {0.00000,
                            5.51559,
			    	        34.2926,
		    		        79.6163,
	    			        98.0815,
    			            99.7602,
                            93.0456,
	    			        71.9424,
		    		        43.4053,
			    	        16.3070,
				            7.43405,
				            2.15827,
				            0.00000},
    // Electron Energy [keV] 7
    std::vector<G4double> {1.*CLHEP::keV,1000.*CLHEP::keV,1000000.*CLHEP::keV},
    // Electron Light Output [photons] 8
    std::vector<G4double> {63,63000,63000000}};

std::vector<G4double> EJ309ConstProperties = {
    // Time Const 1
    3.5*ns,
    // Time Const 2
    35.3*ns,
    // Time Const 3
    294.0*ns,
    // yield /MeV
    12300.,
    // R-scale or Gaussian Broadening
    15.0
};

std::vector<std::vector<G4double> > EJ309VectorProperties = {
    EJ309ConstProperties,
    // rIndexSpectrum
    std::vector<G4double> {1.*eV, 15.*eV},
    // rIndex
    std::vector<G4double> {1.57, 1.57},
    // absorption length spectrum
    std::vector<G4double> {1.*eV, 15.*eV},
    // absorption lengths
    std::vector<G4double> {2.5*m, 2.5*m},
    // Emission wavelength spectrum
    std::vector<G4double> {381.08*nm,
          383.44*nm,
          385.81*nm,
          387.71*nm,
          389.29*nm,
          391.35*nm,
          393.88*nm,
          397.34*nm,
          401.27*nm,
          405.96*nm,
          410.83*nm,
          412.72*nm,
          417.12*nm,
          421.21*nm,
          423.08*nm,
          425.27*nm,
          427.13*nm,
          429.15*nm,
          432.10*nm,
          437.72*nm,
          443.66*nm,
          447.87*nm,
          451.60*nm,
          456.59*nm,
          461.58*nm,
          467.66*nm,
          473.59*nm,
          479.67*nm,
          484.51*nm,
          489.03*nm,
          496.06*nm,
          503.24*nm,
          509.17*nm,
          516.04*nm,
          520.73*nm,
          524.01*nm},
    // emission probabilites
    std::vector<G4double> {2.764,
              8.436,
              15.855,
              25.455,
              34.182,
              44.000,
              55.564,
              63.418,
              68.436,
              71.055,
              76.291,
              83.491,
              93.091,
              98.764,
              99.636,
              100.00,
              93.964,
              85.891,
              80.873,
              77.164,
              74.327,
              68.436,
              61.455,
              55.127,
              48.909,
              44.000,
              38.764,
              33.309,
              29.164,
              24.582,
              18.909,
              14.327,
              11.491,
              8.655,
              6.909,
              6.691},
    // energy deposited
    std::vector<G4double> {0.0001*MeV,
					   0.10*MeV,
					   0.13*MeV,
					   0.17*MeV,
					   0.20*MeV,
					   0.24*MeV,
					   0.30*MeV,
					   0.34*MeV,
					   0.40*MeV,
					   0.48*MeV,
					   0.60*MeV,
					   0.72*MeV,
					   0.84*MeV,
					   1.0*MeV,
					   1.3*MeV,
					   1.7*MeV,
					   2.0*MeV,
					   2.4*MeV,
					   3.0*MeV,
					   3.4*MeV,
					   4.0*MeV,
					   4.8*MeV,
					   6.0*MeV,
					   7.2*MeV,
					   8.4*MeV,
					   10.*MeV,
					   13.*MeV,
					   17.*MeV,
					   20.*MeV,
					   24.*MeV,
					   30.*MeV,
					   34.*MeV,
					   40.*MeV},
    // light output
    std::vector<G4double> {1, 10, 100}
};


const std::map<G4String,std::vector<std::vector<G4double> > > G4MaterialsBuilder::fOptVectorsDataTable = {
    // Name

    // Lanthanum Bromide

    {"LanthanumBromide",LaBrVectorProperties},

    // EJ309

    {"EJ309", EJ309VectorProperties}



};
