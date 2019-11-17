
#include "TesteTrackingAction.hh"
#include "TesteTrajectory.hh"
#include "TesteTrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"

//constructor
TesteTrackingAction::TesteTrackingAction():
  G4UserTrackingAction()
{;}

void TesteTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  
  // Create trajectory only for track in tracking region
  TesteTrackInformation* trackInfo = (TesteTrackInformation*)(aTrack->GetUserInformation());

  if(trackInfo->GetTrackingStatus() > 0)
  {
    fpTrackingManager->SetStoreTrajectory(true);
    fpTrackingManager->SetTrajectory(new TesteTrajectory(aTrack));
  }
  else
  { fpTrackingManager->SetStoreTrajectory(false); }
  
}

//Post user tracking action
void TesteTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    TesteTrackInformation* info = 
      (TesteTrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        TesteTrackInformation* infoNew = new TesteTrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
}


