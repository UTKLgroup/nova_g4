#ifndef LXePrimaryGeneratorAction_h
#define LXePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

//from CRY
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "Randomize.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleTable.hh"

class G4ParticleGun;
class G4Event;

class LXePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    LXePrimaryGeneratorAction(const char *inputfile);
    virtual ~LXePrimaryGeneratorAction();
 
  public:

    virtual void GeneratePrimaries(G4Event* anEvent);

  //from CRY
  public:

  void InputCRY();
  void UpdateCRY(std::string* MessInput);
  void CRYFromFile(G4String newValue);
  void useCRY(G4bool usecry);

  private:

  std::vector<CRYParticle*> *vect; // vector of generated particles
  G4ParticleTable* particleTable;
  G4ParticleGun* particleGun;
  CRYGenerator* gen;
  G4int InputState;
  PrimaryGeneratorMessenger* gunMessenger;
  G4bool fuseCRY;
};

#endif
