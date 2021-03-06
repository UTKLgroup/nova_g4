#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "NovaDetectorConstruction.hh"
#include "NovaPhysicsList.hh"
#include "NovaPrimaryGeneratorAction.hh"
#include "NovaEventAction.hh"
#include "NovaStackingAction.hh"
#include "NovaSteppingAction.hh"
#include "NovaTrackingAction.hh"
#include "NovaRunAction.hh"
#include "NovaRecorderBase.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"


int main(int argc, char** argv)
{
  NovaRecorderBase* recorder = 0;
  G4RunManager* runManager = new G4RunManager;
  runManager->SetUserInitialization(new NovaDetectorConstruction);
  runManager->SetUserInitialization(new NovaPhysicsList);
  runManager->SetUserAction(new NovaPrimaryGeneratorAction(""));
  runManager->SetUserAction(new NovaStackingAction);
  runManager->SetUserAction(new NovaRunAction(recorder));
  runManager->SetUserAction(new NovaEventAction(recorder));
  runManager->SetUserAction(new NovaTrackingAction(recorder));
  runManager->SetUserAction(new NovaSteppingAction(recorder));

  G4UImanager* uiManager = G4UImanager::GetUIpointer();
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  if (argc == 1) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    uiManager->ApplyCommand("/control/execute vis.mac");
    uiManager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
    G4String filename = argv[1];
    G4String randomSeed = argv[2];
    uiManager->ApplyCommand("/random/setSeeds " + randomSeed + " 1");
    uiManager->ApplyCommand("/control/execute " + filename);
  }

  delete visManager;
  delete runManager;
  return 0;
}
