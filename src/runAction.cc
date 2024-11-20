#include "G4RunManager.hh"
#include "runAction.hh"

#include "G4AnalysisManager.hh"

runAction::runAction()
{
    OpticalPhotons = true;
    
    if (OpticalPhotons){
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->SetDefaultFileType("root");

        analysisManager->SetFileName("OpticalPhotonDetection");
        
        // Creating 2D histograms
        analysisManager
            ->CreateH2("PmtTimeResponse","Optical Photon Origin Voxcel vs PMT Arrival Time", // h2 Id = 0
                9, 1, 10, 100, 0., 2.);
    }

}

runAction::~runAction()
{;}

void runAction::BeginOfRunAction(const G4Run *)
{
    G4cout << "\n *********** Run Started *************"
    << G4endl;
    if(OpticalPhotons){
        auto analysisManager = G4AnalysisManager::Instance();
    
        // reset the histograms
        analysisManager->Reset();

        // Open an output file
        // The default file name is set in RunAction::RunAction(),
        // it can be overwritten in a macro
        analysisManager->OpenFile();
    }
}

void runAction::EndOfRunAction(const G4Run *)
{
    if(OpticalPhotons){
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->Write();
        
        // Keep content of histos so that they are plotted.
        // The content will be reset at start of the next run.
        analysisManager->CloseFile(false);
    }

    G4cout << "\n *********** Run Finished ************"
    << G4endl;
}