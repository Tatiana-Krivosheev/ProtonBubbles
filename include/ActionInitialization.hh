#pragma once

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
    public: HadrontherapyActionInitialization();
    public: virtual ~HadrontherapyActionInitialization();

    public: virtual void BuildForMaster() const;
    public: virtual void Build() const;
};
