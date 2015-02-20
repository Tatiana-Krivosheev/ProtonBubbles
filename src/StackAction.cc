#include "StackAction.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"

StackAction::StackAction()
{
}

StackAction::~StackAction()
{
}

G4ClassificationOfNewTrack StackAction::ClassifyNewTrack(const G4Track* /* aTrack */)
{
    G4ClassificationOfNewTrack classification = fUrgent;
    return classification;
}
