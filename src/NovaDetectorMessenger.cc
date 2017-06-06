#include "NovaDetectorMessenger.hh"
#include "NovaDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"


NovaDetectorMessenger::NovaDetectorMessenger(NovaDetectorConstruction* detector)
    : detectorConstruction(detector)
{
  detectorDir = new G4UIdirectory("/Nova/detector/");
  detectorDir->SetGuidance("Detector geometry control");

  straightHeight = new G4UIcmdWithADoubleAndUnit("/Nova/detector/straightHeight", this);
  straightHeight->SetGuidance("Set the rectangle height.");
  straightHeight->SetParameterName("straightHeight", false);
  straightHeight->SetDefaultUnit("mm");

  straightWidth = new G4UIcmdWithADoubleAndUnit("/Nova/detector/straightWidth", this);
  straightWidth->SetGuidance("Set the rectangle width.");
  straightWidth->SetParameterName("straightWidth", false);
  straightWidth->SetDefaultUnit("mm");

  innerCellCornerRadius = new G4UIcmdWithADoubleAndUnit("/Nova/detector/innerCellCornerRadius", this);
  innerCellCornerRadius->SetGuidance("Set the inner cell corner radius.");
  innerCellCornerRadius->SetParameterName("innerCellCornerRadius", false);
  innerCellCornerRadius->SetDefaultUnit("mm");

  pvcThickness = new G4UIcmdWithADoubleAndUnit("/Nova/detector/pvcThickness", this);
  pvcThickness->SetGuidance("Set the thickness of the PVC.");
  pvcThickness->SetParameterName("pvcThickness", false);
  pvcThickness->SetDefaultUnit("mm");

  fiberRadius = new G4UIcmdWithADoubleAndUnit("/Nova/detector/fiberRadius", this);
  fiberRadius->SetGuidance("Set the fiber radius.");
  fiberRadius->SetParameterName("fiberRadius", false);
  fiberRadius->SetDefaultUnit("mm");

  cellLength = new G4UIcmdWithADoubleAndUnit("/Nova/detector/cellLength", this);
  cellLength->SetGuidance("Set the cell length.");
  cellLength->SetParameterName("cellLength", false);
  cellLength->SetDefaultUnit("cm");

  usePmt = new G4UIcmdWithABool("/Nova/detector/volumes/usePmt", this);
  usePmt->SetGuidance("Use PMT, else use APD");

  update = new G4UIcommand("/Nova/detector/update", this);
  update->SetGuidance("Update the detector geometry with new values.");
  update->SetGuidance("Must be run before beamOn if detector has been updated.");

  defaultValue = new G4UIcommand("/Nova/detector/defaultValue", this);
  defaultValue->SetGuidance("Set all detector geometry values to defaults.");

  liquidScintillatorLightYield=new G4UIcmdWithADouble("/Nova/detector/liquidScintillatorLightYield", this);
  liquidScintillatorLightYield->SetGuidance("Set the liquid scintillator light yield in number of photons per MeV.");

  printSetting = new G4UIcmdWithoutParameter("/Nova/detector/printSetting", this);
  printSetting->SetGuidance("Print the detector setting.");
}

NovaDetectorMessenger::~NovaDetectorMessenger()
{
  delete straightWidth;
  delete straightHeight;
  delete cellLength;
  delete pvcThickness;
  delete innerCellCornerRadius;
  delete fiberRadius;
  delete usePmt;
  delete update;
  delete defaultValue;
  delete liquidScintillatorLightYield;
  delete printSetting;
}

void NovaDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == straightWidth) {
    detectorConstruction->setStraightWidth(straightWidth->GetNewDoubleValue(newValue));
  }
  else if (command == straightHeight) {
    detectorConstruction->setStraightHeight(straightHeight->GetNewDoubleValue(newValue));
  }
  else if (command == cellLength) {
    detectorConstruction->setCellLength(cellLength->GetNewDoubleValue(newValue));
  }
  else if (command == pvcThickness) {
    detectorConstruction->setPvcThickness(pvcThickness->GetNewDoubleValue(newValue));
  }
  else if (command == innerCellCornerRadius) {
    detectorConstruction->setInnerCellCornerRadius(innerCellCornerRadius->GetNewDoubleValue(newValue));
  }
  else if (command == fiberRadius) {
    detectorConstruction->setFiberRadius(fiberRadius->GetNewDoubleValue(newValue));
  }
  else if (command == usePmt) {
    detectorConstruction->setUsePMT(usePmt->GetNewBoolValue(newValue));
  }
  else if (command == update) {
    detectorConstruction->updateGeometry();
  }
  else if (command == defaultValue) {
    detectorConstruction->setDefaults();
  }
  else if (command == liquidScintillatorLightYield) {
    detectorConstruction->setLiquidScintillatorLightYield(liquidScintillatorLightYield->GetNewDoubleValue(newValue));
  }
  else if (command == printSetting) {
    detectorConstruction->printSetting();
  }
}
