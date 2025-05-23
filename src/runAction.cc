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

    analysisManager->CreateH2("psdHist","Photons Created vs. Photons in Tail",100,100,30000,50,0,1);
    analysisManager->CreateH1("timeHist","Time Photon Created",100,0,60);
}

void runAction::EndOfRunAction(const G4Run *)
{
    G4cout << "\n *********** Run Finished ************"
    << G4endl;
    
    dataOutput = true;
//
    if (dataOutput){
       //auto anaMngr = G4AnalysisManager::Instance();
        //G4int psdID = anaMngr->GetH2Id("psdHist");
        //anaMngr->WriteH2(psdID,"PSDTestBoth.root");
        //anaMngr->WriteH1(0,"BothTimes.root");
        //anaMngr->CloseFile();
    }
}