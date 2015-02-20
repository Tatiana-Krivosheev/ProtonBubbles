#pragma once 

class Source;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

#include "globals.hh"
#include "G4UImessenger.hh"

class SourceMessenger: public G4UImessenger
{
#pragma region Data
    private: Source* _source;

    //commands
    private: G4UIcmdWithADoubleAndUnit*  _momentum_cmd;       // mean E in MeV
    private: G4UIcmdWithADoubleAndUnit*  _momentumSigma_cmd;  // sigma E in MeV

    private: G4UIcmdWithADoubleAndUnit*  _radius_cmd;       // r in mm
    private: G4UIcmdWithADoubleAndUnit*  _radiusSigma_cmd;  // rSigma in mm    
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: SourceMessenger(Source* src);
    public: ~SourceMessenger();
#pragma endregion    

    public: void     SetNewValue(G4UIcommand * command, G4String newValues);
    public: G4String GetCurrentValue(G4UIcommand * command);
};
