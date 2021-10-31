// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 27 04:13:53 2018 (+0800)
// Last-Updated: 日 10月 31 20:45:29 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 87
// URL: http://wuhongyi.cn 

//g++ `root-config --cflags` main.cc  `root-config --glibs` -l Spectrum -o 123


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

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SecondOrderIIR(int32_t data_in, int32_t *data_out,
		    int32_t b0,  int32_t b1 ,  int32_t b2,
		    int32_t a1,  int32_t a2
		    )
{

  int32_t temp=0; //fixed point math: 16 bit integer + 16 bit fractional
  int32_t out_upscaled=0; //fixed point math: 16 bit integer + 16 bit fractional

  static int16_t  m_x2 =0; // x[n-2]
  static int32_t m_y2 =0; // y[n-2] //fixed point math: 16 bit integer + 16 bit fractional
  static int32_t m_y2_b =0; // y[n-2] //fixed point math: 16 bit integer + 16 bit fractional

  static int16_t m_x1 =0;
  static int32_t m_y1 =0; //fixed point math: 16 bit integer + 16 bit fractional
  static int32_t m_y1_b =0; //fixed point math: 16 bit integer + 16 bit fractional


  // static int32_t c_b0 = 0; //fixed point math: 24 bit integer + 4 bit fractional
  // static int32_t c_b1 = 0; //fixed point math: 24 bit integer + 4 bit fractional
  // static int32_t c_b2 = 0; //fixed point math: 24 bit integer + 4 bit fractional
  // static int32_t c_a1 = 0; //fixed point math: 24 bit integer + 4 bit fractional
  // static int32_t c_a2 = 0; //fixed point math: 24 bit integer + 4 bit fractional

  int N = 16777215;
  double c_b0 = b0/(double)N; 
  double c_b1 = b1/(double)N; 
  double c_b2 = b2/(double)N; 
  double c_a1 = a1/(double)N; 
  double c_a2 = a2/(double)N; 
  
  out_upscaled = c_b0*(data_in)
    + c_b1*(m_x1)
    + c_b2*(m_x2)
    + c_a1*(m_y1)
    + c_a2*(m_y2);

  temp = out_upscaled;
  std::cout<<temp<<std::endl;

  // *data_out =temp.range()>>16;
  *data_out = temp;
  m_x2 = m_x1;
  m_y2 = m_y1;
  m_x1 = data_in;
  m_y1 = temp;

}


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

  TCanvas *c1 = new TCanvas("c1","c1",600,400);
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

  TGraph *gg5 = new TGraph();
  gg5->SetTitle("");
  gg5->GetXaxis()->SetTitle("");
  gg5->GetYaxis()->SetTitle("");
  gg5->SetMarkerColor(5);

  TGraph *giir = new TGraph();
  giir->SetTitle("");
  giir->GetXaxis()->SetTitle("");
  giir->GetYaxis()->SetTitle("");
  giir->SetMarkerColor(6);

  
  TGraph *pz = new TGraph();
  pz->SetTitle("");
  pz->GetXaxis()->SetTitle("");
  pz->GetYaxis()->SetTitle("");
  pz->SetMarkerColor(2);
  
  TGraph *bl = new TGraph();
  bl->SetTitle("");
  bl->GetXaxis()->SetTitle("");
  bl->GetYaxis()->SetTitle("");
  bl->SetMarkerColor(3);
  
  int32_t data[10000];
  double crrc1[10000];
  double crrc2[10000];
  double crrc3[10000];
  double crrc4[10000];
  double crrc5[10000];
  
  double datapz[10000];
  double databl[10000];

  int32_t iir[10000];
  
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

  bool flag = false;
  
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
	  crrc4[i] =5*alpha*crrc4[i-1]-10*alpha*alpha*crrc4[i-2]+10*alpha*alpha*alpha*crrc4[i-3]-5*alpha*alpha*alpha*alpha*crrc4[i-4]+alpha*alpha*alpha*alpha*alpha*crrc4[i-5]+(1.0/24)*(-aT*alpha+4*alpha)*data[i-1]+(1.0/24)*(-11*aT*alpha*alpha+12*alpha*alpha)*data[i-2]+(1.0/24)*(-12*alpha*alpha*alpha-11*aT*alpha*alpha*alpha)*data[i-3]+(1.0/24)*(-aT*alpha*alpha*alpha*alpha-4*alpha*alpha*alpha*alpha)*data[i-4];
	}

      if(crrc4[i] > max4) max4 = crrc4[i];
    }


  for (int i = 0; i < datapoint; ++i)
    {
      SecondOrderIIR(data[i],&iir[i],356671,-713342,356671,-638033,-1016141);

      
    }

  
  
  for (int i = 0; i < datapoint; ++i)
    {
      if(i==0) crrc5[i] = data[i]-offset;
      else crrc5[i] = crrc5[i-1]+(data[i]-offset)-(data[i-1]-offset)*TMath::Exp(-1.0/tau);
	
    }

  

  double temp = 0;
  for (int i = 0; i < datapoint; ++i)
    {
      temp += crrc1[i];
      datapz[i] = crrc1[i]+temp/tau;
    }

  int blk = 3000;
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
	  gg5->SetPoint(i, i, crrc5[i]);
	}
      else
	{
	  gg1->SetPoint(i, i, crrc1[i]);
	  gg2->SetPoint(i, i, crrc2[i]);
	  gg3->SetPoint(i, i, crrc3[i]);
	  gg4->SetPoint(i, i, crrc4[i]);
	  gg5->SetPoint(i, i, crrc5[i]);
	}

      giir->SetPoint(i,i,iir[i]);
      pz->SetPoint(i,i,datapz[i]);
      bl->SetPoint(i,i,databl[i]);
    }

  c1->cd(1);
  gg->Draw("AP");

  c1->cd(2);
  // gg1->Draw("AP");
  // gg2->Draw("Psame");
  // gg3->Draw("Psame");
  giir->Draw("AP");
  // gg5->Draw("AP");
  
  // pz->Draw("Psame");
  // bl->Draw("AP");

  // pz->Draw("AP");
  
  c1->Update();
  
  
  
  // and enter the event loop...
  theApp->Run();
  delete theApp;

  return 0;
}

// 
// main.cc ends here
