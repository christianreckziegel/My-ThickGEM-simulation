
#include "TesteSteppingAction.hh"
#include "TesteRegionInformation.hh"
#include "TesteTrackInformation.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Region.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"

//#include "TesteMySession.hh"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "G4UIsession.hh"

#include "TVector3.h"
#include "TFile.h"

using namespace std;

//constructor
TesteSteppingAction::TesteSteppingAction():
    G4UserSteppingAction()
{;}

//destructor
TesteSteppingAction::~TesteSteppingAction()
{;}

//user stepping action
void TesteSteppingAction::UserSteppingAction(const G4Step * theStep)
{
  // Suspend a track if it is entering into the Helmholtz

  // check if it is alive
  G4Track * theTrack = theStep->GetTrack();
  if(theTrack->GetTrackStatus()!=fAlive) { return; }

  // get region information
  G4StepPoint * thePrePoint = theStep->GetPreStepPoint();
  G4LogicalVolume * thePreLV = thePrePoint->GetPhysicalVolume()->GetLogicalVolume();
  TesteRegionInformation* thePreRInfo = (TesteRegionInformation*)(thePreLV->GetRegion()->GetUserInformation());
  G4StepPoint * thePostPoint = theStep->GetPostStepPoint();
  G4LogicalVolume * thePostLV = thePostPoint->GetPhysicalVolume()->GetLogicalVolume();
  TesteRegionInformation* thePostRInfo = (TesteRegionInformation*)(thePostLV->GetRegion()->GetUserInformation());
  
  /* Modelo para cópia
  G4Track* track = step->GetTrack();
  double Energy = track->GetKineticEnergy();
  G4ThreeVector momentum = track->GetMomentumDirection();
  double px = momentum.x();
  double py = momentum.y();
  double pz = momentum.z();
  double mModule = sqrt( pow(px,2) + pow(py,2) + pow(pz,2) );
  double angx = acos(px/mModule);
  double angy = acos(py/mModule);
  double angz = acos(pz/mModule);
  */
  G4ThreeVector iMomentum;
  G4ThreeVector fMomentum;
  double ipx;
  double ipy;
  double ipz;
  double fpx;
  double fpy;
  double fpz;
  double iModule;
  double fModule;
  double iEnergy;
  double fEnergy;
  //criando histograma
  //TH1F* meuHist = new TH1F("h" , "histograma teste", 20,0.,5.);
  
  //G4cout << fixed << setprecision(3);
  //OPÇÃO 1
  // check if it is entering to the Helmholtz volume
  if(!(thePreRInfo->IsHelmholtz()) && (thePostRInfo->IsHelmholtz())) 
  {
    
    // if the track had already been suspended at the previous step, let it go.
    TesteTrackInformation* trackInfoIn = static_cast<TesteTrackInformation*>(theTrack->GetUserInformation());
    trackInfoIn->SetSourceTrackInformation(theTrack);
    G4cout << G4endl;
    G4cout << "Entering the Helmholtz volume\n";
    //print() source and original information: track ID, definition, energy, position; -> PROBLEMA
    trackInfoIn->Print();
    G4cout << G4endl;
    
    //Get initial momentum entering the Helmholtz coil
    iMomentum = theTrack->GetMomentumDirection();
    ipx = (iMomentum).x();
    ipy = (iMomentum).y();
    ipz = (iMomentum).z();
    iModule = sqrt( pow(ipx,2) + pow(ipy,2) + pow(ipz,2) );
    iEnergy = theTrack->GetKineticEnergy();
    G4cout << "Momentum entering the Helmholtz coil:"
           << "(px,py,pz) = "
           << "(" << ipx << "," << ipy << "," << ipz << ") "
           << G4endl;
           
    
    
    //para salvar arquivo usando C++
    //OPÇÃO 1       
    ofstream myfile;
    myfile.open("meuOutput.txt", fstream::app);
    //myfile << "#Momentum entering the Helmholtz coil\n";
    //myfile << "(px,py,pz) = " << "(" << ipx << "," << ipy << "," << ipz << ")\n";
    myfile << ipx <<  	"	" << ipy <<  	"	" << ipz <<  	"	" << iEnergy << "	";
    myfile.close();
    
    /*//OPÇÃO 2
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    std::cout << "TESTE DE OUTPUT 1";  //output to the file out.txt
    std::cout.rdbuf(coutbuf); //reset to standard output again
    std::cout << "TESTE DE OUTPUT 2";  //output to the standard input	*/
    
    
    //TesteMySession* LoggedSession = new TesteMySession("meuOutput.txt", "meuErrput.txt");
    //LoggedSession->ReceiveG4cout("Momentum leaving the Helmholtz coil");
    
  }
  // check if it is getting out of the Helmholtz volume
  if((thePreRInfo->IsHelmholtz()) && !(thePostRInfo->IsHelmholtz())) 
  {
    // if the track had already been suspended at the previous step, let it go.
    //converte o objeto do tipo GetUserInformation para o objeto do tipo TesteTrackInformation
    TesteTrackInformation* trackInfoOut = static_cast<TesteTrackInformation*>(theTrack->GetUserInformation());
    trackInfoOut->SetSourceTrackInformation(theTrack);
    G4cout << G4endl;
    G4cout << "Getting out of the Helmholtz volume\n";
    //print() source and original information: track ID, definition, energy, position; -> PROBLEMA
    trackInfoOut->Print();
    G4cout << G4endl;
    
    //Get final momentum leaving the Helmholtz coil
    fMomentum = theTrack->GetMomentumDirection();
    fpx = (fMomentum).x();
    fpy = (fMomentum).y();
    fpz = (fMomentum).z();
    fModule = sqrt( pow(fpx,2) + pow(fpy,2) + pow(fpz,2) );
    fEnergy = theTrack->GetKineticEnergy();
    G4cout << "Momentum leaving the Helmholtz coil:"
           << "(px,py,pz) = "
           << "(" << fpx << "," << fpy << "," << fpz << ") "
           << G4endl;
    
    //1 tab = 7 espaços
    
    //para salvar dados no arquivo usando C++       
    ofstream myfile;
    myfile.open("meuOutput.txt", fstream::app);
    //myfile << "#Momentum leaving the Helmholtz coil\n";
    //myfile << "(px,py,pz) = " << "(" << fpx << "," << fpy << "," << fpz << ")\n";
    myfile << fpx <<  	"	" << fpy <<  	"	" << fpz <<  	"	" << fEnergy << G4endl;
    myfile.close();
    
    //ângulo entre dois vetores
    //ifstream theFile;
    //theFile.open("meuOutput.txt");
    //TVector3 pIn;
    //TVector3 pOut;
    //vector<double> angulo;
    
    //ofstream anglesFile;
    //anglesFile.open("myAngles.txt", fstream::app);
    //leitura do arquivo
    /*
    while(!theFile.eof()){
      theFile >> ipx >> ipy >> ipz >> iEnergy >> fpx >> fpy >> fpz >> fEnergy;
      pIn.SetXYZ(ipx,ipy,ipz);
      pOut.SetXYZ(fpx,fpy,fpz);
      Double_t meuAngulo = pIn.Angle(pOut);
      G4cout << fixed << setprecision(2);
      G4cout << "Ângulo de deflexão entre vetores de entrada e saída da bobina de Helmholtz é: "
           << meuAngulo 
           << " radianos ou " << meuAngulo*180./3.1415 << " graus "
           << endl;
      //salva os ângulos em outro arquivo para fazer histograma depois
      //anglesFile << meuAngulo*180./3.1415 << G4endl;
      //necessário mudar nome do arquivo especificando energia, campo magnético
      //e no fim apagar conteúdo de anglesFile
      //meuHist->Fill(meuAngulo*180./3.1415);
    }
    */
    //anglesFile.close();
    
    //theFile.close();
    
    
      
    
    //OBS.:
    //cálculo do angulo deve ser dentro do segundo if(saindo da Helmholtz), caso
    //contrário, o mesmo cálculo será feito para todos os steps,
    //ou seja, mesmo quando ainda não existe vetor de saída, pois
    //a partícula não saiu ainda
    
    
      
  }
  
  
  
  
}
