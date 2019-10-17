#oragma once

#include "G4UserRunAction.hh"
#include "G4RunManager.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
    public: HadrontherapyRunAction();
    public: ~HadrontherapyRunAction();

    public: void BeginOfRunAction(const G4Run*) override;
    public: void EndOfRunAction(const G4Run* )  override;
    public: void SelectEnergy(G4int);
};
