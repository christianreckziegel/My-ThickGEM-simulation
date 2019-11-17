
#ifndef TesteRegionInformation_H
#define TesteRegionInformation_H 1

#include "globals.hh"
#include "G4VUserRegionInformation.hh"

class TesteRegionInformation : public G4VUserRegionInformation
{
public:
  TesteRegionInformation(); 
  virtual ~TesteRegionInformation();
  virtual void Print() const;

  inline void SetWorld(G4bool v=true) {fIsWorld = v;}
  inline void SetHelmholtz(G4bool v=true) {fIsHelmholtz = v;}
  inline G4bool IsHelmholtz() const {return fIsHelmholtz;}

private:
  G4bool fIsWorld;
  G4bool fIsHelmholtz;
};

#endif

