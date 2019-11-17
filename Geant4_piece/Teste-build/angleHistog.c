#include "TH1F.h"
#include "TVector3.h"
#include "TCanvas.h"
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <Riostream.h>
using namespace std;

std::string to_string(double i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}


void angleHistog(){
	double Bz;
	double ipx;
	double ipy;
	double ipz;
	double iEnergy; //é a mesma em todos os eventos
	double fpx;
	double fpy;
	double fpz;
	double fEnergy;
	Double_t meuAngulo;
	double histMean;
	//criando histograma
	TH1F* meuHist = new TH1F("h" , "Frequencia dos angulos de deflexao", 100,0.,100.);
	//criando um canvas para o histograma
	TCanvas *mycanvas=new TCanvas();
	mycanvas->SetGrid();
	//arquivo dos vetores e energia vetores
	ifstream theFile;
	theFile.open("meuOutput.txt");
	TVector3 pIn;
	TVector3 pOut;

	//arquivo dos angulos
	//ofstream anglesFile;
	//anglesFile.open("myAngles.txt", fstream::app);
	
	//o primeiro valor é o campo magnético em tesla
	theFile >> Bz;
	
	//leitura do arquivo
	while(!theFile.eof()){
		theFile >> ipx >> ipy >> ipz >> iEnergy >> fpx >> fpy >> fpz >> fEnergy;
		pIn.SetXYZ(ipx,ipy,ipz);
		pOut.SetXYZ(fpx,fpy,fpz);
		meuAngulo = pIn.Angle(pOut);
		cout << fixed << setprecision(2);
		cout << "Ângulo de deflexão entre vetores de entrada e saída da bobina de Helmholtz é: "
		       << meuAngulo 
		       << " radianos ou " << meuAngulo*180./3.1415 << " graus "
		       << endl;
		//salva os ângulos em outro arquivo para fazer histograma depois
		//anglesFile << meuAngulo*180./3.1415 << G4endl;
		//necessário mudar nome do arquivo especificando energia, campo magnético
		//e no fim apagar conteúdo de anglesFile
		meuHist->Fill(meuAngulo*180./3.1415);
	}
	meuHist->SetTitle("Frequencia dos angulos de deflexao");
	meuHist->Draw();
	
	/* OPÇÃO 1
	string sEnergy = to_string(iEnergy) + "MeV_";
	string sMagneticField = to_string(Bz) + "tesla";
	string nome_do_arquivo_novo = sEnergy + sMagneticField + ".txt";
	*/
	//OPÇÃO 2
	std::string Snome_do_arquivo_novo;
	Snome_do_arquivo_novo.append(to_string(iEnergy));
	Snome_do_arquivo_novo.append("MeV_");
	Snome_do_arquivo_novo.append(to_string(Bz));
	Snome_do_arquivo_novo.append("tesla");
	//Snome_do_arquivo_novo.append(".txt");
	cout << Snome_do_arquivo_novo << endl;
	
	//versão char array
	//char nome_do_arquivo_novo[] = iEnergy + "MeV_" + Bz + "tesla.txt CHAR";
	//cout << nome_do_arquivo_novo[] << endl;
	//printf("%s",nome_do_arquivo_novo);
  	
  	
	char meuCharArray[30]; //p[s.length()]
  
	
	for (int i = 0; i < sizeof(meuCharArray); i++) { 
		meuCharArray[i] = Snome_do_arquivo_novo[i]; 
		cout << meuCharArray[i]; 
	} 
	//nome do .txt
	char meuCharArrayTXT[30];
	strcpy(meuCharArrayTXT,meuCharArray);
	strcat(meuCharArrayTXT,".txt");
	//nome da imagem
	char meuCharArrayPNG[30];
	strcpy(meuCharArrayPNG,meuCharArray);
	strcat(meuCharArrayPNG,".png");
	//nome do arquivo .root
	char meuCharArrayROOT[30];
	strcpy(meuCharArrayROOT,meuCharArray);
	strcat(meuCharArrayROOT,".root");
	
	/*OPÇÃO 3: SAMUEL
	stringstream nome_do_arquivo_novo;
	nome_do_arquivo_novo << iEnergy << "MeV_" << Bz << "tesla.txt";
	*/
	
	mycanvas->Print(meuCharArrayPNG); //"Histograma_dos_angulos.png"
	//cout << nome_do_arquivo_novo;
	//salvando em arquivo root
	TFile out_fileI(meuCharArrayROOT,"RECREATE"); //TFile out_fileI("my_Histog.root","NEW");
	meuHist->Write();
	out_fileI.ls(); //lista conteúdo nese diretório (neste TFile)
	out_fileI.Close();
	//renomeando arquivo
	rename("meuOutput.txt", meuCharArrayTXT);
	
	//anglesFile.close();
	theFile.close();
	
	//salvando média do histograma num arquivo
	histMean = meuHist->GetMean();
	//nome do arquivo contendo as médias dos ângulos para as energias num dado campo magnético Bz
	std::string meanValString;
	meanValString.append("HistMeanValues_");
	meanValString.append(to_string(Bz));
	meanValString.append("Tesla.txt");
	char HistogramMeanValues[30];
	for (int i = 0; i < sizeof(HistogramMeanValues); i++) { 
		HistogramMeanValues[i] = meanValString[i]; 
		cout << HistogramMeanValues[i]; 
	} 
	//abre arquivo e salva valores
	ofstream MeanFile;
	MeanFile.open(HistogramMeanValues, fstream::app);
	MeanFile << iEnergy << "\t" << histMean << "\t" << "0.0" << "\t" << "0.5" << endl;
	MeanFile.close();
	
	
}

