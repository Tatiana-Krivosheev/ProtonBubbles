
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "HadrontherapyPhysicsList.hh"
#include "HadrontherapyPhysicsListMessenger.hh"
#include "HadrontherapyStepMax.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4AutoDelete.hh"

Physica::Physica(double cuts):
    G4VModularPhysicsList(),
    _emPhysicsList(nullptr),
    _decayPhysicsList(nullptr),
    _radioactiveDecay_List(nullptr)
{
    G4LossTableManager::Instance();

    defaultCutValue = cuts;

    _cutForGamma = _cutForElectron = _cutForPositron = _cutForProton = cuts;

    std::cout << "Cut set to:" << G4BestUnit(cuts,"Length") << '\n';

    _messenger = new PhysicaMessenger(this);

    SetVerboseLevel(ver);

    // EM physics
    _emPhysicsList = new G4EmStandardPhysics_option4();
    _emName = G4String("standard_opt4");

    _hadronPhys.push_back( new G4DecayPhysics());
    _hadronPhys.push_back( new G4RadioactiveDecayPhysics());
    _hadronPhys.push_back( new G4IonBinaryCascadePhysics());
    _hadronPhys.push_back( new G4EmExtraPhysics());
    _hadronPhys.push_back( new G4HadronElasticPhysics());
    _hadronPhys.push_back( new G4StoppingPhysics());
    _hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());

    // Decay physics and all particles
    _decayPhysicsList      = new G4DecayPhysics();
    _radioactiveDecay_List = new G4RadioactiveDecayPhysics();
}

Physica::~Physica()
{
    delete _messenger;

    delete _emPhysicsList;
    delete _decayPhysicsList;
    delete _radioactiveDecay_List;

    for(size_t i = 0; i != _hadronPhys.size(); ++i)
    {
        delete hadronPhys[i];
    }
}

void Physica::ConstructParticle()
{
    _decayPhysicsList->ConstructParticle();
    _emPhysicsList->ConstructParticle();
    _radioactiveDecay_List->ConstructParticle();
    for(size_t i = 0; i != _hadronPhys.size(); ++i)
    {
        hadronPhys[i]->ConstructParticle();
    }
    hadronPhys.clear();
}

void Physica::ConstructProcess()
{
    AddTransportation();

    emPhysicsList->ConstructProcess();
    em_config.AddModels();

    for(size_t i=0; i != hadronPhys.size(); ++i)
    {
        hadronPhys[i]->ConstructProcess();
    }

    AddStepMax();
}

void Physica::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>1)
	   std::cout << "PhysicsList::AddPhysicsList: <" << name << ">" << '\n';

    if (name == _emName)
        return;

    /////////////////////////////////////////////////////////////////////////////
    //   ELECTROMAGNETIC MODELS
    /////////////////////////////////////////////////////////////////////////////
    if (name == "HADRONTHERAPY_1")
    {
        // The HADRONTHERAPY_1 physics list corresponds to the actual QGSP_BIC_HP list
        // but with the following differences:
        // --> G4EmStandardPhysics_option4 for the electromagnetic processes
        //     is used in place of the less accurate G4EmStandardPhysics
        // --> The G4RadioactiveDecayPhysics is added
        delete _emPhysicsList;
        _emPhysicsList = new G4EmStandardPhysics_option4();
        _emName = G4String("standard_opt4");

        hadronPhys.clear();

        hadronPhys.push_back( new G4DecayPhysics());
        hadronPhys.push_back( new G4RadioactiveDecayPhysics());
        hadronPhys.push_back( new G4IonBinaryCascadePhysics());
        hadronPhys.push_back( new G4EmExtraPhysics());
        hadronPhys.push_back( new G4HadronElasticPhysicsHP());
        hadronPhys.push_back( new G4StoppingPhysics());
        hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());

        G4cout << "HADRONTHERAPY_1 PHYSICS LIST has been activated" << G4endl;
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
