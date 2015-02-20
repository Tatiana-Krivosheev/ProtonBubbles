#include "G4SteppingManager.hh"
#include "G4VProcess.hh"
#include "G4ParticleTypes.hh"

#include "StepAction.hh"
#include "Tally.hh"

StepAction::StepAction(Tally* tally):
    _tally(tally)
{
}

StepAction::~StepAction()
{
}

void StepAction::UserSteppingAction(const G4Step* aStep)
{
    double edep = aStep->GetTotalEnergyDeposit();
    if (edep <= 0.0)
        return;
    
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

    if (!preStepPoint)
        return;

    G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
    /// G4int depth = theTouchable->GetHistoryDepth();

    ///if (theTouchable->GetVolume()->GetLogicalVolume()->GetName() == "VoxelBoxLog") // not to use string comparison!

    if (theTouchable->GetVolume()->GetLogicalVolume() == _tally->voxel_box_log())
    {
        int voxIdx = theTouchable->GetReplicaNumber();
        double* t;

        // add energy deposition
        t = _tally->edep();
        t[voxIdx] += edep;

        // accumulate variance
        t = _tally->eerr();
        t[voxIdx] += edep*edep;

        _tally->tot_edep() += edep;
        _tally->tot_eerr() += edep*edep;
    }
}
