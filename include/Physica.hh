#pragma once

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicaMessenger;
class G4VPhysicsConstructor;

class Physica: public G4VModularPhysicsList
{
#pragma region Data
    private: double _cutForGamma;
    private: double _cutForElectron;
    private: double _cutForPositron;
    private: double _cutForProton;

    private: G4String                _emName;

    private: G4VPhysicsConstructor*  _emPhysicsList;
    private: G4VPhysicsConstructor*  _decayPhysicsList;
    private: G4VPhysicsConstructor*  _emExtraPhysicsList;
    private: G4VPhysicsConstructor*  _hadelasticPhysicsList;
    private: G4VPhysicsConstructor*  _hadPhysicsList;
    private: G4VPhysicsConstructor*  _stoppingPhysicsList;
    private: G4VPhysicsConstructor*  _ionsPhysicsList;
    private: G4VPhysicsConstructor*  _neutronsPhysicsList;

    private: PhysicaMessenger*       _messenger;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Physica(double cuts);
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
#pragma endregion
};
