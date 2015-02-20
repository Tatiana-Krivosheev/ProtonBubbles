#pragma once

#include "globals.hh"
#include "G4UImessenger.hh"

class Physica;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAInt;

class PhysicaMessenger: public G4UImessenger
{
#pragma region Data
    private: Physica*            _physicsList;   
    private: G4UIdirectory*      _listDir;
    private: G4UIcmdWithAString* _physicsListCmd;

    private: G4UIcmdWithADoubleAndUnit* _gammaCutCmd;
    private: G4UIcmdWithADoubleAndUnit* _electCutCmd;
    private: G4UIcmdWithADoubleAndUnit* _positCutCmd;

    private: G4UIcmdWithADoubleAndUnit* _allCutCmd;    
#pragma endregion

    public: PhysicaMessenger(Physica* physList);
    public: ~PhysicaMessenger();
  
    public: void SetNewValue(G4UIcommand*, G4String);
};
