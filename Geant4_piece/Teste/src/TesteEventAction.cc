



#include "TesteEventAction.hh"

//CONSERTAR
#include "HelmholtzHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "TesteTrajectory.hh"
#include "G4SystemOfUnits.hh"   


#include "G4Event.hh"
#include "G4RunManager.hh"



TesteEventAction::TesteEventAction():
  G4UserEventAction(),
  fHelmholtzCollID(-1)
{} 



TesteEventAction::~TesteEventAction()
{}



void TesteEventAction::BeginOfEventAction(const G4Event*)
{    
  
  //CONSERTAR
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  if(fHelmholtzCollID<0)
  {
    G4String colNam;
    fHelmholtzCollID = SDman->GetCollectionID(colNam="helmholtzCollection");
    
  }
}



void TesteEventAction::EndOfEventAction(const G4Event* evt)
{   
  //CONSERTAR
  G4cout << ">>> Summary of Event " << evt->GetEventID() << G4endl;
  if(evt->GetNumberOfPrimaryVertex()==0)
  {
    G4cout << "Event is empty." << G4endl;
    return;
  }
  
  if(fHelmholtzCollID<0) return;

  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  HelmholtzHitsCollection* HHC = 0;
  if(HCE)
  {
    HHC = (HelmholtzHitsCollection*)(HCE->GetHC(fHelmholtzCollID));
  }

  if(HHC)
  {
    int n_hit = HHC->entries();
    G4cout << G4endl;
    G4cout << "Helmholtz hits "<<
      "--------------------------------------------------------------" << G4endl;
    G4cout << n_hit << " hits are stored in HelmholtzHitsCollection." << G4endl;
    G4double totE = 0;
    for(int i=0;i<n_hit;i++)
    { totE += (*HHC)[i]->GetEdep(); }
    G4cout << "     Total energy deposition in Helmholtz coil : " << totE / GeV << " (GeV)" << G4endl;
  }

  // get number of stored trajectories
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  // extract the trajectories and print them out
  G4cout << G4endl;
  G4cout << "Trajectories in tracker "<< "--------------------------------------------------------------" << G4endl;
  if(fpEventManager->GetVerboseLevel()>0)
  {
    for(G4int i=0; i<n_trajectories; i++) 
    {
      TesteTrajectory* trj = (TesteTrajectory*)((*(evt->GetTrajectoryContainer()))[i]);
      trj->ShowTrajectory();
    }
  }
    
  G4cout << G4endl;
  /*
  G4cout << "Primary particles "<< "--------------------------------------------------------------" << G4endl;
  G4int n_vertex = evt->GetNumberOfPrimaryVertex();
  for(G4int iv=0;iv<n_vertex;iv++)
  {
    G4PrimaryVertex* pv = evt->GetPrimaryVertex(iv);
    G4cout << G4endl;
    G4cout << "Primary vertex "
           << G4ThreeVector(pv->GetX0(),pv->GetY0(),pv->GetZ0())
           << "   at t = " << (pv->GetT0())/ns << " [ns]" << G4endl;
    if(fpEventManager->GetVerboseLevel()>0)
    {
      G4PrimaryParticle* pp = pv->GetPrimary();
      while(pp)
      {
        PrintPrimary(pp,0);
        pp = pp->GetNext();
      }
    }
  }
  */
}
