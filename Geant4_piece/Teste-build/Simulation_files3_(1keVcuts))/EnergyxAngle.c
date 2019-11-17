#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TArrow.h"

//original com ponteiro
void EnergyxAngle(){
	TCanvas *mycanvas=new TCanvas();
	mycanvas->SetGrid();
	
	//gráfico da tomada de dados
	TGraphErrors *grafico = new TGraphErrors("HistMeanValues_1Tesla.txt","\%lg \%lg \%lg \%lg");
	//grafico->SetTitle("Angulo de deflexao em funcao da energia a %4.2f; E (MeV);Theta (grau)", Bz);
	grafico->SetTitle("Angulo de deflexao em funcao da energia a 2T; E (MeV);Theta (grau)");
	grafico->Draw("AP");
	/*
	//Define a linear function
	TF1 f("Linear law","[0]+x*[1]",0.,50.);
	f.SetLineColor(kRed);
	f.SetLineStyle(1);
	//Fit it to the graph and draw it
	grafico->Fit(&f);
	f.DrawClone("Same");
	*/
	/*
	//Define an logarithmic function
	TF1 f("Exponential law","[0]+[1]*log([2]*x)",0.,50.);
	f.SetLineColor(kRed);
	f.SetLineStyle(1);
	//Fit it to the graph and draw it
	grafico->Fit(&f);
	f.DrawClone("Same");
	*/
	/*
	//gráfico teórico esperado
	TF1 *teorico = new TF1("f2","[0] + [1]*x",0.,10.);
	teorico->SetParameter(0,0.);	//(parameter number, parameter value)
	teorico->SetParameter(1,1.79950427);
	teorico->SetLineColor(kBlue);
	teorico->SetLineStyle(2);
	teorico->DrawClone("Same");
	*/
	/*
	//build and draw a legend (não funcionou)
	TLegend *leg(.1,.7,.3,.9,"Legenda");
	leg->SetFillColor(0);
	leg->AddEntry(f,"Pontos experimentais");		//curva medida
	leg->AddEntry(teorico,"Curva teorica esperada"); 	//curva esperada
	leg->DrawClone("Same");
	*/
	
	mycanvas->Print("Deflexao_por_energia_1Tesla.png");
	
	
}
