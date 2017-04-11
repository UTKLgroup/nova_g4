#include "LXeDetectorMessenger.hh"
#include "NovaDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4Scintillation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorMessenger::LXeDetectorMessenger(NovaDetectorConstruction* detector)
 : fLXeDetector(detector)
{
  //Setup a command directory for detector controls with guidance

  fDetectorDir = new G4UIdirectory("/LXe/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");

  fVolumesDir = new G4UIdirectory("/LXe/detector/volumes/");
  fVolumesDir->SetGuidance("Enable/disable volumes");
 
  //Various commands for modifying detector geometry

  fRectHeightCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/RectHeight",this);
  fRectHeightCmd->SetGuidance("Set the rectangle height.");
  fRectHeightCmd->SetParameterName("rect_height",false);
  fRectHeightCmd->SetDefaultUnit("mm");     

  fRectWidthCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/RectWidth",this);
  fRectWidthCmd->SetGuidance("Set the rectangle width.");
  fRectWidthCmd->SetParameterName("rect_width",false);
  fRectWidthCmd->SetDefaultUnit("mm");     

  fInner_Corner_RadiusCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/InnerCornerRadius",this);
  fInner_Corner_RadiusCmd->SetGuidance("Set the inner corner radius.");
  fInner_Corner_RadiusCmd->SetParameterName("inner_corner_radius",false);
  fInner_Corner_RadiusCmd->SetDefaultUnit("mm");

  fPVCThicknessCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/PVCThickness",this);
  fPVCThicknessCmd->SetGuidance("Set the thickness of the PVC.");
  fPVCThicknessCmd->SetParameterName("pvc_thickness",false);
  fPVCThicknessCmd->SetDefaultUnit("mm");

  fFibRadiusCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/FibRadius",this);
  fFibRadiusCmd->SetGuidance("Set the fiber radius.");
  fFibRadiusCmd->SetParameterName("fiber_radius",false);
  fFibRadiusCmd->SetDefaultUnit("mm");

  fCellLengthCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/CellLength",this);
  fCellLengthCmd->SetGuidance("Set the cell length.");
  fCellLengthCmd->SetParameterName("cell_length",false);
  fCellLengthCmd->SetDefaultUnit("cm");

  fFib1XCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/Fib1X",this);
  fFib1XCmd->SetGuidance("Set the x position of the first fiber.");
  fFib1XCmd->SetParameterName("fib1_x",false);
  fFib1XCmd->SetDefaultUnit("cm");

  fFib1YCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/Fib1Y",this);
  fFib1YCmd->SetGuidance("Set the y position of the first fiber.");
  fFib1YCmd->SetParameterName("fib1_y",false);
  fFib1YCmd->SetDefaultUnit("cm");

  fFib2XCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/Fib2X",this);
  fFib2XCmd->SetGuidance("Set the x position of the second fiber.");
  fFib2XCmd->SetParameterName("fib2_x",false);
  fFib2XCmd->SetDefaultUnit("cm");

  fFib2YCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/Fib2Y",this);
  fFib2YCmd->SetGuidance("Set the y position of the second fiber.");
  fFib2YCmd->SetParameterName("fib2_y",false);
  fFib2YCmd->SetDefaultUnit("cm");

  fCelltoPMTLCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/CelltoPMTL",this);
  fCelltoPMTLCmd->SetGuidance("Set the distance from the cell center and the PMT/APD.");
  fCelltoPMTLCmd->SetParameterName("celltopmtl",false);
  fCelltoPMTLCmd->SetDefaultUnit("cm");

  fFibTailCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/FibTail",this);
  fFibTailCmd->SetGuidance("Set the extra length beyond the cell.");
  fFibTailCmd->SetParameterName("fiber_tail",false);
  fFibTailCmd->SetDefaultUnit("cm");

  // other setups

  fUsePMTCmd = new G4UIcmdWithABool("/LXe/detector/volumes/usePmt",this);
  fUsePMTCmd->SetGuidance("Use PMT or APD.");  

  fLxeCmd = new G4UIcmdWithABool("/LXe/detector/volumes/lxe",this);
  fLxeCmd->SetGuidance("Enable/Disable the main detector volume.");

  fUpdateCmd = new G4UIcommand("/LXe/detector/update",this);
  fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
  fUpdateCmd->SetGuidance("Must be run before beamOn if detector has been changed.");

  fDefaultsCmd = new G4UIcommand("/LXe/detector/defaults",this);
  fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
  fDefaultsCmd->SetGuidance("(Update still required)");

  fMainScintYield=new G4UIcmdWithADouble("/LXe/detector/MainScintYield",this);
  fMainScintYield->SetGuidance("Set scinitillation yield of main volume.");
  fMainScintYield->SetGuidance("Specified in photons/MeV");

  fpmtQECmd = new G4UIcmdWithADouble("/LXe/detector/pmtQE",this);
  fpmtQECmd->SetGuidance("Set the pmt QE."); 

  fPrintSettings = new G4UIcmdWithoutParameter
    ("/LXe/detector/printSettings",this);
  fPrintSettings->SetGuidance("Print the full detector setting.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorMessenger::~LXeDetectorMessenger()
{
  //delete fScintLightYieldCmd;
  delete fRectWidthCmd;
  delete fRectHeightCmd;
  delete fCellLengthCmd;
  delete fPVCThicknessCmd;
  delete fInner_Corner_RadiusCmd;
  delete fFibRadiusCmd;
  delete fFib1XCmd;
  delete fFib1YCmd;
  delete fFib2XCmd;
  delete fFib2YCmd;
  delete fCelltoPMTLCmd;
  delete fFibTailCmd;
  delete fUsePMTCmd; 

  delete fUpdateCmd;
  delete fDefaultsCmd;
  delete fLxeCmd;
  delete fMainScintYield;
  delete fpmtQECmd;
  delete fPrintSettings;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fRectWidthCmd){
    fLXeDetector->setRectWidth(fRectWidthCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fRectHeightCmd){
    fLXeDetector->setRectHeight(fRectHeightCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fCellLengthCmd){
    fLXeDetector->setCellLength(fCellLengthCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fPVCThicknessCmd){
    fLXeDetector->setPVCThickness(fPVCThicknessCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fInner_Corner_RadiusCmd){
    fLXeDetector->setInnerCornerRadius(fInner_Corner_RadiusCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fFibRadiusCmd){
    fLXeDetector->setFiberRadius(fFibRadiusCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fFib1XCmd){
    fLXeDetector->setFiber1X(fFib1XCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fFib1YCmd){
    fLXeDetector->setFiber1Y(fFib1YCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fFib2XCmd){
    fLXeDetector->setFiber2X(fFib2XCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fFib2YCmd){
    fLXeDetector->setFiber2Y(fFib2YCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fCelltoPMTLCmd){
    fLXeDetector->setCellToPmtDistance(fCelltoPMTLCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fFibTailCmd){
    fLXeDetector->setFiberTailLength(fFibTailCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fUsePMTCmd){
    fLXeDetector->setUsePMT(fUsePMTCmd->GetNewBoolValue(newValue));
  }  
  else if (command == fUpdateCmd){
    fLXeDetector->updateGeometry();
  }
  else if (command == fDefaultsCmd){
    fLXeDetector->setDefaults();
  }
  else if (command == fLxeCmd){
    fLXeDetector->setMainVolumeOn(fLxeCmd->GetNewBoolValue(newValue));
  }
  else if (command == fMainScintYield){
    fLXeDetector->setMainScintYield(fMainScintYield->GetNewDoubleValue(newValue));
  }
  else if (command == fpmtQECmd){
    fLXeDetector->setPmtQuantumEfficiency(fpmtQECmd->GetNewDoubleValue(newValue));
  }  
  //print settings
  else if (command == fPrintSettings){
    fLXeDetector->printSettings();
  }
}
