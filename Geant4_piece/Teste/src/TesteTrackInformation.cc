

#include "TesteTrackInformation.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"    

G4ThreadLocal G4Allocator<TesteTrackInformation> * aTrackInformationAllocator = 0;

//Constructor 1
TesteTrackInformation::TesteTrackInformation()
  : G4VUserTrackInformation()
{
    fOriginalTrackID = 0;
    fParticleDefinition = 0;
    fOriginalPosition = G4ThreeVector(0.,0.,0.);
    fOriginalMomentum = G4ThreeVector(0.,0.,0.);
    fOriginalEnergy = 0.;
    fOriginalTime = 0.;
    fTrackingStatus = 1;
    fSourceTrackID = -1;
    fSourceDefinition = 0;
    fSourcePosition = G4ThreeVector(0.,0.,0.);
    fSourceMomentum = G4ThreeVector(0.,0.,0.);
    fSourceEnergy = 0.;
    fSourceTime = 0.;
    fSuspendedStepID = -1;
}

//constructor 2
TesteTrackInformation::TesteTrackInformation(const G4Track* aTrack):
    G4VUserTrackInformation()
{
    fOriginalTrackID = aTrack->GetTrackID();
    fParticleDefinition = aTrack->GetDefinition();
    fOriginalPosition = aTrack->GetPosition();
    fOriginalMomentum = aTrack->GetMomentum();
    fOriginalEnergy = aTrack->GetTotalEnergy();
    fOriginalTime = aTrack->GetGlobalTime();
    fTrackingStatus = 1;
    fSourceTrackID = -1;
    fSourceDefinition = 0;
    fSourcePosition = G4ThreeVector(0.,0.,0.);
    fSourceMomentum = G4ThreeVector(0.,0.,0.);
    fSourceEnergy = 0.;
    fSourceTime = 0.;
    fSuspendedStepID = -1;
}

//constructor 3
TesteTrackInformation::TesteTrackInformation(const TesteTrackInformation* aTrackInfo):
    G4VUserTrackInformation()
{
    fOriginalTrackID = aTrackInfo->fOriginalTrackID;
    fParticleDefinition = aTrackInfo->fParticleDefinition;
    fOriginalPosition = aTrackInfo->fOriginalPosition;
    fOriginalMomentum = aTrackInfo->fOriginalMomentum;
    fOriginalEnergy = aTrackInfo->fOriginalEnergy;
    fOriginalTime = aTrackInfo->fOriginalTime;
    fTrackingStatus = aTrackInfo->fTrackingStatus;
    fSourceTrackID = aTrackInfo->fSourceTrackID;
    fSourceDefinition = aTrackInfo->fSourceDefinition;
    fSourcePosition = aTrackInfo->fSourcePosition;
    fSourceMomentum = aTrackInfo->fSourceMomentum;
    fSourceEnergy = aTrackInfo->fSourceEnergy;
    fSourceTime = aTrackInfo->fSourceTime;
    fSuspendedStepID = -1;
}

//destructor
TesteTrackInformation::~TesteTrackInformation()
{;}

//operator
TesteTrackInformation& TesteTrackInformation::operator =(const TesteTrackInformation& aTrackInfo)
{
    fOriginalTrackID = aTrackInfo.fOriginalTrackID;
    fParticleDefinition = aTrackInfo.fParticleDefinition;
    fOriginalPosition = aTrackInfo.fOriginalPosition;
    fOriginalMomentum = aTrackInfo.fOriginalMomentum;
    fOriginalEnergy = aTrackInfo.fOriginalEnergy;
    fOriginalTime = aTrackInfo.fOriginalTime;
    fTrackingStatus = aTrackInfo.fTrackingStatus;
    fSourceTrackID = aTrackInfo.fSourceTrackID;
    fSourceDefinition = aTrackInfo.fSourceDefinition;
    fSourcePosition = aTrackInfo.fSourcePosition;
    fSourceMomentum = aTrackInfo.fSourceMomentum;
    fSourceEnergy = aTrackInfo.fSourceEnergy;
    fSourceTime = aTrackInfo.fSourceTime;
    fSuspendedStepID = -1;

    return *this;
}

//coleta informação da fonte
void TesteTrackInformation::SetSourceTrackInformation(const G4Track* aTrack)
{
    fSourceTrackID = aTrack->GetTrackID();
    fSourceDefinition = aTrack->GetDefinition();
    fSourcePosition = aTrack->GetPosition();
    fSourceMomentum = aTrack->GetMomentum();
    fSourceEnergy = aTrack->GetTotalEnergy();
    fSourceTime = aTrack->GetGlobalTime();
}

//imprimir informações da Track
void TesteTrackInformation::Print() const
{
    G4cout 
     << "Source track ID " << fSourceTrackID << " (" 
     << fSourceDefinition->GetParticleName() << ","
     << fSourceEnergy/GeV << "[GeV]) at " << fSourcePosition << G4endl;
    G4cout
      << "Original primary track ID " << fOriginalTrackID << " (" 
      << fParticleDefinition->GetParticleName() << ","
     << fOriginalEnergy/GeV << "[GeV])" << G4endl;
}

