//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file optical/LXe/include/LXeDetectorConstruction.hh
/// \brief Definition of the LXeDetectorConstruction class
//
//
#ifndef LXeDetectorConstruction_H
#define LXeDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class LXePMTSD;
class LXeScintSD;
class G4Sphere;

#include "G4Material.hh"
#include "LXeDetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"

#include "TString.h"

class LXeDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

  LXeDetectorConstruction();
  virtual ~LXeDetectorConstruction();
  
  virtual G4VPhysicalVolume* Construct();
  
  void SetDefaults();
  void UpdateGeometry();
  G4bool GetUpdated(){return fUpdated;}
  void SetMainVolumeOn(G4bool b){fMainVolume=b; fUpdated=true;}
  G4bool GetMainVolumeOn(){return fMainVolume;}
  void PrintSettings();  
  void SetMainScintYield(G4double );

  void SetRectWidth(G4double a){fRectWidth = a; fUpdated=true;}
  void SetRectHeight(G4double a){fRectHeight = a; fUpdated=true;}
  void SetCellLength(G4double a){fCellLength = a; fUpdated=true;}
  void SetPVCThickness(G4double a){fPVCThickness = a; fUpdated=true;}
  void SetInner_Corner_Radius(G4double a){fInner_Corner_Radius = a; fUpdated=true;}
  void SetFibRadius(G4double a){fFibRadius = a; fUpdated=true;}
  void SetFib1X(G4double a){fFib1X = a; fUpdated=true;}
  void SetFib1Y(G4double a){fFib1Y = a; fUpdated=true;}
  void SetFib2X(G4double a){fFib2X = a; fUpdated=true;}
  void SetFib2Y(G4double a){fFib2Y = a; fUpdated=true;}
  void SetCelltoPMTL(G4double a){fCelltoPMTL = a; fUpdated=true;}
  void SetFibTail(G4double a){fFibTail = a; fUpdated=true;}     

  G4double GetScintLightYield(){return fScintLightYield;}
  G4double GetRectWidth(){return fRectWidth;}
  G4double GetRectHeight(){return fRectHeight;}
  G4double GetCellLength(){return fCellLength;}
  G4double GetPVCThickness(){return fPVCThickness;}
  G4double GetInner_Corner_Radius(){return fInner_Corner_Radius;}
  G4double GetFibRadius(){return fFibRadius;}
  G4double GetFib1X(){return fFib1X;}
  G4double GetFib1Y(){return fFib1Y;}
  G4double GetFib2X(){return fFib2X;}
  G4double GetFib2Y(){return fFib2Y;}
  G4double GetCelltoPMTL(){return fCelltoPMTL;}
  G4double GetFibTail(){return fFibTail;}     

  void SetUsePMT(bool b){UsePMT = b;}
  bool GetUsePMT(){return UsePMT;}

  void SetpmtQE(G4double qe){fpmtQE = qe; fUpdated=true;}
  G4double GetpmtQE(){return fpmtQE;}
  
  // derived quantities
  G4double GetPMTHeight(){return fFibRadius / 4.;}
  G4double GetFibFullLength(){return fCelltoPMTL + fCellLength / 2. + fFibTail;}
  G4double GetOuter_Corner_Radius(){return fInner_Corner_Radius + fPVCThickness;}    
  G4double GetCellWidth(){return fRectWidth + 2. * GetOuter_Corner_Radius();}
  G4double GetCellHeight(){return fRectHeight + 2. * GetOuter_Corner_Radius();}
  G4double GetPMTZ(){return fCelltoPMTL + GetPMTHeight();}
  G4double GetFibZ(){return GetFibFullLength() / 2. - (fFibTail + fCellLength / 2.);}

  private:

  void DefineMaterials();
  G4VPhysicalVolume* ConstructDetector();
  LXeDetectorMessenger* fDetectorMessenger;
  
  G4bool fUpdated;
  
  G4Box* fExperimentalHall_box;
  G4LogicalVolume* fExperimentalHall_log;
  G4VPhysicalVolume* fExperimentalHall_phys;
  
  G4Element* fN;
  G4Element* fO;
  G4Element* fC;
  G4Element* fH;
  G4Element* fTi;

  G4Material* fLXe;
  G4Material* fAl;  
  G4Material* fAir;
  G4Material* fVacuum;
  G4Material* fGlass;
  G4Material* fPstyrene;
  G4Material* fPMMA;
  G4Material* fPethylene1;
  G4Material* fPethylene2;
  G4Material* fWater;
  
  G4Material* fTiO2;
  G4Material* fPVC;
  G4Material* flscint;
  G4Material* Polystyrene;
  G4Material* PMMA;
  G4Material* Fluorinated_Polymer;

  G4MaterialPropertiesTable* fLXe_mt;
  G4MaterialPropertiesTable* fWater_mt;
  G4MaterialPropertiesTable* fMPTPStyrene;
  G4MaterialPropertiesTable* fScintMPT;
  G4MaterialPropertiesTable* vacMPT;
  G4MaterialPropertiesTable* wlsMPT;
  G4MaterialPropertiesTable* innercladMPT;
  G4MaterialPropertiesTable* outercladMPT;       

  G4bool fMainVolume;  
  G4double fpmtQE;
  G4double fScintLightYield;
  G4double fRectWidth;
  G4double fRectHeight;
  G4double fCellLength;
  G4double fPVCThickness;
  G4double fInner_Corner_Radius;
  G4double fFibRadius;
  G4double fFib1X;
  G4double fFib1Y;
  G4double fFib2X;
  G4double fFib2Y;
  G4double fCelltoPMTL;
  G4double fFibTail;  
  bool UsePMT;

  TString InputDir;
};

#endif
