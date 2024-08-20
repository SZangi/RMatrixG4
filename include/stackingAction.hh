#ifndef stackingAction_hh
#define stackingAction_hh 1

#include "G4UserStackingAction.hh"

class eventAction;

class stackingAction : public G4UserStackingAction
{
public:
  stackingAction(eventAction*);
  ~stackingAction();
  
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
  
private:
    eventAction *evtAction;
};

#endif