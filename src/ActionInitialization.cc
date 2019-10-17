#include "ActionINitialization.hh"

#include "Source.hh"
#include "Tally.hh"
#include "StepAction.hh"
#include "EventAction.hh"
#include "StepAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"


ActionInitialization::ActionInitialization():
    G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void HadrontherapyActionInitialization::BuildForMaster() const
{
    auto* runAction = new RunAction();
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    // Initialize the primary particles
    auto* source = new Source();
    SetUserAction(source);

    // Optional UserActions: run, event, stepping
    auto* tally = new Tally();
    SetUserAction(tally);

    auto* eventAction = new EventAction();
    SetUserAction(eventAction);

    auto* stepAction = new StepAction(tally);
    SetUserAction(stepAction);
}
