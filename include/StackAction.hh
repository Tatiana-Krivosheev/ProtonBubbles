#pragma once

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;

class StackAction : public G4UserStackingAction
{
#pragma region Ctor/Dtor/ops
    public: StackAction();
    public: virtual ~StackAction();
#pragma endregion

    public: virtual G4ClassificationOfNewTrack ClassifyNewTrack( const G4Track* aTrack);
};
