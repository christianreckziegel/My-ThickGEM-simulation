

#include "HelmholtzSD.hh"
#include "HelmholtzHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

//constructor
HelmholtzSD::HelmholtzSD(G4String name):
   G4VSensitiveDetector(name),
   fHelCollection(0),
   fNumberOfCellsInZ(20),
   fNumberOfCellsInPhi(48)
{
  // Initialize data member.
  for(G4int j=0;j<fNumberOfCellsInZ;j++)
  for(G4int k=0;k<fNumberOfCellsInPhi;k++)
  {
    fCellID[j][k] = -1;
  }
  //
  G4String HCname;
  collectionName.insert(HCname="HelCollection");
}

//destructor
HelmholtzSD::~HelmholtzSD()
{;}

//Initialize() chamado no inicio de cada evento
void HelmholtzSD::Initialize(G4HCofThisEvent* HCE)
{
  fHelCollection = new HelmholtzHitsCollection(SensitiveDetectorName,collectionName[0]); 
  for(G4int j=0;j<fNumberOfCellsInZ;j++)
  for(G4int k=0;k<fNumberOfCellsInPhi;k++)
  {
    fCellID[j][k] = -1;
  }

  static G4int HCID = -1;
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCE->AddHitsCollection( HCID, fHelCollection );
}

//ProcessHits chamado para cada step do volume sensitivo
//ProcessHits() chamado por steppingmanager, quando anda um step no logical volume que tem um ponteiro para este sensitive detector
//um ou mais objetos G4VHit serão construidos, se este step for importante para o detector
G4bool HelmholtzSD::ProcessHits(G4Step*aStep,G4TouchableHistory*)
{
//***** RE05CalorimeterSD has been migrated to Geant4 version 10 that does not
//***** support Readout Geometry in multi-threaded mode. Now RE05CalorimeterSD
//***** is assigned to a dedicaed parallel world. The pointer "aStep" points to
//***** a G4Step object for the parallel world.
  
  
  G4Track* track = aStep->GetTrack();
  double Energy = track->GetKineticEnergy();
  G4ThreeVector momentum = track->GetMomentumDirection();
  double px = momentum.x();
  double py = momentum.y();
  double pz = momentum.z();
  double mModule = sqrt( pow(px,2) + pow(py,2) + pow(pz,2) );
  double angx = acos(px/mModule);
  double angy = acos(py/mModule);
  double angz = acos(pz/mModule);
  
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;

  const G4VTouchable* ROhist = aStep->GetPreStepPoint()->GetTouchable();
  G4int copyIDinZ = ROhist->GetReplicaNumber();
  G4int copyIDinPhi = ROhist->GetReplicaNumber(1);

  if(fCellID[copyIDinZ][copyIDinPhi]==-1)
  {
    HelmholtzHit* HelHit = new HelmholtzHit(ROhist->GetVolume()->GetLogicalVolume(),copyIDinZ,copyIDinPhi);
    HelHit->SetEdep( edep );
    G4AffineTransform aTrans = ROhist->GetHistory()->GetTopTransform();
    aTrans.Invert();
    HelHit->SetPos(aTrans.NetTranslation());
    HelHit->SetRot(aTrans.NetRotation());
    HelHit->SetTrackInformation(aStep->GetTrack());
    G4int icell = fHelCollection->insert( HelHit );
    fCellID[copyIDinZ][copyIDinPhi] = icell - 1;
    if(verboseLevel>0)
    { G4cout << " New Helmholtz Hit on CellID " << copyIDinZ << " " << copyIDinPhi << G4endl; }
  }
  else
  { 
    (*fHelCollection)[fCellID[copyIDinZ][copyIDinPhi]]->AddEdep(edep);
    (*fHelCollection)[fCellID[copyIDinZ][copyIDinPhi]]->SetTrackInformation(aStep->GetTrack());
    if(verboseLevel>0)
    { G4cout << " Energy added to CellID " 
           << copyIDinZ << " " << copyIDinPhi << G4endl; }
  }

  return true;
}

