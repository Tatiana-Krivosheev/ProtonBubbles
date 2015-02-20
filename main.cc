#define USE_MAG_FIELD

#include "G4SystemOfUnits.hh"
#include "Phantom.hh"
#include "Source.hh"
#include "EventAction.hh"
#include "StackAction.hh"
#include "StepAction.hh"
#include "Tally.hh"
#include "Spectrum.hh"
#include "PhantomSetup.hh"
#include "Physica.hh"
#include "QGSP_BIC_EMY.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

int main(int argc, char** argv)
{
        // SKIP BEFORE PHANTOM INIT !
        // will produce unique bubbles configuration
        if (argc > 2)
        {
            float s = 0.0f;
            int skip = atoi(argv[2]);
            for(int k = 0; k != skip; ++k)
            {
                s += G4UniformRand();
            }
            std::cout << "Skipped " << skip << " random numbers with sum = " << s << std::endl;
        }

    // Number of generated protons
    G4String phname = "Phantom";

    // up to 8 voxels with bubbles, 8-19 is air,
    int boundBubbles =  8; // [0...8)
    int boundAir     = 20; // [8...20)
    float fill_rate  = 0.3f;
    PhantomSetup phs("phantom.hed", boundBubbles, boundAir, fill_rate);

    // Construct the default run manager
    G4RunManager* runManager = new G4RunManager;

    // Set mandatory initialization classes
    Phantom* phantom = new Phantom(phname, phs);

    runManager->SetUserInitialization(phantom);

    // physics init

    ///double cuts = 0.1*mm;
    ///runManager->SetUserInitialization(new Physica(cuts));

    // physics init QGSP_BIC_EMY
    runManager->SetUserInitialization(new QGSP_BIC_EMY);

    // scoring
    Tally* tally = new Tally(phantom);

    // Set mandatory user action class
    /// Spectrum* spec = new Spectrum("spectr.dat");

    runManager->SetUserAction(new Source(phs.cube_x(), phs.cube_y(), phs.cube_z(),
                                         99.9999*cm,
                                         150.0*MeV, 1.0*MeV,
                                         20.0*mm, 0.1*mm));
    runManager->SetUserAction(tally);
    runManager->SetUserAction(new StepAction(tally));
    runManager->SetUserAction(new StackAction);
    runManager->SetUserAction(new EventAction);

    // Initialize G4 kernel
    runManager->Initialize();

    // visuals
#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    // Get the pointer to the UI manager and set verbosities
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc > 1) // Batch mode
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];

        UImanager->ApplyCommand(command+fileName);
    }
    else
    {
    // interactive mode : define UI session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
        UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
        ui->SessionStart();
        delete ui;
#endif
    }

#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    return 0;
}
