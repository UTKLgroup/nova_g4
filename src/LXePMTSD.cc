#include "LXePMTSD.hh"
#include "LXePMTHit.hh"
#include "NovaDetectorConstruction.hh"
#include "LXeUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePMTSD::LXePMTSD(G4String name)
  : G4VSensitiveDetector(name),fPMTHitCollection(0),fPMTPositionsX(0)
  ,fPMTPositionsY(0),fPMTPositionsZ(0)
{
  collectionName.insert("pmtHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePMTSD::~LXePMTSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::Initialize(G4HCofThisEvent* hitsCE){
  fPMTHitCollection = new LXePMTHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, fPMTHitCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool LXePMTSD::ProcessHits(G4Step* ,G4TouchableHistory* ){
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool LXePMTSD::ProcessHits_constStep(const G4Step* aStep,
                                       G4TouchableHistory* ){

  //need to know if this is an optical photon
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4int pmtNumber=
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
  G4VPhysicalVolume* physVol=
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

  //Find the correct hit collection
  G4int n=fPMTHitCollection->entries();
  LXePMTHit* hit=NULL;
  for(G4int i=0;i<n;i++){
    if((*fPMTHitCollection)[i]->GetPMTNumber()==pmtNumber){
      hit=(*fPMTHitCollection)[i];
      break;
    }
  }
 
  if(hit==NULL){//this pmt wasnt previously hit in this event
    hit = new LXePMTHit(); //so create new hit
    hit->SetPMTNumber(pmtNumber);
    hit->SetPMTPhysVol(physVol);
    fPMTHitCollection->insert(hit);
    hit->SetPMTPos((*fPMTPositionsX)[pmtNumber],(*fPMTPositionsY)[pmtNumber],
                   (*fPMTPositionsZ)[pmtNumber]);
  }

  hit->IncPhotonCount(); //increment hit for the selected pmt

  /*
  if(!NovaDetectorConstruction::GetSphereOn()){
    hit->SetDrawit(true);
    //If the sphere is disabled then this hit is automaticaly drawn
  }
  else{//sphere enabled
    LXeUserTrackInformation* trackInfo=
      (LXeUserTrackInformation*)aStep->GetTrack()->GetUserInformation();
    if(trackInfo->GetTrackStatus()&hitSphere)
      //only draw this hit if the photon has hit the sphere first
      hit->SetDrawit(true);
  }
  */
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::PrintAll() {}
