#pragma once

#include "G4UserEventAction.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
#pragma region Ctor/Dtor/ops
    public: EventAction();
    public: virtual ~EventAction();
#pragma endregion

#pragma region Overrides
    public: virtual void BeginOfEventAction(const G4Event*);
    public: virtual void EndOfEventAction(const G4Event*);
#pragma endregion
};
