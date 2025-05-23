#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "G4RunManagerKernel.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4AnalysisManager.hh"

#include "eventAction.hh"
#include "eventActionMessenger.hh"
#include "G4RunManager.hh"

using namespace std;

eventAction::eventAction()
{
  // Create a messenger to allow user commands 
  eventMessenger = new eventActionMessenger(this);
  
  // This sets the name of the default MuSE output data file
  eventOutput.open("defaultOutput.csv",std::ofstream::trunc);

  // This is a boolean 'on' or 'off' switch to control data ouput
  dataOutputSwitch = false;
}


eventAction::~eventAction()
{ 
  eventOutput.close();
  delete eventMessenger;
}


// Anything included in this function is performed before each event
// is tracked through the geometry
void eventAction::BeginOfEventAction(const G4Event *)
{
  // Initialization per event.  We need to reset to the total photons
  // generated at the beginning of each event
  PhotonsCreated = 0.;
  NeutronEnergy = 0.;
  EventStartTime = -1.;
  TailPhotons = 0;
  TotPhotons = 0;

  PhotonTimes = {};
}

// Anything included in this function is performed at the very end of
// each event's lifetime.
void eventAction::EndOfEventAction(const G4Event *)
{
  // If the user has turned data output 'on', and photons were created then do this!
  if(dataOutputSwitch and (PhotonsCreated > 0))
    {
      eventOutput << NeutronEnergy << ";" << PhotonsCreated << std::endl;
      auto anaMgr = G4AnalysisManager::Instance();
      G4int histID = anaMgr->GetH2Id("psdHist");
      
      //G4cout<<"TailPhotons1: "<< (TailPhotons/PhotonsCreated)<<G4endl;
    

      EventStartTime = *min_element(PhotonTimes.begin(),PhotonTimes.end());
      //EventStartTime += 1;
      
      TailPhotons = 0;

      for (G4int i=0; i<PhotonTimes.size();i++){
        if(PhotonTimes[i] > EventStartTime+10*ns and PhotonTimes[i] < EventStartTime+400*ns){
          TailPhotons += 1;
        }
        else if(PhotonTimes[i] < EventStartTime+400*ns){
          TotPhotons += 1;
        }
      }
      TotPhotons += TailPhotons;
      //G4cout<<"TailPhotons2: "<< (TailPhotons/PhotonsCreated)<< " Photons Created "<< PhotonsCreated <<G4endl;
      //G4cout<<"Recorded Photons: "<<PhotonTimes.size()<<G4endl;
      anaMgr->FillH2(histID,TotPhotons,(TailPhotons/TotPhotons));

    }

    //delete PhotonTimes;
    
}
