#pragma once

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "G4Material.hh"

class Tally;

class StepAction : public G4UserSteppingAction
{
#pragma region Data
    private: Tally*      _tally;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: StepAction( Tally* );
    public: virtual ~StepAction();
#pragma endregion

#pragma region Overrides
    public: virtual void UserSteppingAction(const G4Step*);
#pragma endregion
};
