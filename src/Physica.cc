#include "G4RunManager.hh"
#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4PhysListFactory.hh"
#include "Physica.hh"
#include "PhysicaMessenger.hh"
#include "G4SystemOfUnits.hh"

// Physic lists (contained inside the Geant4 source code, in the 'physicslists folder')
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4DataQuestionaire.hh"
#include "HadronPhysicsQGSP_BIC.hh"

#include "G4LossTableManager.hh"

Physica::Physica(double cuts):
    G4VModularPhysicsList(),
    _emPhysicsList(0),
    _decayPhysicsList(0),
    _emExtraPhysicsList(0),
    _hadelasticPhysicsList(0),
    _hadPhysicsList(0),
    _stoppingPhysicsList(0),
    _ionsPhysicsList(0),
    _neutronsPhysicsList(0)
{
    int ver = 1;

    G4LossTableManager::Instance();

    defaultCutValue = cuts;

    std::cout << "Cut set to:" << G4BestUnit(cuts,"Length") << std::endl;

    _cutForGamma     = defaultCutValue;
    _cutForElectron  = defaultCutValue;
    _cutForPositron  = defaultCutValue;

    _messenger = new PhysicaMessenger(this);

    SetVerboseLevel(ver);

    // EM physics
    _emPhysicsList = new G4EmStandardPhysics_option3(ver);
    _emName = G4String("standard_opt3");

    // Decay physics and all particles
    _decayPhysicsList = new G4DecayPhysics(ver);

    // Synchroton Radiation & GN Physics
     _emExtraPhysicsList = new G4EmExtraPhysics(ver);

    // Hadron Elastic scattering
    _hadelasticPhysicsList = new G4HadronElasticPhysics(ver);

    // Hadron Physics
    _hadPhysicsList = new HadronPhysicsQGSP_BIC(ver);

    // Stopping Physics
    _stoppingPhysicsList = new G4StoppingPhysics(ver);

    // Ion Physics
    _ionsPhysicsList = new G4IonBinaryCascadePhysics(ver);

    // Neutron tracking cut
    _neutronsPhysicsList = new G4NeutronTrackingCut(ver);
}

Physica::~Physica()
{ 
    delete _messenger;

    delete _emPhysicsList;
    delete _decayPhysicsList;
    delete _emExtraPhysicsList;
    delete _hadelasticPhysicsList;
    delete _hadPhysicsList;
    delete _stoppingPhysicsList;
    delete _ionsPhysicsList;
    delete _neutronsPhysicsList;
}

void Physica::ConstructParticle()
{
    _decayPhysicsList->ConstructParticle();
    _emPhysicsList->ConstructParticle();
    _emExtraPhysicsList->ConstructParticle();
    _hadelasticPhysicsList->ConstructParticle();
    _hadPhysicsList->ConstructParticle();
    _stoppingPhysicsList->ConstructParticle();
    _ionsPhysicsList->ConstructParticle();
    _neutronsPhysicsList->ConstructParticle();
}

void Physica::ConstructProcess()
{
    AddTransportation();

    _emPhysicsList->ConstructProcess();
    _decayPhysicsList->ConstructProcess();
    _emExtraPhysicsList->ConstructProcess();
    _hadelasticPhysicsList->ConstructProcess();
    _hadPhysicsList->ConstructProcess();
    _stoppingPhysicsList->ConstructProcess();
    _ionsPhysicsList->ConstructProcess();
    _neutronsPhysicsList->ConstructProcess();
}

void Physica::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>1)
	   G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;

    if (name == _emName)
        return;

    /////////////////////////////////////////////////////////////////////////////
    //   ELECTROMAGNETIC MODELS
    /////////////////////////////////////////////////////////////////////////////
    if (name == "standard_opt3")
    {
	   _emName = name;
	   delete _emPhysicsList;
	   _emPhysicsList = new G4EmStandardPhysics_option3();
	   G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	   G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardPhysics_option3" << G4endl;
    }
    else if (name == "LowE_Livermore")
    {
	   _emName = name;
	   delete _emPhysicsList;
	   _emPhysicsList = new G4EmLivermorePhysics();
	   G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	   G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
    }
    else if (name == "LowE_Penelope")
    {
	   _emName = name;
	   delete _emPhysicsList;
	   _emPhysicsList = new G4EmPenelopePhysics();
	   G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	   G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
    } 
    else
    {
	   G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
	          << " is not defined"
	          << G4endl;
    }
}

void Physica::SetCuts()
{  
    if (verboseLevel > 0)
    {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(_cutForGamma, "gamma");
    SetCutValue(_cutForElectron, "e-");
    SetCutValue(_cutForPositron, "e+");
    SetCutValue(_cutForProton, "proton");

    if (verboseLevel > 0)
        DumpCutValuesTable();
}

void Physica::SetCutForGamma(double cut)
{
    _cutForGamma = cut;
    SetParticleCuts(_cutForGamma, G4Gamma::Gamma());
}

void Physica::SetCutForElectron(double cut)
{
    _cutForElectron = cut;
    SetParticleCuts(_cutForElectron, G4Electron::Electron());
}

void Physica::SetCutForPositron(double cut)
{
    _cutForPositron = cut;
    SetParticleCuts(_cutForPositron, G4Positron::Positron());
}

void Physica::SetCutForProton(double cut)
{
    _cutForProton = cut;
    SetParticleCuts(_cutForProton, G4Proton::Proton());
}
