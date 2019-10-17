#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "HadrontherapyDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4Timer.hh"
#include "RunAction.hh"


RunAction::RunAction()
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    std::cout << "Run " << run->GetRunID() << " starts ..." << '\n';
}

void HadrontherapyRunAction::EndOfRunAction(const G4Run*)
{
    //G4cout << " Summary of Run " << aRun -> GetRunID() <<" :"<< G4endl;
    //G4cout << "Number of electromagnetic processes of primary particles in the phantom:"
    // 	   << electromagnetic << G4endl;
    //G4cout << "Number of hadronic processes of primary particles in the phantom:"
    //	   << hadronic << G4endl;
}
