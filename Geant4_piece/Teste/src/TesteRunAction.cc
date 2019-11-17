

#include "TesteRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"


//constructor
TesteRunAction::TesteRunAction():
  G4UserRunAction()
{;}

//destructor
TesteRunAction::~TesteRunAction()
{}


void TesteRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //inform the runManager to save random number seed
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}


void TesteRunAction::EndOfRunAction(const G4Run*)
{;}

