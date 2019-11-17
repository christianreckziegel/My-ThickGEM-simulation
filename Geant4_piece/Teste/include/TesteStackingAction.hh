
#ifndef TesteStackingAction_H
#define TesteStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;
class G4VHitsCollection;

class TesteStackingAction : public G4UserStackingAction
{
public:
  TesteStackingAction();
  virtual ~TesteStackingAction();

public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();

private:
  G4VHitsCollection* GetHelCollection();
  
  G4int fStage;
  G4int fHelmholtzHitsColID;
};

#endif

