

#ifndef HelmholtzSD_h
#define HelmholtzSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HelmholtzHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class HelmholtzSD : public G4VSensitiveDetector
{

public:
  HelmholtzSD(G4String name);
  virtual ~HelmholtzSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);

protected:
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  
private:
  HelmholtzHitsCollection *fHelCollection;
  int   fCellID[20][48];
  const int fNumberOfCellsInZ;
  const int fNumberOfCellsInPhi;
};

#endif

