#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

#include "stackingAction.hh"
#include "eventAction.hh"

#include <iostream>

stackingAction::stackingAction(eventAction *currentEvent)
    : evtAction(currentEvent)
{;}


stackingAction::~stackingAction()
{;}


G4ClassificationOfNewTrack stackingAction::ClassifyNewTrack(const G4Track* currentTrack)
{
  // Get particle definiton
  G4ParticleDefinition *PDef = currentTrack->GetDefinition();

    // Add count to tally of photons created
  if(PDef == G4OpticalPhoton::OpticalPhotonDefinition())
    evtAction->AddPhotonCreated(1);
  else if (PDef == G4Neutron::NeutronDefinition()){
    G4double PKE = 0;
    PKE =+ currentTrack->GetKineticEnergy()/keV;
    evtAction->SetEnergy(PKE);
  }

  //if (PKE > evtAction->GetEnergy() and PDef == G4Proton::ProtonDefinition()){
  //  G4String CreatorProcess = currentTrack->GetCreatorProcess()->GetProcessName();
  //  G4cout <<PDef->GetParticleName()<<"; "<<PKE<<";"<<evtAction->GetEnergy()<<";"<<CreatorProcess<<G4endl;
  //}
 
  // Set this flag to drastically improve CPU when using optical
  // physics, provided the spectra from photon creation is sufficient
  G4bool killOpticalPhotons = true;

  // Set this flag to kill gammas in order to mimick
  // post-processing of experimental data using PSD.
  // This will also kill gammas from the PGA
  G4bool killSecondaryGammas = true;
  
  if(killOpticalPhotons or killSecondaryGammas){

    if(killOpticalPhotons and PDef == G4OpticalPhoton::OpticalPhotonDefinition())
      return fKill;

    else if(killSecondaryGammas and PDef == G4Gamma::GammaDefinition())
      return fKill;
    
    else
      return fUrgent;
  }
  else
    return fUrgent;
}
