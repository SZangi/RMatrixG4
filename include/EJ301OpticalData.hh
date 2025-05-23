#ifndef EJ301OpticalData_hh
#define EJ301OpticalData_hh 1

// name: EJ301OpticalData.hh
// date: 21 Mar 23
// auth: Zach Hartwig

// desc: This file contains material and optical data for the liquid
//       organic scintillator EJ301 (Eljen Technologies). All data
//       below is available from the following references:
//
//       [1] http://www.eljentechnology.com/index.php/products/liquid-scintillators/71-ej-301
//
//       [2] Filchenkov et al, NIM A 245 (1986) 490-494
//
//       [3] V.V. Verbinski et al, NIM 65 (1968) 8-25

namespace EJ301{

  // Density [1]
  const G4double EJ301_density = 0.874*g/cm3;

  // Number of atoms (x10^20) in a cm3 of scintillator [1]
  const G4int EJ301_CAtoms = 398;
  const G4int EJ301_HAtoms = 482;

  // Refractive index [1]
  std::vector<G4double> EJ301_rIndexSpectrum = {1.*eV, 15.*eV};
  std::vector<G4double> EJ301_rIndex = {1.505, 1.505};

  // Absorption length [2]
  std::vector<G4double> EJ301_absLengthSpectrum = {1.*eV, 15.*eV};
  std::vector<G4double> EJ301_absLength = {2.5*m, 2.5*m};
  
  // Time constants [1]
  const G4double EJ301_timeConst1 = 3.16*ns;
  const G4double EJ301_timeConst2 = 32.3*ns;
  const G4double EJ301_timeConst3 = 270.*ns;
  
  // Scaling factor to reduce number of scintillation photons
  // produced. Set to 1.0 until future work on scaling is implemented
  // for nonlinear scintillation
  const G4double EJ301_yieldScaleFactor = 1.0;
  
  // This parameter gives the relative strength of the fast component as
  // a fraction of total scintillation yield.  
  const G4double EJ301_yRatio = 1.0;

  // The number of optical photon scintillated is distributed in a
  // gaussian around the scint. yield.  This factor describes the
  // "broadening" beyond the gaussian (1.0 = gaussian).
  const G4double EJ301_rScale = 7.0;

  // The EJ301 emission spectrum [1]
  const G4int EJ301_entries = 24;

  std::vector<G4double> EJ301_eSpectrum;

  std::vector<G4double> EJ301_eWavelength = {400.49017*nm,
					     405.66647*nm,
					     409.16382*nm,
					     415.07773*nm,
					     416.89502*nm,
					     419.55652*nm,
					     422.22815*nm,
					     423.90091*nm,
					     425.41293*nm,
					     427.10760*nm,
					     429.47226*nm,
					     433.00665*nm,
					     435.01950*nm,
					     441.05182*nm,
					     444.40915*nm,
					     448.44492*nm,
					     450.97372*nm,
					     454.34061*nm,
					     459.21503*nm,
					     466.43277*nm,
					     475.49246*nm,
					     489.24451*nm,
					     500.81113*nm,
					     519.07843*nm};
  
  std::vector<G4double> EJ301_eProb = {5.869,
				       13.294,
				       22.496,
				       75.333,
				       86.822,
				       95.255,
				       99.095,
				       100.00,
				       98.091,
				       89.171,
				       80.254,
				       72.619,
				       71.353,
				       70.364,
				       67.064,
				       59.942,
				       52.557,
				       44.921,
				       37.292,
				       30.441,
				       23.854,
				       16.271,
				       12.249,
				       8.005};
  
  
  // The EJ301 scintillation light emission as a function of particle
  // type and energy deposited. A note on the data for
  //
  // -- Electrons : [1] plus personal communication with Eljen fixes
  //    the # scintillation photons per MeV deposited by electrons at
  //    12000, which is linear above Order(10) keV. 
  //
  // --Protons/Alphas/Carbons: [3], Table 1 provides the best
  //   available data on nonlinear response for these particles. The
  //   values are given in Verbinski's "pulse height units" or "light
  //   units", which is defined as the light produced by 1.25 MeV
  //   electrons (See Section 7). Therefore, Table 1 values must be
  //   multiplied by 1.25*12000 compute the number of photons per
  //   MeVee deposited. Note that "1.25" is approximate; authors have
  //   used values +/- ~5% of this value.
  
  const G4double EJ301_yield = 12000.;
  
  const G4double EJ301_conversionFactor = 1.06;
  
  const G4int EJ301_lightEntries = 33;

  // Energy deposited by particle in the EJ301 scintillator [MeV]
  std::vector<G4double> EJ301_energyDep = {0.0001*MeV,
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
					   40.*MeV};

  ///////////////
  // Electrons //
  ///////////////
  
  // Number of scintillation photons [#] @ corresponding MeV value
  std::vector<G4double> EJ301_eLight = {0.0001 * EJ301_yield,
					0.10 * EJ301_yield,
					0.13 * EJ301_yield,
					0.17 * EJ301_yield,
					0.20 * EJ301_yield,
					0.24 * EJ301_yield,
					0.30 * EJ301_yield,
					0.34 * EJ301_yield,
					0.40 * EJ301_yield,
					0.48 * EJ301_yield,
					0.60 * EJ301_yield,
					0.72 * EJ301_yield,
					0.84 * EJ301_yield,
					1.0 * EJ301_yield, 
					1.3 * EJ301_yield,
					1.7 * EJ301_yield, 
					2.0 * EJ301_yield, 
					2.4 * EJ301_yield, 
					3.0 * EJ301_yield, 
					3.4 * EJ301_yield, 
					4.0 * EJ301_yield, 
					4.8 * EJ301_yield, 
					6.0 * EJ301_yield, 
					7.2 * EJ301_yield, 
					8.4 * EJ301_yield, 
					10. * EJ301_yield, 
					13. * EJ301_yield, 
					17. * EJ301_yield, 
					20. * EJ301_yield, 
					24. * EJ301_yield, 
					30. * EJ301_yield, 
					34. * EJ301_yield, 
					40. * EJ301_yield}; 
  
  
  /////////////
  // Protons //
  /////////////

  // Number of scintillation photons [#] @ corresponding MeV value
  std::vector<G4double> EJ301_pLight = {1,
					0.00671 * EJ301_yield * EJ301_conversionFactor,
					0.00886 * EJ301_yield * EJ301_conversionFactor,
					0.01207 * EJ301_yield * EJ301_conversionFactor,
					0.01465 * EJ301_yield * EJ301_conversionFactor,
					0.01838 * EJ301_yield * EJ301_conversionFactor,
					0.0246 * EJ301_yield * EJ301_conversionFactor,
					0.029 * EJ301_yield * EJ301_conversionFactor,
					0.0365 * EJ301_yield * EJ301_conversionFactor,
					0.0483 * EJ301_yield * EJ301_conversionFactor,
					0.0678 * EJ301_yield * EJ301_conversionFactor,
					0.091 * EJ301_yield * EJ301_conversionFactor,
					0.1175 * EJ301_yield * EJ301_conversionFactor,
					0.1562 * EJ301_yield * EJ301_conversionFactor,
					0.2385 * EJ301_yield * EJ301_conversionFactor,
					0.366 * EJ301_yield * EJ301_conversionFactor,
					0.4725 * EJ301_yield * EJ301_conversionFactor,
					0.625 * EJ301_yield * EJ301_conversionFactor,
					0.866 * EJ301_yield * EJ301_conversionFactor,
					1.042 * EJ301_yield * EJ301_conversionFactor,
					1.327 * EJ301_yield * EJ301_conversionFactor,
					1.718 * EJ301_yield * EJ301_conversionFactor,
					2.31 * EJ301_yield * EJ301_conversionFactor,
					2.95 * EJ301_yield * EJ301_conversionFactor,
					3.62 * EJ301_yield * EJ301_conversionFactor,
					4.55 * EJ301_yield * EJ301_conversionFactor,
					6.36 * EJ301_yield * EJ301_conversionFactor,
					8.83 * EJ301_yield * EJ301_conversionFactor,
					10.8 * EJ301_yield * EJ301_conversionFactor,
					13.5 * EJ301_yield * EJ301_conversionFactor,
					17.7 * EJ301_yield * EJ301_conversionFactor,
					20.5 * EJ301_yield * EJ301_conversionFactor,
					24.8 * EJ301_yield * EJ301_conversionFactor};
  

  ////////////
  // Alphas //
  ////////////

  // Number of scintillation photons [#] @ corresponding MeV value
  std::vector<G4double> EJ301_aLight = {1,
					0.00164 * EJ301_yield * EJ301_conversionFactor,
					0.00209 * EJ301_yield * EJ301_conversionFactor,
					0.00272 * EJ301_yield * EJ301_conversionFactor,
					0.0032 * EJ301_yield * EJ301_conversionFactor,
					0.00386 * EJ301_yield * EJ301_conversionFactor,
					0.0049 * EJ301_yield * EJ301_conversionFactor,
					0.00564 * EJ301_yield * EJ301_conversionFactor,
					0.00675 * EJ301_yield * EJ301_conversionFactor,
					0.0083 * EJ301_yield * EJ301_conversionFactor,
					0.0108 * EJ301_yield * EJ301_conversionFactor,
					0.0135 * EJ301_yield * EJ301_conversionFactor,
					0.01656 * EJ301_yield * EJ301_conversionFactor,
					0.021 * EJ301_yield * EJ301_conversionFactor,
					0.0302 * EJ301_yield * EJ301_conversionFactor,
					0.0441 * EJ301_yield * EJ301_conversionFactor,
					0.0562 * EJ301_yield * EJ301_conversionFactor,
					0.075 * EJ301_yield * EJ301_conversionFactor,
					0.11 * EJ301_yield * EJ301_conversionFactor,
					0.1365 * EJ301_yield * EJ301_conversionFactor,
					0.1815 * EJ301_yield * EJ301_conversionFactor,
					0.2555 * EJ301_yield * EJ301_conversionFactor,
					0.407 * EJ301_yield * EJ301_conversionFactor,
					0.607 * EJ301_yield * EJ301_conversionFactor,
					0.87 * EJ301_yield * EJ301_conversionFactor,
					1.32 * EJ301_yield * EJ301_conversionFactor,
					2.35 * EJ301_yield * EJ301_conversionFactor,
					4.03 * EJ301_yield * EJ301_conversionFactor,
					5.44 * EJ301_yield * EJ301_conversionFactor,
					7.41 * EJ301_yield * EJ301_conversionFactor,
					10.42 * EJ301_yield * EJ301_conversionFactor,
					12.44 * EJ301_yield * EJ301_conversionFactor,
					15.5 * EJ301_yield * EJ301_conversionFactor};
  

  /////////////////
  // Carbon ions //
  /////////////////

  // Number of scintillation photons [#] @ corresponding MeV value
  std::vector<G4double> EJ301_cLight = {1,
					0.001038 * EJ301_yield * EJ301_conversionFactor,
					0.00127 * EJ301_yield * EJ301_conversionFactor,
					0.001573 * EJ301_yield * EJ301_conversionFactor,
					0.001788 * EJ301_yield * EJ301_conversionFactor,
					0.002076 * EJ301_yield * EJ301_conversionFactor,
					0.002506 * EJ301_yield * EJ301_conversionFactor,
					0.002793 * EJ301_yield * EJ301_conversionFactor,
					0.003191 * EJ301_yield * EJ301_conversionFactor,
					0.003676 * EJ301_yield * EJ301_conversionFactor,
					0.004361 * EJ301_yield * EJ301_conversionFactor,
					0.005023 * EJ301_yield * EJ301_conversionFactor,
					0.005686 * EJ301_yield * EJ301_conversionFactor,
					0.006569 * EJ301_yield * EJ301_conversionFactor,
					0.008128 * EJ301_yield * EJ301_conversionFactor,
					0.010157 * EJ301_yield * EJ301_conversionFactor,
					0.011647 * EJ301_yield * EJ301_conversionFactor,
					0.013634 * EJ301_yield * EJ301_conversionFactor,
					0.016615 * EJ301_yield * EJ301_conversionFactor,
					0.018713 * EJ301_yield * EJ301_conversionFactor,
					0.021859 * EJ301_yield * EJ301_conversionFactor,
					0.026054 * EJ301_yield * EJ301_conversionFactor,
					0.032347 * EJ301_yield * EJ301_conversionFactor,
					0.03875 * EJ301_yield * EJ301_conversionFactor,
					0.045154 * EJ301_yield * EJ301_conversionFactor,
					0.053986 * EJ301_yield * EJ301_conversionFactor,
					0.071346 * EJ301_yield * EJ301_conversionFactor,
					0.098808 * EJ301_yield * EJ301_conversionFactor,
					0.12144 * EJ301_yield * EJ301_conversionFactor,
					0.153456 * EJ301_yield * EJ301_conversionFactor,
					0.206448 * EJ301_yield * EJ301_conversionFactor,
					0.246192 * EJ301_yield * EJ301_conversionFactor,
					0.312432 * EJ301_yield * EJ301_conversionFactor};
}

#endif
