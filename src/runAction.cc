#include "G4RunManager.hh"
#include "runAction.hh"

runAction::runAction()
{;}

runAction::~runAction()
{;}

void runAction::BeginOfRunAction(const G4Run *)
{
    G4cout << "\n *********** Run Started *************"
    << G4endl;
}

void runAction::EndOfRunAction(const G4Run *)
{
    G4cout << "\n *********** Run Finished ************"
    << G4endl;
}