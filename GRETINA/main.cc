// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 27 04:13:53 2018 (+0800)
// Last-Updated: 五 8月  7 09:39:56 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 63
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
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  TCanvas *c1 = new TCanvas("c1","",600,400);
  c1->Divide(1,4);
  
  TGraph *gg = new TGraph();//waveform
  gg->SetTitle("");
  gg->GetXaxis()->SetTitle("");
  gg->GetYaxis()->SetTitle("");

  TGraph *gg1 = new TGraph();//led
  gg1->SetTitle("");
  gg1->GetXaxis()->SetTitle("");
  gg1->GetYaxis()->SetTitle("");
  gg1->SetMarkerColor(1);
  
  TGraph *gg2 = new TGraph();//cfd
  gg2->SetTitle("");
  gg2->GetXaxis()->SetTitle("");
  gg2->GetYaxis()->SetTitle("");
  gg2->SetMarkerColor(2);
  
  TGraph *gg3 = new TGraph();//trapezoidal
  gg3->SetTitle("");
  gg3->GetXaxis()->SetTitle("");
  gg3->GetYaxis()->SetTitle("");
  gg3->SetMarkerColor(3);
  
  TGraph *gg4 = new TGraph();//pole-zero cancellation
  gg4->SetTitle("");
  gg4->GetXaxis()->SetTitle("");
  gg4->GetYaxis()->SetTitle("");
  gg4->SetMarkerColor(4);

  TGraph *gg5 = new TGraph();//baseline restoration
  gg5->SetTitle("");
  gg5->GetXaxis()->SetTitle("");
  gg5->GetYaxis()->SetTitle("");
  gg5->SetMarkerColor(5);
  
  double data[10000];
  double data1[10000];
  double data2[10000];
  double data3[10000];
  double data4[10000];
  double data5[10000];

  double tau = 2000;//2000
  int datapoint = 6000;
  double offset = 1000;
  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 1000)
	{
	  data[i] = offset+gRandom->Gaus(0, 30);
	}
      else
	{
	  data[i] = offset+gRandom->Gaus(0, 30)+5000*TMath::Exp(-(i-1000)/tau);
	}

      if(i >= 3500) data[i] += 3000*TMath::Exp(-(i-3500)/tau);
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  int m = 200;
  int k = 200;

  int blk = 3000;
  
  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 2*m+k)
	{
	  data3[i] = 0;
	}
      else
	{
	  data3[i] = data3[i-1]+data[i]+data[i-2*m-k]-data[i-m]-data[i-m-k];
	}
    }

  double temp = 0;
  for (int i = 0; i < datapoint; ++i)
    {
      temp += data3[i];
      data4[i] = data3[i]+temp/tau;
    }

  
  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 1)
	{
	  data5[i] = 0;
	}
      else
	{
	  data5[i] = (data5[i-1]+blk*data4[i])/(1+blk);
	}
    }

  
  for (int i = 0; i < datapoint; ++i)
    {
      gg->SetPoint(i,i,data[i]);
      gg3->SetPoint(i,i,data3[i]);
      gg4->SetPoint(i,i,data4[i]);
      gg5->SetPoint(i,i,data5[i]);
    }
  
  c1->cd(1);
  gg->Draw("AP");

  c1->cd(2);
  gg3->Draw("AP");

  c1->cd(3);
  gg4->Draw("AP");

  c1->cd(4);
  gg5->Draw("AP");
  
  c1->Update();
  
  
  
  // and enter the event loop...
  theApp->Run();
  delete theApp;

  return 0;
}

// 
// main.cc ends here
