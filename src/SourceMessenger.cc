#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"

#include "SourceMessenger.hh"
#include "Source.hh"

SourceMessenger::SourceMessenger(Source* src):
    _source(src)
{
    _momentum_cmd = new G4UIcmdWithADoubleAndUnit("/phsource/energy", this);
    _momentum_cmd->SetGuidance("Mean energy of primaries");
    _momentum_cmd->SetParameterName("E", true);
    _momentum_cmd->SetRange("E>=0.");
    _momentum_cmd->SetDefaultValue(150.0);
    _momentum_cmd->SetDefaultUnit("MeV");

    _momentumSigma_cmd = new G4UIcmdWithADoubleAndUnit("/phsource/energySigma", this);
    _momentumSigma_cmd->SetGuidance("Mean energy sigma of primaries");
    _momentumSigma_cmd->SetParameterName("ESigma", true);
    _momentumSigma_cmd->SetRange("ESigma>=0.");
    _momentumSigma_cmd->SetDefaultValue(1.0);
    _momentumSigma_cmd->SetDefaultUnit("MeV");

    _radius_cmd = new G4UIcmdWithADoubleAndUnit("/phsource/radius", this);
    _radius_cmd->SetGuidance("Source radius");
    _radius_cmd->SetParameterName("R", true);
    _radius_cmd->SetRange("R>=0.");
    _radius_cmd->SetDefaultValue(20.0);
    _radius_cmd->SetDefaultUnit("mm");

    _radiusSigma_cmd = new G4UIcmdWithADoubleAndUnit("/phsource/radiusSigma", this);
    _radiusSigma_cmd->SetGuidance("Source radius sigma");
    _radiusSigma_cmd->SetParameterName("RSigma", true);
    _radiusSigma_cmd->SetRange("RSigma>=0.");
    _radiusSigma_cmd->SetDefaultValue(1.0);
    _radiusSigma_cmd->SetDefaultUnit("mm");
}

SourceMessenger::~SourceMessenger()
{
    delete _momentum_cmd;
    delete _momentumSigma_cmd;
    delete _radius_cmd;
    delete _radiusSigma_cmd;
}

void SourceMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == _momentum_cmd )
    {
        _source->set_momentum(_momentum_cmd->GetNewDoubleValue(newValue));
    }
    if( command == _momentumSigma_cmd )
    {
        _source->set_momentumSigma(_momentumSigma_cmd->GetNewDoubleValue(newValue));
    }

    if( command == _radius_cmd )
    {
        _source->set_radius(_radius_cmd->GetNewDoubleValue(newValue));
    }    
    if( command == _radiusSigma_cmd )
    {
        _source->set_radiusSigma(_radiusSigma_cmd->GetNewDoubleValue(newValue));
    }    
}

G4String SourceMessenger::GetCurrentValue(G4UIcommand * command)
{
    G4String cv;
    if( command == _momentum_cmd )
    {
        cv = _momentum_cmd->ConvertToString(_source->momentum(), "MeV");
    }
    if( command == _momentumSigma_cmd )
    {
        cv = _momentumSigma_cmd->ConvertToString(_source->momentumSigma(), "MeV");
    }

    if( command == _radius_cmd )
    {
        cv = _radius_cmd->ConvertToString(_source->radius(), "mm");
    }
    if( command == _radiusSigma_cmd )
    {
        cv = _radiusSigma_cmd->ConvertToString(_source->radiusSigma(), "mm");
    }

    return cv;
}
