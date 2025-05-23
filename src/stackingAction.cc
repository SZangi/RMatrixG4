#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

#include "stackingAction.hh"
#include "eventAction.hh"

#include "G4AnalysisManager.hh"

#include <iostream>

stackingAction::stackingAction(eventAction *currentEvent)
    : evtAction(currentEvent)
{PSDShortGate = 2*ns;
  PSDLongGate = 200*ns;}


stackingAction::~stackingAction()
{;}


G4ClassificationOfNewTrack stackingAction::ClassifyNewTrack(const G4Track* currentTrack)
{
  auto anaMgr = G4AnalysisManager::Instance();

  // Get particle definiton
  G4ParticleDefinition *PDef = currentTrack->GetDefinition();
  G4double trackTime = currentTrack->GetGlobalTime();
    // Add count to tally of photons created
  if(PDef == G4OpticalPhoton::OpticalPhotonDefinition()){
    evtAction->AddPhotonCreated(1);
    evtAction->AddPhotonTime(trackTime);
    anaMgr->FillH1(0,trackTime);
    if(evtAction->GetStartTime()==-1){evtAction->SetStartTime(trackTime);}
    G4double StartTime = evtAction->GetStartTime();
    //G4cout << StartTime*ns<<G4endl;
    //G4cout<< (trackTime-StartTime)<<G4endl;
    if((trackTime-StartTime) > PSDShortGate){
      //G4cout << "TailPhoton"<<G4endl;
      evtAction->AddTailPhoton(1);}
    }
  else if (PDef == G4Neutron::NeutronDefinition()){
    G4double PKE = 0;
    PKE =+ currentTrack->GetKineticEnergy()/keV;
    evtAction->SetEnergy(PKE);
  }
  else if (PDef == G4Gamma::GammaDefinition()){
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
  G4bool killSecondaryGammas = false;
  
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
