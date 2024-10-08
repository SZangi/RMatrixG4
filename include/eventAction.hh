#ifndef eventAction_hh
#define eventAction_hh 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "eventActionMessenger.hh"

#include <fstream>
#include <list>

class runAction;

// eventAction class handles information about entire events.  More
// specifically, it will receive energy deposited per step from
// steppingAction and recursively add it to get the total energy per
// event.  Also, eventAction is responsible for outputting information
// to data files, thus, it has user- controlled functions, which are
// set through eventActionMessenger, which control data output

class eventAction : public G4UserEventAction
{

public:
  eventAction();
  ~eventAction();

  // These are virtual functions of G4UserEventAction that the user
  // can redefine to perform his or her heart's desire.  They always
  // accept a G4Event * as an argument
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(const G4Event *);
  
  // This is a user-dfined function to add the energy deposited at
  // each step to a variable that holds total energy deposited per
  // event.  It is called by "steppingAction.cc" every time there is
  // energy deposited on a step
  void AddPhotonCreated(G4int Photons)  
  {PhotonsCreated += Photons;
  };

  void SetEnergy(G4double PartEnergy)
  {
    if (NeutronEnergy == 0)
      NeutronEnergy += PartEnergy;
  }

  G4int GetEnergy()
  {
    return NeutronEnergy;
  }

  // The following two functions are called from eventActionMessenger
  // at runtime when the user desires to change something....
  void SetDataOutput(G4String onOff)
  { if(onOff == "on") dataOutputSwitch = true;
    if(onOff == "off") dataOutputSwitch = false;};
  
  void SetOutputFileName(G4String fName)
  {if(eventOutput.is_open()) eventOutput.close();
    eventOutput.open(fName,std::ofstream::trunc);};
  
private:
  G4int PhotonsCreated;
  
  G4double NeutronEnergy;

  G4bool dataOutputSwitch;
 
  eventActionMessenger *eventMessenger;
  
  std::ofstream eventOutput;

  std::ofstream processOutput;

  std::ofstream detectOutput;
};
    
#endif
