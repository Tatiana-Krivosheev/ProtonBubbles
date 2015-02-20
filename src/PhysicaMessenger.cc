#include "PhysicaMessenger.hh"
#include "Physica.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"

PhysicaMessenger::PhysicaMessenger(Physica* physList):
    _physicsList(physList)
{  
    _listDir = new G4UIdirectory("/Physics/");

    // Building modular PhysicsList

    _physicsListCmd = new G4UIcmdWithAString("/Physics/addPhysics",this);  
    _physicsListCmd->SetGuidance("Add chunks of PhysicsList.");
    _physicsListCmd->SetParameterName("physList",false);
    _physicsListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
    _gammaCutCmd = new G4UIcmdWithADoubleAndUnit("/Physics/setGCut",this);  
    _gammaCutCmd->SetGuidance("Set gamma cut.");
    _gammaCutCmd->SetParameterName("Gcut",false);
    _gammaCutCmd->SetUnitCategory("Length");
    _gammaCutCmd->SetRange("Gcut>0.0");
    _gammaCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _electCutCmd = new G4UIcmdWithADoubleAndUnit("/Physics/setECut",this);  
    _electCutCmd->SetGuidance("Set electron cut.");
    _electCutCmd->SetParameterName("Ecut",false);
    _electCutCmd->SetUnitCategory("Length");
    _electCutCmd->SetRange("Ecut>0.0");
    _electCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
    _positCutCmd = new G4UIcmdWithADoubleAndUnit("/Physics/setPCut",this);  
    _positCutCmd->SetGuidance("Set positron cut.");
    _positCutCmd->SetParameterName("Pcut",false);
    _positCutCmd->SetUnitCategory("Length");
    _positCutCmd->SetRange("Pcut>0.0");
    _positCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

    _allCutCmd = new G4UIcmdWithADoubleAndUnit("/Physics/setCuts",this);  
    _allCutCmd->SetGuidance("Set cut for all.");
    _allCutCmd->SetParameterName("cut",false);
    _allCutCmd->SetUnitCategory("Length");
    _allCutCmd->SetRange("cut>0.0");
    _allCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
}

PhysicaMessenger::~PhysicaMessenger()
{
    delete _physicsListCmd;
    delete _gammaCutCmd;
    delete _electCutCmd;
    delete _positCutCmd;
    delete _allCutCmd;
    delete _listDir;
}

void PhysicaMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if (command == _physicsListCmd)
    { 
		_physicsList->AddPhysicsList(newValue);
	}
    else if( command == _gammaCutCmd )
    {
        _physicsList->SetCutForGamma(_gammaCutCmd->GetNewDoubleValue(newValue));
    }
    else if( command == _electCutCmd )
    {
        _physicsList->SetCutForElectron(_electCutCmd->GetNewDoubleValue(newValue));
    }
    else if( command == _positCutCmd )
    {
        _physicsList->SetCutForPositron(_positCutCmd->GetNewDoubleValue(newValue));
    }
    else if( command == _allCutCmd )
    {
        double cut = _allCutCmd->GetNewDoubleValue(newValue);
        _physicsList->SetCutForGamma(cut);
        _physicsList->SetCutForElectron(cut);
        _physicsList->SetCutForPositron(cut);
    }
}
