// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 27 04:13:53 2018 (+0800)
// Last-Updated: 二 5月 25 17:25:33 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 106
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

#include <fstream>
#include <iostream>
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

  TGraph *gg6 = new TGraph();//gaus
  gg6->SetTitle("");
  gg6->GetXaxis()->SetTitle("");
  gg6->GetYaxis()->SetTitle("");
  gg6->SetMarkerColor(4);
  
  double data[10000];
  double data1[10000];
  double data2[10000];
  double data3[10000];
  double data4[10000];
  double data5[10000];
  double data6[10000];
  double data7[10000];
  double datapz[10000];
  double databl[10000];
  
  double tau = 5000;//2000
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
	  data[i] = offset+gRandom->Gaus(0, 30)+3000*TMath::Exp(-(i-1000)/tau);
	}

      if(i >= 2500) data[i] += 3000*TMath::Exp(-(i-2500)/tau);

      if(i >= 3000) data[i] += 3000*TMath::Exp(-(i-3000)/tau);


      if(i >= 4500) data[i] += 3000*TMath::Exp(-(i-4500)/tau);
      if(i >= 4800) data[i] += 3000*TMath::Exp(-(i-4800)/tau);
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  int m = 400;
  int k = 80;

  int blk = 3000;

  int kgaus = 100;


  double gausstau = 100;
  double aT = 1/gausstau;
  double alpha = TMath::Exp(-1/gausstau);

  double max1,max2,max3,max4,max5,max6;

  max6 = -1e308;
  for (int i = 0; i < datapoint; ++i)
    {
      // if(i < 2)
      // 	{
      // 	  data6[i] = 0;
      // 	}
      // else
      // 	{
      // 	  // data6[i] = ((kgaus+2*kgaus*kgaus)*data6[i-1]-kgaus*kgaus*data6[i-2]+2*(data[i]-offset))/(double(1+kgaus+kgaus*kgaus));

      // 	  // data6[i] = (-(kgaus+kgaus*kgaus)*data6[i-2]+kgaus*data6[i-1]+2*(data[i-1]-offset))/(double(kgaus+kgaus*kgaus));
      // 	}

      if(i< 29)
	{
	  data7[i] = 0;
	}
      else
	{
	  data7[i] = 0;
	  for (int j = 0; j < 10; ++j)
	    {
	      data7[i] += (data[i-j]-data[i-20-j]);
	    }

	  data7[i] /= 10;
	}

      
      if(i < 5)
	{
	  data6[i] = 0;
	}
      else
	{
	  data6[i] =5*alpha*data6[i-1]-10*alpha*alpha*data6[i-2]+10*alpha*alpha*alpha*data6[i-3]-5*alpha*alpha*alpha*alpha*data6[i-4]+alpha*alpha*alpha*alpha*alpha*data6[i-5]+(1.0/24)*(-aT*alpha+4*alpha)*data[i-1]+(1.0/24)*(-11*aT*alpha*alpha+12*alpha*alpha)*data[i-2]+(1.0/24)*(-12*alpha*alpha*alpha-11*aT*alpha*alpha*alpha)*data[i-3]+(1.0/24)*(-aT*alpha*alpha*alpha*alpha-4*alpha*alpha*alpha*alpha)*data[i-4];
	}
      
    }


  double temp = 0;
  for (int i = 0; i < datapoint; ++i)
    {
      temp += data6[i];
      datapz[i] = data6[i]+temp/tau;
    }

  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 1)
	{
	  databl[i] = 0;
	}
      else
	{
	  databl[i] = (databl[i-1]+blk*datapz[i])/(1+blk);
	}

      if(databl[i] > max6 && i < 2000) max6 = databl[i];
    }


  
  for (int i = 0; i < datapoint; ++i)
    {
      if(i < 2*m+k)
	{
	  data3[i] = 0;
	  data1[i] = 0;
	  data2[i] = 0;
	}
      else
	{
	  data3[i] = data3[i-1]+data[i]+data[i-2*m-k]-data[i-m]-data[i-m-k];
	  data1[i] = data[i]+data[i-2*m-k]-data[i-m]-data[i-m-k];

	  for (int j = 0; j < m; ++j)
	    {
	      data2[i] += (data[i-j]-0.9*data[i-m-k-j]);
	    }
	}
    }

  temp = 0;
  max4 = -1e308;
  for (int i = 0; i < datapoint; ++i)
    {
      temp += data3[i];
      data4[i] = data3[i]+temp/tau;
      
      if(i< 4000 && data4[i] > max4) max4 = data4[i];
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
      gg1->SetPoint(i,i,data1[i]);
      gg2->SetPoint(i,i,data2[i]);
      gg3->SetPoint(i,i,data7[i]);
      gg4->SetPoint(i,i,data4[i]/max4);
      gg5->SetPoint(i,i,data5[i]);
      gg6->SetPoint(i,i,databl[i]/max6);
    }
  
  c1->cd(1);
  gg->Draw("AP");

  c1->cd(2);
  gg3->Draw("AP");

  c1->cd(3);
  gg4->Draw("AP");

  c1->cd(4);
  gg6->Draw("AP");
  
  c1->Update();


  std::ofstream dataw;//
  dataw.open("filter.txt");//ios::bin ios::app
  if(!dataw.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  for (int i = 0; i < datapoint; ++i)
    {
      // dataw<<i<<" "<<data[i]<<" "<<data4[i]/max4<<" "<<databl[i]/max6<<std::endl;
      dataw << i << " " <<data[i]<<" "<< data4[i]/max4<<std::endl;
    }
  dataw.close();
  
  
  
  // and enter the event loop...
  theApp->Run();
  delete theApp;

  return 0;
}

// 
// main.cc ends here
