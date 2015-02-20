#pragma once

#include "G4PhantomParameterisation.hh"
#include "PhantomSetup.hh"

class G4Material;
class G4VPhysicalVolume;
class G4VTouchable;

class Parameterisation : public G4PhantomParameterisation
{ 
#pragma region Data
    private: PhantomSetup     _phs;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Parameterisation(const PhantomSetup& phs);

    public: virtual ~Parameterisation();
#pragma endregion

#pragma region Overrides
    public: virtual G4Material* ComputeMaterial(const int copyNo,
                                                G4VPhysicalVolume*  currentVol,
                                                const G4VTouchable* parentTouch=0);
#pragma endregion
};
