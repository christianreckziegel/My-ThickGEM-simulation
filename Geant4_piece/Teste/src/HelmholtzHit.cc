

#include "HelmholtzHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


G4ThreadLocal G4Allocator<HelmholtzHit> * HelmholtzHitAllocator = 0;

//constructor
HelmholtzHit::HelmholtzHit(G4LogicalVolume* logVol,G4int z, G4int phi):
   G4VHit(), fZCellID(z), fPhiCellID(phi), fEdep(0.0), 
   fPos(0),fRot(0.,0.,0.),fPLogV(logVol),fEdepByATrack(0.0),fTrackInfo()
{;}

//dstructor
HelmholtzHit::~HelmholtzHit()
{;}

//draw
void HelmholtzHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Transform3D trans(fRot,fPos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = fPLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4Colour colour(1.,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceWireframe(false);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*fPLogV,attribs,trans);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const std::map<G4String,G4AttDef>* HelmholtzHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("HelmholtzHit",isNew);
  if (isNew) {
    G4String hitType("HitType");
    (*store)[hitType] = G4AttDef(hitType,"Hit Type","Physics","","G4String");

    G4String zCellID("ZCellID");
    (*store)[zCellID] = G4AttDef(zCellID,"Z Cell ID","Physics","","G4int");

    G4String phiCellID("PhiCellID");
    (*store)[phiCellID] = G4AttDef(phiCellID,"Phi Cell ID","Physics","","G4int");

    G4String energy("Energy");
    (*store)[energy] = G4AttDef(energy,"Energy Deposited","Physics","G4BestUnit",
                                "G4double");

    G4String eTrack("ETrack");
    (*store)[eTrack] = G4AttDef(eTrack,"Energy Deposited By Track","Physics",
                                "G4BestUnit","G4double");

    G4String pos("Pos");
    (*store)[pos] = G4AttDef(pos, "Position",
                      "Physics","G4BestUnit","G4ThreeVector");

    G4String lvol("LVol");
    (*store)[lvol] = G4AttDef(lvol,"Logical Volume","Physics","","G4String");
  }

  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<G4AttValue>* HelmholtzHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","HelmholtzHit",""));

  values->push_back
    (G4AttValue("ZCellID",G4UIcommand::ConvertToString(fZCellID),""));

  values->push_back
    (G4AttValue("PhiCellID",G4UIcommand::ConvertToString(fPhiCellID),""));

  values->push_back
    (G4AttValue("Energy",G4BestUnit(fEdep,"Energy"),""));

  values->push_back
    (G4AttValue("ETrack",G4BestUnit(fEdepByATrack,"Energy"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));

  if (fPLogV)
    values->push_back
      (G4AttValue("LVol",fPLogV->GetName(),""));
  else
    values->push_back
      (G4AttValue("LVol"," ",""));

  return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HelmholtzHit::Print()
{
  G4cout << "Cell[" << fZCellID << "," << fPhiCellID << "]    " << fEdep/GeV << " [GeV]" << G4endl;
}


