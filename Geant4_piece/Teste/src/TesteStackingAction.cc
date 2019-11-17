
#include "TesteStackingAction.hh"
#include "TesteTrackInformation.hh"
#include "HelmholtzHit.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"    
#include "G4ios.hh"

//constructor
TesteStackingAction::TesteStackingAction():
   G4UserStackingAction(),
   fStage(0),
   fHelmholtzHitsColID(-1)
{;}

//destructor
TesteStackingAction::~TesteStackingAction()
{;}

//WTF 1
G4ClassificationOfNewTrack TesteStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  G4ClassificationOfNewTrack classification = fUrgent;

  if(fStage==0)
  {
    TesteTrackInformation* trackInfo;
    if(aTrack->GetTrackStatus()==fSuspend) // Track reached to Helmholtz
    {
      trackInfo = (TesteTrackInformation*)(aTrack->GetUserInformation());
      trackInfo->SetTrackingStatus(0);
      trackInfo->SetSourceTrackInformation(aTrack);
//      G4cout << "Track " << aTrack->GetTrackID() 
//             << " (parentID " << aTrack->GetParentID() 
//             << ") has reached to Helmholtz and has been suspended at " 
//             << aTrack->GetPosition() << G4endl;
      classification = fWaiting;
    }
    else if(aTrack->GetParentID()==0) // Primary particle
    {
      trackInfo = new TesteTrackInformation(aTrack);
      trackInfo->SetTrackingStatus(1);
      G4Track* theTrack = (G4Track*)aTrack;
      theTrack->SetUserInformation(trackInfo);
    }
  }
  return classification;
}

//WTF 2
G4VHitsCollection* TesteStackingAction::GetHelCollection()
{
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  G4RunManager* runMan = G4RunManager::GetRunManager();
  if(fHelmholtzHitsColID<0)
  { fHelmholtzHitsColID = SDMan->GetCollectionID("HelCollection"); }
  if(fHelmholtzHitsColID>=0)
  {
    const G4Event* currentEvent = runMan->GetCurrentEvent();
    G4HCofThisEvent* HCE = currentEvent->GetHCofThisEvent();
    return HCE->GetHC(fHelmholtzHitsColID);
  }
  return 0;
}

//WTF 3
void TesteStackingAction::NewStage()
{
  // G4cout << "+++++++++++ Stage " << fStage << G4endl;
  if(fStage==0)
  {
    // display trajetory information in the tracking region
    G4cout << G4endl;
    G4cout << "Tracks in tracking region have been processed. -- Stage 0 over." 
           << G4endl;
    G4cout << G4endl;
  }
  else
  {
    // display Helmholtz information caused by a "source" track 
    // in the tracker region
    //    G4cout << G4endl;
    //    G4cout << "Processing one shower originated by a source track "
    //           << "in tracker region is over." << G4endl;
    //    G4cout << G4endl;
    HelmholtzHitsCollection* CHC = (HelmholtzHitsCollection*)GetHelCollection();
    if(CHC)
    { 
      int n_hit = CHC->entries();
      G4double totE = 0;
      G4int n_hitByATrack = 0;
      for(int i=0;i<n_hit;i++)
      {
        G4double edepByATrack = (*CHC)[i]->GetEdepByATrack();
        if(edepByATrack>0.)
        {
          totE += edepByATrack;
          if(n_hitByATrack==0)
          { (*CHC)[i]->GetTrackInformation()->Print(); }
          n_hitByATrack++;
          G4cout << "Cell[" << (*CHC)[i]->GetZ() << "," 
                 << (*CHC)[i]->GetPhi() << "]    " 
                 << edepByATrack/GeV << " [GeV]" << G4endl;
          (*CHC)[i]->ClearEdepByATrack();
        }
      }
      if(n_hitByATrack>0)
      {
        G4cout << 
          "###  Total energy deposition in Helmholtz by a source track in "
               << n_hitByATrack << " cells : " << totE / GeV << " (GeV)" 
               << G4endl;
        G4cout << G4endl;
      }
    }
  }

  if(stackManager->GetNUrgentTrack())
  {
    // Transfer all tracks in Urgent stack to Waiting stack, since all tracks
    // in Waiting stack have already been transfered to Urgent stack before
    // invokation of this method.
    stackManager->TransferStackedTracks(fUrgent,fWaiting);

    // Then, transfer only one track to Urgent stack.
    stackManager->TransferOneStackedTrack(fWaiting,fUrgent);

    fStage++;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......     
void TesteStackingAction::PrepareNewEvent()
{ 
  fStage = 0; 
}
