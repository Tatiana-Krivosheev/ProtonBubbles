#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

bool reshuffleBubbles = false;

EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event*)
{
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
    int event_id = evt->GetEventID();
  
    // periodic printing
    if (event_id < 10 || event_id % 10000 == 0)
    {
        G4cout << ">>> Event " << evt->GetEventID() << G4endl;
#if 0
        // get number of stored trajectories

        G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
        int n_trajectories = trajectoryContainer ? trajectoryContainer->entries() : 0;
        G4cout << "    " << n_trajectories 
               << " trajectories stored in this event." << G4endl;
#endif
    }

    if (event_id > 0)
    {
        if ((event_id % 1000) == 0)
            reshuffleBubbles = true;
    }
}

