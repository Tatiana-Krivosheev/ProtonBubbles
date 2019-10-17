#pragma once

#include <vector>

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicaMessenger;
class G4VPhysicsConstructor;

class Physica: public G4VModularPhysicsList
{
#pragma region Data

    private: G4EmConfigurator em_config;

    private: double _cutForGamma;
    private: double _cutForElectron;
    private: double _cutForPositron;
    private: double _cutForProton;

    private: bool _radioactiveDecayIsRegistered;

    private: G4VPhysicsConstructor*  _emPhysicsList;
    private: G4VPhysicsConstructor*  _decayPhysicsList;
    private: G4VPhysicsConstructor*  _radioactiveDecayList;

    std::vector<G4VPhysicsConstructor*>  _hadronPhys;

    private: PhysicaMessenger*       _messenger;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Physica(cuts=0.1*mm);
    public: virtual ~Physica();
#pragma endregion

#pragma region Overrides
    public: virtual void ConstructParticle();
    public: virtual void ConstructProcess();
#pragma endregion

#pragma region Helpers
    public: void SetCuts();

    public: void SetCutForGamma(double cut);
    public: void SetCutForElectron(double cut);
    public: void SetCutForPositron(double cut);
    public: void SetCutForProton(double cut);

    public: void AddPhysicsList(const G4String& name);
    public: void AddPackage(const G4String& pack);

    public: void ConstructProcess();
    public: void AddStepMax();
    public: void AddPackage(const G4String& name);
#pragma endregion
};
