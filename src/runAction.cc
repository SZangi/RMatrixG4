#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "runAction.hh"

runAction::runAction()
{;}

runAction::~runAction()
{;}

void runAction::BeginOfRunAction(const G4Run *)
{
    G4cout << "\n *********** Run Started *************"
    << G4endl;

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");

    analysisManager->CreateH2("psdHist","Photons Created vs. Photons in Tail",0,);
}

void runAction::EndOfRunAction(const G4Run *)
{
    G4cout << "\n *********** Run Finished ************"
    << G4endl;
}