// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 27 04:13:53 2018 (+0800)
// Last-Updated: 三 6月 27 07:39:47 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 46
// URL: http://wuhongyi.cn 

#include "RVersion.h"//版本判断
#include "TApplication.h"
#include "TBenchmark.h"
#include "TBrowser.h"
#include "TCutG.h"
#include "TDatime.h"
#include "TError.h"
#include "TList.h"
#include "TObject.h"
#include "TRint.h"
#include "TROOT.h"
#include "TSlider.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TTimeStamp.h"
#include "THttpServer.h"

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TFrame.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TProfile.h"

#include "TF1.h"
#include "TF2.h"
#include "TFitResult.h"
#include "TFormula.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TVector3.h"
#include "TVectorD.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // gStyle->SetOptStat(0);//不显示统计框
  // c1->ToggleEventStatus();//底端信息栏
  // c1->ToggleEditor();
  // c1->ToggleToolBar();
  // c1->Clear("");
  // c1->Divide(/*col*/,/*raw*/);
  // c1->SetGridx();//SetGridy();
  // c1->SetLogx();//SetLogy(); SetLogz();
  // c1->SetName("");

  // gg->SetLineColor(Color_t);
  // gg->SetLineStyle(Style_t);
  // gg->SetLineWidth(Width_t);//1
  // gg->SetFillStyle(Style_t);
  // gg->SetFillColor(Color_t);
  // gg->SetMarkerColor(Color_t);
  // gg->SetMarkerStyle(Style_t);
  // gg->SetMarkerSize(Size_t);
  // gg->SetMinimum(Double_t);
  // gg->SetMaximum(Double_t);
  // gg->IsInside(Double_t x, Double_t y);// int   判断（x，y）是否在TCut选定的范围
  // gg->Eval(Double_t x);//double 获得x对应的y值
  // gg->GetN();//int 获得点数
  
  
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  TCanvas *c1 = new TCanvas("c1","",600,400);
  c1->Divide(1,2);
  
  TGraph *gg = new TGraph();
  gg->SetTitle("");
  gg->GetXaxis()->SetTitle("");
  gg->GetYaxis()->SetTitle("");

  TGraph *gg1 = new TGraph();
  gg1->SetTitle("");
  gg1->GetXaxis()->SetTitle("");
  gg1->GetYaxis()->SetTitle("");
  gg1->SetMarkerColor(1);
  
  TGraph *gg2 = new TGraph();
  gg2->SetTitle("");
  gg2->GetXaxis()->SetTitle("");
  gg2->GetYaxis()->SetTitle("");
  gg2->SetMarkerColor(2);
  
  TGraph *gg3 = new TGraph();
  gg3->SetTitle("");
  gg3->GetXaxis()->SetTitle("");
  gg3->GetYaxis()->SetTitle("");
  gg3->SetMarkerColor(3);
  
  TGraph *gg4 = new TGraph();
  gg4->SetTitle("");
  gg4->GetXaxis()->SetTitle("");
  gg4->GetYaxis()->SetTitle("");
  gg4->SetMarkerColor(4);
  
  double data[10000];
  double crrc1[10000];
  double crrc2[10000];
  double crrc3[10000];
  double crrc4[10000];

  
  double tau = 200000;//2000
  int datapoint = 6000;
  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 1000)
	{
	  data[i] = gRandom->Gaus(0, 30);
	}
      else
	{
	  data[i] = gRandom->Gaus(0, 30)+5000*TMath::Exp(-(i-1000)/tau);
	}

      // if(i >= 3500) data[i] += 3000*TMath::Exp(-(i-3500)/tau);
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  bool flag = true;
  
  double gausstau = 100;
  double aT = 1/gausstau;
  double alpha = TMath::Exp(-1/gausstau);

  double max1,max2,max3,max4;

  max1 = -1e308;
  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 2)
	{
	  crrc1[i] = 0;
	}
      else
	{
	  crrc1[i] = 2*alpha*crrc1[i-1]-alpha*alpha*crrc1[i-2]+data[i]-alpha*(1+aT)*data[i-1];
	}
      
      if(crrc1[i] > max1) max1 = crrc1[i];
    }

  max2 = -1e308;
  for (int i = 0; i < datapoint; ++i)//error
    {
      if(i < 3)
	{
	  crrc2[i] = 0;
	}
      else
	{
	  crrc2[i] = 3*alpha*crrc2[i-1]-3*alpha*alpha*crrc2[i-2]+alpha*alpha*alpha*crrc2[i-3]+alpha*(1-aT/2)*data[i-1]-alpha*alpha*(1+aT/2)*data[i-2];
	}
      
      if(crrc2[i] > max2) max2 = crrc2[i];
    }

  max3 = -1e308;
  for (int i = 0; i < datapoint; ++i)//error
    {
      if(i < 4)
	{
	  crrc3[i] = 0;
	}
      else
	{
	  crrc3[i] = 4*alpha*crrc3[i-1]-6*alpha*alpha*crrc3[i-2]+4*alpha*alpha*alpha*crrc3[i-3]-alpha*alpha*alpha*alpha*crrc3[i-4]+alpha*(1.0/2-aT*aT/6)*data[i-1]-2.0/3*aT*alpha*alpha*data[i-2]-alpha*alpha*alpha*(1.0/2+aT/6)*data[i-3];
	}

      if(crrc3[i] > max3) max3 = crrc3[i];
    }

  max4 = -1e308;
  for (int i = 0; i < datapoint; ++i)//error
    {
      if(i < 5)
	{
	  crrc4[i] = 0;
	}
      else
	{
	  crrc4[i] =5*alpha*crrc4[i-1]-10*alpha*alpha*crrc4[i-2]+10*alpha*alpha*alpha*crrc4[i-3]-5*alpha*alpha*alpha*alpha*crrc4[i-4]+alpha*alpha*alpha*alpha*alpha*crrc4[i-5]+(1.0/24)*(-aT*alpha+4*alpha)*data[i-1]+(1.0/24)*(-11*aT*alpha*alpha+12*alpha*alpha)*data[i-2]+(1.0/24)*(-12*alpha*alpha*alpha-11*aT*alpha*alpha*alpha)*data[i-3]+(1.0/24)*(-aT*alpha*alpha*alpha*alpha-4*alpha*alpha*alpha*alpha)*data[-4];
	}

      if(crrc4[i] > max4) max4 = crrc4[i];
    }

  

  for (int i = 0; i < datapoint; ++i)
    {
      gg->SetPoint(i, i, data[i]);//i = 0~N-1
      
      if(flag)
	{
	  gg1->SetPoint(i, i, crrc1[i]/max1);
	  gg2->SetPoint(i, i, crrc2[i]/max2);
	  gg3->SetPoint(i, i, crrc3[i]/max3);
	  gg4->SetPoint(i, i, crrc4[i]/max4);
	}
      else
	{
	  gg1->SetPoint(i, i, crrc1[i]);
	  gg2->SetPoint(i, i, crrc2[i]);
	  gg3->SetPoint(i, i, crrc3[i]);
	  gg4->SetPoint(i, i, crrc4[i]);
	}

    }

  c1->cd(1);
  gg->Draw("AP");

  c1->cd(2);
  gg1->Draw("AP");
  gg2->Draw("Psame");
  gg3->Draw("Psame");
  gg4->Draw("Psame");
  
  c1->Update();
  
  
  
  // and enter the event loop...
  theApp->Run();
  delete theApp;

  return 0;
}

// 
// main.cc ends here
