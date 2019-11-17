

#include "TesteRegionInformation.hh"
#include "G4ios.hh"

//constructor
TesteRegionInformation::TesteRegionInformation():
   G4VUserRegionInformation(),
   fIsWorld(false),
   fIsHelmholtz(false)
{;}

//destructor
TesteRegionInformation::~TesteRegionInformation()
{;}

//imprime
void TesteRegionInformation::Print() const
{
 G4cout << "I'm ";
 if(fIsWorld) { G4cout << "World."; }
 else if(fIsHelmholtz) { G4cout << "Helmholtz."; }
 else { G4cout << "unknown."; }
 G4cout << G4endl;
}

