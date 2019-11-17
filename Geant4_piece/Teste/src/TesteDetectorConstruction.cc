//INITIALIZATION
#include "TesteDetectorConstruction.hh"
#include "TesteMagneticField.hh"
#include "TesteRegionInformation.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4ThreeVector.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"

#include "G4VSensitiveDetector.hh"
#include "G4MultiSensitiveDetector.hh"
#include "HelmholtzHit.hh"
#include "HelmholtzSD.hh"
//#include "ElectricFieldHit.hh"
//#include "ElectricFieldSD.hh"

#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4AutoDelete.hh"

#include "G4String.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
//#include "SystemOfUnits.h"
#include "G4ios.hh"
#include "G4Colour.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
//#include "G4BestUnit.hh"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
//WTF
G4ThreadLocal TesteMagneticField* TesteDetectorConstruction::fMagneticField = 0;
//G4ThreadLocal TesteElectricField* TesteDetectorConstruction::fElectricField = 0;
G4ThreadLocal G4FieldManager* TesteDetectorConstruction::fFieldMgr = 0;
//G4ThreadLocal G4FieldManager* TesteDetectorConstruction::electFieldMgr = 0;

/*Para criar um volume:
	-sólido: dá forma e dimensões
	-volume lógico: dá propriedades e características físicas, como, materiais, elementos sensitiveis a detectores, campo
	 magnético
	-volume físico: dá a posição do volume
*/

//WTF 1: construtor
TesteDetectorConstruction::TesteDetectorConstruction():
  G4VUserDetectorConstruction(),
  fScoringVolume(0),
  cilindLog(0)
{}


//WTF 2: destrutor
TesteDetectorConstruction::~TesteDetectorConstruction()
{}



//implementação da classe TesteDetectorConstruction
//retorna um ponteiro para um objeto do tipo G4VPhysicalVolume
G4VPhysicalVolume* TesteDetectorConstruction::Construct(){
	//World box, que contém todos os outros volumes
	G4double world_hx = 6.0*m; //-2 < hx < +2
	G4double world_hy = 3.0*m; //-1.0 < hy < +1.0
	G4double world_hz = 1.5*m; //-0.5 < hz < +0.5

	G4Box* caixaMundo = new G4Box("WorldBox", world_hx, world_hy, world_hz);
	/*
	//definindo uma mistura de gas fracional: ar
	G4double z1, a1, fractionmass1, density1;
	G4String name1, symbol1;
	G4int ncomponents1;

	a1 = 14.01*g/mole;
	G4Element* elN  = new G4Element(name="Nitrogen",symbol="N" , z1= 7., a1);

	a1 = 16.00*g/mole;
	G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z1= 8., a1);

	density1 = 1.290*mg/cm3;
	G4Material* Air = new G4Material(name="Air  ",density1,ncomponents1=2);
	Air->AddElement(elN, fractionmass1=70*perCent);
	Air->AddElement(elO, fractionmass1=30*perCent);
	*/

	//definindo uma mistura de gas fracional: Ar (70%) + CO2 (30%)
	G4double z, a, fractionmass, density;
	G4String name, symbol;
	G4int ncomponents, natoms;
	//Argon
	a = 39.95*g/mole;
	G4Element* elAr = new G4Element(name="Argon",symbol="Ar" , z=18., a);
	//Argon gas
	density= 1.784*g/L;
	G4Material* ArGas = new G4Material("ArgonGas",z=18.,a,density);
	//Carbon
	a = 12.00*g/mole;
	G4Element* elC =new G4Element(name="Carbon",symbol="C" , z=6., a);
	//Oxygen
	a = 16.00*g/mole;
	G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
	//CO2
	density = 1.976*g/L;
	G4Material* CO2 = new G4Material(name="CO2", density,ncomponents=2);
	CO2->AddElement(elC, natoms=1);
	CO2->AddElement(elO, natoms=2);
	//mistura ArGas + CO2
	density = 1.8416*g/L;
	G4Material* ArMistCo2 = new G4Material("Mistura Ar + CO2", density,ncomponents=2);
	ArMistCo2->AddMaterial(CO2,fractionmass=30*perCent);
	ArMistCo2->AddMaterial(ArGas,fractionmass=70*perCent);
	
	//definindo Air from database
	G4NistManager* man = G4NistManager::Instance();
	G4Material* Air  = man->FindOrBuildMaterial("G4_AIR");
	//man = G4NISTManager::GetPointer;
	G4Material* vacuum = man->FindOrBuildMaterial("G4_Galactic");
	
		
	
	//criando um logical volume (mãe)
	G4LogicalVolume* caixaMundoLog = new G4LogicalVolume(caixaMundo, vacuum, "MundoLogico");

	//colocando o physical volume (mãe) (instancia do logical volume no espaço)
	G4VPhysicalVolume* caixaMundoPhys = new G4PVPlacement(0,G4ThreeVector(0,0,0),caixaMundoLog,"VolumeGlobalPhys",0,false,0); //como já é o logical volume mãe, deve ter um ponteiro para logical volume mãe nulo
	//... = new G4PVPlacement(rotaçao, vetor de posiçao,logical volume, nome do physical volume, logical volume "mãe", operaçoes booleanas, numero de cópia)
	G4Region* defaultRegion = (*(G4RegionStore::GetInstance()))[0];
  	TesteRegionInformation* defaultRInfo = new TesteRegionInformation();
  	defaultRInfo->SetWorld();
  	defaultRInfo->Print();
  	defaultRegion->SetUserInformation(defaultRInfo);
	
	
	
	//Bobina de Hemlholtz
	//world volume filha: cilindro de raio 20cm
	G4double innerRadius = 0.*cm;
   	G4double outerRadius = 40.*cm; //original = 20.*cm
   	G4double hz = outerRadius/2; //metade da altura = outerRadius/2 = 10.*cm
   	G4double startAngle = 0.*deg;
   	G4double spanningAngle = 360.*deg;
	
   	G4Tubs* cilindVol
   	  = new G4Tubs("CilindroVolume",
   	               innerRadius,
   	               outerRadius,
   	               hz,
   	               startAngle,
   	               spanningAngle);
	//logical volume filha
	cilindLog = new G4LogicalVolume(cilindVol, ArMistCo2, "CilindroLogico"); //definido no .hh
	//physical volume filha
	G4double pos_x = 0.0*meter;
	G4double pos_y = 0.0*meter;
	G4double pos_z = 0.0*meter;

	G4VPhysicalVolume* cubinhoPhys 
	= new G4PVPlacement(0,                       // no rotation (em relação à mãe)
		      G4ThreeVector(pos_x, pos_y, pos_z),
		                               // translation position
		      cilindLog,              // its logical volume
		      "MeuCilindro",               // its name
		      caixaMundoLog,                // its mother (logical) volume
		      false,                   // no boolean operations
		      0);                      // its copy number
	G4Region* HelmholtzRegion = new G4Region("HelmholtzRegion");
  	TesteRegionInformation* HelmholtzInfo = new TesteRegionInformation();
        HelmholtzInfo->SetHelmholtz();
        HelmholtzRegion->SetUserInformation(HelmholtzInfo);
        cilindLog->SetRegion(HelmholtzRegion);
        HelmholtzRegion->AddRootLogicalVolume(cilindLog);
	
	
	
	
	/*
	//exemplo de world volume (filha) 5x5x5cm3
	G4Box* cubinhoVol = new G4Box("cubinho", 2.5*cm, 2.5*cm, 2.5*cm);  
	G4LogicalVolume* cubinhoLog = new G4LogicalVolume(cubinhoVol, ArMistCo2, "CubinhoLogico"); //exemplo de logical volume (filha)
	//colocando o physical volume (filha) (instancia do logical volume no espaço)
	G4double pos_x = 0.0*meter;
	G4double pos_y = 0.0*meter;
	G4double pos_z = 0.0*meter;

	G4VPhysicalVolume* cubinhoPhys 
	= new G4PVPlacement(0,                       // no rotation (em relação à mãe)
		      G4ThreeVector(pos_x, pos_y, pos_z),
		                               // translation position
		      cubinhoLog,              // its logical volume
		      "MeuCubinho",               // its name
		      caixaMundoLog,                // its mother (logical) volume
		      false,                   // no boolean operations
		      0);                      // its copy number
		      
		      */
	//WTF 3
	fScoringVolume = cilindLog;
	
	
	
	//atributos de visualização
	//volume mãe
	/*G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	visAttributes->SetVisibility(false);
	caixaMundoLog->SetVisAttributes(visAttributes);*/
	
	G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	visAttributes->SetVisibility(false);
	caixaMundoLog->SetVisAttributes(visAttributes);
	
	//cilindro com aros vermelho
	visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray 0.9,0.9,0.9
	//visAttributes->SetVisibility(true);			      // purple 1.0, 0.0, 0.7
	visAttributes->SetForceWireframe(true);
    	cilindLog->SetVisAttributes(visAttributes);
    	/*
    	G4VisAttributes* caloLayer_logVisAtt = new G4VisAttributes(G4Colour(0.7,1.0,0.0));
  	caloLayer_logVisAtt->SetForceWireframe(true);
  	caloLayer_log->SetVisAttributes(caloLayer_logVisAtt);
    	*/
	
	
	//retorna o physical volume mãe
	return caixaMundoPhys; 
}

void TesteDetectorConstruction::ConstructSDandField()
{
	// sensitive detectors -----------------------------------------------------
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4String SDname;

	G4VSensitiveDetector* Helmholtz = new HelmholtzSD(SDname="/Helmholtz");
	SDman->AddNewDetector(Helmholtz);
	cilindLog->SetSensitiveDetector(Helmholtz);
	
	
	//SDname = "/ElectricField";
	//G4VSensitiveDetector* ElectricField = new ElectricFieldSD(SDname="ElectricField");
	//SDman->AddNewDetector(ElectricField);
	//cilindLog->SetSensitiveDetector(Helmholtz);
	
	
	//campo magnetico dentro do cilindro
	//cria o campo (opção 1)
	/*
	G4ThreeVector fieldValue = G4ThreeVector(0.,0.,0.5*mT);
	fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
	fMagFieldMessenger->SetVerboseLevel(1);
	*/
	/*
	//cria o campo (opção 2)
	G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0.*tesla,0.*tesla,0.5e-4*tesla)); //no sentido e direção do eixo z 
	//cria field manager e define como campo padrão
	G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
	fieldMgr->SetDetectorField(magField);
	//cria objetos para calcular a tragetória
	fieldMgr->CreateChordFinder(magField);
	//muda a precisão do volume de intersecção
	//fieldMgr->GetChordFinder()->SetDeltaChord(1.e-5);
	//seleciona volume lógico com o campo
	cilindLog->SetFieldManager(fieldMgr, true);
	//registra o campo e o manager para serem deletados automaticamente
	G4AutoDelete::Register(magField);
	G4AutoDelete::Register(fieldMgr);
	*/
	
	//cria o campo (opção 3)
	fMagneticField = new TesteMagneticField(); //constructor tem como padrão fBz 0.5e-4*tesla
	G4double Bmodule = fMagneticField->GetField();
	G4cout << "THAT'S THE MAGNETIC FIELD MODULE: " << Bmodule/tesla << " tesla" << G4endl;
	//salvando a intensidade do campo no arquivo "meuOutput.txt"
	ofstream myfile;
        myfile.open("meuOutput.txt", fstream::app);
	myfile << Bmodule/tesla << G4endl;
        myfile.close();
	
	fFieldMgr = new G4FieldManager();
	fFieldMgr->SetDetectorField(fMagneticField);
	fFieldMgr->CreateChordFinder(fMagneticField);
	
	//fElectricField =new TesteElectricField();
	//fFieldMgr->SetDetectorField(fElectricField);
	//fFieldMgr->CreateChordFinder(fElectricField);
	
	fFieldMgr->GetChordFinder()->SetDeltaChord(0.000001); //miss distance
	//fFieldMgr->SetDeltaIntersection(0.001);		   //delta intersection
	//fFieldMgr->SetDeltaOneStep(0.001);		   //delta one step
	cilindLog->SetFieldManager(fFieldMgr, true);
	
	
	
	//campo elétrico dentro do cilindro
	/*
	fElectricField =new TesteElectricField(); //constructor tem como padrão fEz 10.0*kilovolt/cm
	electFieldMgr = new G4FieldManager();
	electFieldMgr->SetDetectorField(fElectricField);
	electFieldMgr->CreateChordFinder(fElectricField);
	electFieldMgr->GetChordFinder()->SetDeltaChord(1.e-2);
	cilindLog->SetFieldManager(electFieldMgr, true)
	
	G4AutoDelete::Register(fElectricField);
	G4AutoDelete::Register(electFieldMgr);
	*/
	
	//G4AutoDelete::Register(fElectricField);
	G4AutoDelete::Register(fMagneticField);
	G4AutoDelete::Register(fFieldMgr);
	

}



