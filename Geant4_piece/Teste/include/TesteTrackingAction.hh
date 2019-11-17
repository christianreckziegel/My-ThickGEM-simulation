
#ifndef TesteTrackingAction_h
#define TesteTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class TesteTrackingAction : public G4UserTrackingAction 
{
public:
  TesteTrackingAction();
  virtual ~TesteTrackingAction(){};
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
  
};

#endif
