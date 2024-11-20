#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TouchableHistoryHandle.hh"
#include "G4StepPoint.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

#include "steppingAction.hh"
#include "eventAction.hh"
#include <iostream>

steppingAction::steppingAction(eventAction *currentEvent)
  : evtAction(currentEvent)
{OpticalPhotons = true;}

steppingAction::~steppingAction()
{;}

void steppingAction::UserSteppingAction(const G4Step *currentStep)
{
    if(OpticalPhotons){
        G4Track *currentTrack = currentStep -> GetTrack();
        G4StepPoint *preStepPoint = currentStep -> GetPreStepPoint();
        // Get the volume the track is in
        G4String currentVolumeName = currentTrack -> GetVolume() -> GetName();

        G4bool transmitted = (preStepPoint->GetStepStatus() <= fGeomBoundary);

        G4double currentTime = currentTrack->GetGlobalTime();

        if(transmitted & (currentVolumeName == "PMT_p")){
            auto touchable = currentTrack->GetOriginTouchable();
            G4int ringNo = touchable->GetCopyNumber();
            G4int pancakeNo = touchable->GetCopyNumber(1);
            evtAction->SetPhotonPosition(pancakeNo*3+ringNo);
            evtAction -> AddDetection(currentTime);
            // Kill optical photon after PMT
            //currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
        }
    }

}