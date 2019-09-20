// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  8 19:21:20 2016 (+0800)
// Last-Updated: 五 9月 20 19:31:40 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 474
// URL: http://wuhongyi.cn 

#include "wuReadData.hh"
#include "offline.hh"
#include "pkuFFTW.hh"

#include <iostream>
#include <climits>
#include <sstream>
#include <cstring>

#include "RVersion.h"//版本判断
#include "TApplication.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TBenchmark.h"
#include "TBranch.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TColor.h"
#include "TCutG.h"
#include "TDatime.h"
#include "TError.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TFormula.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TList.h"
#include "TLorentzVector.h"
#include "TMarker.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TMultiGraph.h"
#include "TNtuple.h"
#include "TObject.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TRint.h"
#include "TROOT.h"
#include "TSlider.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimer.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TVector3.h"
#include "TVectorD.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{

  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  //初始参数
  TString TreeName="tree";//这里为要处理的文件中 tree name
  // create first the chain with all the files
  TChain *fChain=new TChain(TreeName);
  // std::cout << "Creating the chain" << std::endl;
  fChain->SetCacheSize(100*1024*1024);
  TString dir = gSystem->DirName(__FILE__);//获取当前文件main.cc所在路径 
  dir.ReplaceAll("/./","/");
  // std::cout<<dir<<std::endl;
  //=======================================================
  //以下两个选一个： 手动填写root文件所在路径或者直接使用当前文件所在路径
  // gSystem->Setenv("Dir","/home/wuhongyi");//手动填写路径
  // gSystem->Setenv("Dir",dir);//当前文件路径
  int AddFileNumber = wuReadData::ReadValue<int>("AddFileNumber","ReadData.txt");
  // std::cout<<"AddFileNumber: "<<AddFileNumber<<std::endl;
  //=======================================================
  std::string InputFile;
  for (int i = 0; i < AddFileNumber; ++i)
    {
      char InputFileName[1024];
      sprintf(InputFileName,"InputFileName_%d",i);
      InputFile = wuReadData::ReadValue<std::string>(InputFileName,"ReadData.txt");
      fChain->Add(InputFile.c_str());
    }
  // fChain->Print();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  Bool_t          pileup;
  Bool_t          outofr;
  Short_t         cid;
  Short_t         sid;
  Short_t         ch;
  UShort_t        evte;
  ULong64_t       ts;
  ULong64_t       ets;
  UShort_t        cfd;
  Bool_t          cfdft;
  UInt_t          trae;
  UInt_t          leae;
  UInt_t          gape;
  Double_t        base;
  UInt_t          qs[8];
  UShort_t        ltra;
  UShort_t        data[10000];   //[ltra]
  UShort_t        dt[10000];   //[ltra]
  Int_t           nevt;

  // Int_t           ch;
  // Int_t           size;
  // UInt_t          timestamp;
  // Int_t           dt[65535];   //[size]
  // Int_t           data[65535];   //[size]
  
  TBranch        *b_pileup;   //!
  TBranch        *b_outofr;   //!
  TBranch        *b_cid;   //!
  TBranch        *b_sid;   //!
  TBranch        *b_ch;   //!
  TBranch        *b_evte;   //!
  TBranch        *b_ts;   //!
  TBranch        *b_ets;   //!
  TBranch        *b_cfd;   //!
  TBranch        *b_cfdft;   //!
  TBranch        *b_trae;   //!
  TBranch        *b_leae;   //!
  TBranch        *b_gape;   //!
  TBranch        *b_base;   //!
  TBranch        *b_qs;   //!
  TBranch        *b_ltra;   //!
  TBranch        *b_data;   //!
  TBranch        *b_dt;   //!
  TBranch        *b_nevt;   //!
   
  fChain->SetBranchAddress("pileup", &pileup, &b_pileup);
  // fChain->SetBranchAddress("outofr", &outofr, &b_outofr);
  // fChain->SetBranchAddress("cid", &cid, &b_cid);
  fChain->SetBranchAddress("sid", &sid, &b_sid);
  fChain->SetBranchAddress("ch", &ch, &b_ch);
  // fChain->SetBranchAddress("evte", &evte, &b_evte);
  // fChain->SetBranchAddress("ts", &ts, &b_ts);
  // fChain->SetBranchAddress("ets", &ets, &b_ets);
  // fChain->SetBranchAddress("cfd", &cfd, &b_cfd);
  // fChain->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
  // fChain->SetBranchAddress("trae", &trae, &b_trae);
  // fChain->SetBranchAddress("leae", &leae, &b_leae);
  // fChain->SetBranchAddress("gape", &gape, &b_gape);
  // fChain->SetBranchAddress("base", &base, &b_base);
  // fChain->SetBranchAddress("qs", qs, &b_qs);
  fChain->SetBranchAddress("ltra", &ltra, &b_ltra);
  fChain->SetBranchAddress("data", data, &b_data);
  fChain->SetBranchAddress("dt", dt, &b_dt);
  fChain->SetBranchAddress("nevt", &nevt, &b_nevt);
   
  std::cout <<std::endl<< "=== Running Hongyi Wu Analysis ===" << std::endl;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  gBenchmark->Start("tree");//计时开始
  // std::string OutputFileName = wuReadData::ReadValue<string>("OutputFileName","ReadData.txt");
  Long64_t TotalEntry = fChain->GetEntries();//拿到TChain中总entry行数
  std::cout<<"TotalEntry:  "<<TotalEntry<<std::endl;
  
  int SelectChannel = wuReadData::ReadValue<int>("SelectChannel","ReadData.txt");
  int VotoChannel = wuReadData::ReadValue<int>("VotoChannel","ReadData.txt");
  int VotoTime = wuReadData::ReadValue<int>("VotoTime","ReadData.txt");
  offline *off = new offline();

  std::string OutputFileName = wuReadData::ReadValue<std::string>("OutputFileName","ReadData.txt");
  
  bool PulsePolarity;
  if(wuReadData::ReadValue<int>("PulsePolarity","ReadData.txt") == 1)
    PulsePolarity = true;
  else
    PulsePolarity = false;
  int ADCMSPS = wuReadData::ReadValue<int>("ADCMSPS","ReadData.txt");

  int CalculateVertexPoint = wuReadData::ReadValue<int>("CalculateVertexPoint","ReadData.txt");
  int CalculateBaselinePoint = wuReadData::ReadValue<int>("CalculateBaselinePoint","ReadData.txt");
  int CalculateRiseTimeType = wuReadData::ReadValue<int>("CalculateRiseTimeType","ReadData.txt");
  double FastFilterFL = wuReadData::ReadValue<double>("FastFilterFL","ReadData.txt");
  double FastFilterFG = wuReadData::ReadValue<double>("FastFilterFG","ReadData.txt");
  int FastFilterThre = wuReadData::ReadValue<int>("FastFilterThre","ReadData.txt");
  double SlowFilterSL = wuReadData::ReadValue<double>("SlowFilterSL","ReadData.txt");
  double SlowFilterSG = wuReadData::ReadValue<double>("SlowFilterSG","ReadData.txt");
  int SlowFilterRange = wuReadData::ReadValue<int>("SlowFilterRange","ReadData.txt");
  double PreampTau = wuReadData::ReadValue<double>("PreampTau","ReadData.txt");
  double cfddelay = wuReadData::ReadValue<double>("CFDFilterDelay","ReadData.txt"); 
  int cfdscale = wuReadData::ReadValue<int>("CFDFilterScale","ReadData.txt");
  
  off->SetPulsePolarity(PulsePolarity);
  off->SetADCMSPS(ADCMSPS);
  off->SetCalculateVertexPoint(CalculateVertexPoint);
  off->SetCalculateBaselinePoint(CalculateBaselinePoint);
  off->SetCalculateRiseTimeType(CalculateRiseTimeType);//设置上升时间计算方法
  
  off->SetFastFilterPar(FastFilterFL,FastFilterFG,FastFilterThre);//100
  off->SetSlowFilterPar(SlowFilterSL,SlowFilterSG,SlowFilterRange);
  off->SetPreampTau(PreampTau);
  off->SetCfdFilterPar(cfddelay,cfdscale);
  off->PrintFilterPar();


  double datawave[65536];
  int datafastfilter[65536];
  double doubledatawave[65536];
  double datarc[65536];
  double datarccr[65536];
  double datarccr2[65536];
  double datatswma[65536];
  double datagaus[65536];
  double datask[65536];
  
  TCanvas *c1 = new TCanvas("c1","",2400,800);
  // gStyle->SetOptStat(0);//不显示统计框
  c1->ToggleEventStatus();//底端信息栏
  // c1->ToggleEditor();
  // c1->ToggleToolBar();
  // c1->Clear("");
  // c1->Divide(/*col*/,/*raw*/);
  // c1->SetGridx();//SetGridy();
  // c1->SetLogx();//SetLogy(); SetLogz();
  // c1->SetName("");


  TH1I *projectfastfilter = new TH1I("projectfastfilter","",50,-25,25);
  
  TGraph *filter = new TGraph();
  TMultiGraph *gggabc = new TMultiGraph();gggabc->SetName("gggabc");
  TGraph *ggga = new TGraph();ggga->SetName("ggga");
  TGraph *gggb = new TGraph();gggb->SetName("gggb");
  TGraph *gggc = new TGraph();gggc->SetName("gggc");
  TGraph *gggd = new TGraph();gggd->SetName("gggd");
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // large 67 small 266    145

  // if(argc == 1)
  //   fChain->GetEvent(62);
  // if(argc == 2)
  //   {
  //     int eventcount;
  //     std::stringstream ss;//sstream cstring
  //     ss.clear();//重复使用前一定要清空
  //     ss<<argv[1];
  //     ss>>eventcount;
  //     fChain->GetEvent(eventcount);
  //   }
  
  // off->SetEventData(ltra, data);

  // off->GetWaveData(datawave);
  // for (int i = 0; i < ltra; ++i) doubledatawave[i] = datawave[i];

  // // off->module_RC_int_LP(doubledatawave,datarc,int(ltra),10.0,30.0);
  // // off->module_CR_diff_HP(datarc,datarccr,int(ltra),10.0,30.0);
  // // off->module_CR_diff_HP(datarccr,datarccr2,int(ltra),10.0,30.0);

  // off->RC_CR2_Smoothing(doubledatawave,datarc,int(ltra),16);
  // off->RC_CR2_FirstDifferentiation(datarc,datarccr,int(ltra),10);
  // off->RC_CR2_SecondDifferentiation(datarccr,datarccr2,int(ltra),10);
  
  // // off->Two_sampled_wide_Moving_Average(doubledatawave,datatswma,int(ltra));
    
  // off->GetFastFilter(datafastfilter);

  // off->module_Gaussian(doubledatawave,datagaus,int(ltra),0.01,1,2);

  // off->Sallen_Key_Filter(datagaus,datask,int(ltra),3);
  
  // for (int i = 0; i < ltra; ++i)
  //   {
  //     ggga->SetPoint(i,i,datawave[i]);
  //     gggb->SetPoint(i,i,datafastfilter[i]);
  //     gggc->SetPoint(i,i,datarccr2[i]);
  //     gggd->SetPoint(i,i,datask[i]);

  //     // gggd->SetPoint(i,i,datagaus[i]);
  //     // gggc->SetPoint(i,i,data[i]);
  //     // gggd->SetPoint(i,i,datatswma[i]);
  //     // projectfastfilter->Fill(datafastfilter[i]);
  //   }

  // c1->Divide(2,2);
  // c1->cd(1);
  // ggga->Draw("AL");
  // c1->cd(2);
  // gggb->Draw("ALP*");
  // c1->cd(3);
  // gggc->Draw("AL");
  // c1->cd(4);
  // gggd->Draw("AL");
  // // projectfastfilter->Draw();

  // std::cout<<"sid: "<<sid<<"  ch: "<<ch<<"  pileup: "<<pileup<<std::endl;
  
  // c1->Divide(1,3,0.0,0.0);
  // c1->cd(1);
  // ggga->Draw("AL");
  // ggga->GetXaxis()->SetRangeUser(0,3000);
  // c1->cd(2);
  // gggb->Draw("AL");
  //   gggb->GetXaxis()->SetRangeUser(0,3000);
  // c1->cd(3);
  // gggc->Draw("AL");
  // gggc->GetXaxis()->SetRangeUser(0,3000);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TH1I *hhight = new TH1I("hhight","",8000,0,16000);
  TH1I *hgaus = new TH1I("hgaus","",8000,0,16000);
  double temp1,temp2;
  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
    {//循环处理从这里开始
      fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据

      if(entry%1000 == 0) std::cout<<entry<< " / "<< TotalEntry<<std::endl;
      if(ch != 7) continue;

      off->SetEventData(ltra, data);

      off->GetWaveData(datawave);
      for (int i = 0; i < ltra; ++i) doubledatawave[i] = datawave[i];
      off->module_Gaussian(doubledatawave,datagaus,int(ltra),0.01,2,2);
      off->Sallen_Key_Filter(datagaus,datask,int(ltra),3);
      temp1 = -1000000;
      temp2 = -1000000;
      for (int i = 0; i < ltra; ++i)
  	{
	  if(datask[i] > temp1) temp1 = datask[i];
  	  if(datagaus[i] > temp2) temp2 = datagaus[i];
  	}

      hhight->Fill(temp1);
      hgaus->Fill(temp2);
    }

  c1->Divide(1,2);
  c1->cd(1);
  hhight->Draw("");
  c1->cd(2);
  hgaus->Draw("");
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // int CountUnderThreshold = 0;
  // int CountChannelEntey = 0;
  // double tempenergy,tempenergyvote;
      
  // for (Long64_t entry = 0; entry < TotalEntry; ++entry)
  //   {//循环处理从这里开始
  //     fChain->GetEvent(entry);//这个是重点，拿到TChain中第entry行数据
  //     if(entry % 10000 == 0)
  // 	{
  // 	  std::cout<<"Process Event: "<<entry<<std::endl;
  // 	  std::cout<<"UnderThreshold: "<<CountUnderThreshold<<" / "<<CountChannelEntey<<std::endl;
  // 	  CountUnderThreshold = 0;
  // 	  CountChannelEntey = 0;
  // 	}
	    
  //     if(ch != SelectChannel) continue;
  //     CountChannelEntey++;
  //     off->SetEventData(size, data);

  //     // vote
  //     Long64_t timevote1 = timestamp;
  //     Long64_t timevote2;
  //     bool voteflag = false;
  //     if(VotoChannel > -1)
  // 	{
  // 	  for (Long64_t tempvote = entry+1; tempvote < TotalEntry; ++tempvote)
  // 	    {
  // 	      fChain->GetEvent(tempvote);
  // 	      if(ch == SelectChannel) break;
  // 	      if(ch == VotoChannel)
  // 		{
  // 		  timevote2 = timestamp;
  // 		  // if(timevote1 > timevote2) timevote2 = timevote2+((Long64_t)1<<32);
  // 		  if(TMath::Abs(timevote2-timevote1) <= VotoTime)
  // 		    {
  // 		      voteflag = true;
  // 		      break;
  // 		    }
  // 		}
  // 	    }
  // 	}
  //     if(voteflag)
  // 	{
  // 	  // tempenergy = off->GetEnergy();
  // 	  tempenergy = off->GetWaveHigh();
  // 	  off->SetEventData(size, data);
  // 	  tempenergyvote = off->GetEnergy();
  // 	  // tempenergyvote = off->GetWaveHigh();
  // 	  deltaEE->Fill(tempenergyvote,tempenergy);
  // 	  continue;
  // 	}
	  
  //     // if(data[0]<15700 || data[0]>15800) continue;
	  
  //     tempenergy = off->GetEnergy();
  //     // tempenergy = off->GetWaveHigh();
	  
  //     if(tempenergy < 0)
  // 	{
  // 	  CountUnderThreshold++;
  // 	}
  //     else
  // 	{
  // 	  energy->Fill(tempenergy);
	      
  // 	  double risetime = off->GetRiseTime();
  // 	  // if(tempenergy > 1680 && tempenergy < 1720)
  // 	  {
  // 	    time->Fill(risetime);
  // 	    energytime->Fill(risetime,tempenergy);
  // 	  }
  // 	}
	  
  //   }//循环处理到这里结束
  // std::cout<<std::endl;
  // gBenchmark->Show("tree");//计时结束并输出时间

  // c1->Divide(2,2);
  // c1->cd(1);
  // energy->Draw();
  // c1->cd(2);
  // time->Draw();
  // c1->cd(3);
  // energytime->Draw("colz");
  // c1->cd(4);
  // deltaEE->Draw("colz");
  // c1->Update();

  // TFile *outputrootfile = new TFile(OutputFileName.c_str(),"RECREATE");//"RECREATE" "READ"
  // if(!outputrootfile->IsOpen())
  //   {
  //     std::cout<<"Can't open root file"<<std::endl;
  //   }
 
  // // outputrootfile->ls("");
  // // TObject->Write();
  // // TH1D *h = (TH1D*)outputrootfile->Get("name");
  // outputrootfile->cd();
  // c1->Write();
  // energy->Write();
  // time->Write();
  // energytime->Write();
  // deltaEE->Write();
  // outputrootfile->Close();

  





  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
  gBenchmark->Show("tree");//计时结束并输出时间

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}

// 
// main.cc ends here
