

#include "TestePhysicsList.hh"

#include "G4MuonMinus.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"

//constructor
TestePhysicsList::TestePhysicsList():
  G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  //RegisterPhysics(new G4EmStandardPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  
  // Livermore physics
  RegisterPhysics(new G4EmLivermorePhysics);
}

//destructor

TestePhysicsList::~TestePhysicsList()
{ 
}

//set energy cuts
void TestePhysicsList::SetCuts()
{
  
  //G4VUserPhysicsList::SetCuts();
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(1*keV, 2*GeV); //futuramente usar 250*eV com Garfield++
  /*
  // fixe lower limit for cut
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
  
  // call base class method to set cuts which default value can be
  // modified via /run/setCut/* commands
  G4VUserPhysicsList::SetCuts();
  
  1DumpCutValuesTable();
  //default production thresholds for world volume
  SetCutsWithDefault();
  
  //Production thresholds for detector regions
  G4Region* region;
  G4String* regName;
  G4ProductionCuts* cuts;
  
  regName = "Helmholtz";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProcuctionCuts(); //distância ou energia?
  region->SetProductionCuts(cuts);
  */
  
}  
