// patent.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 25 18:14:04 2021 (+0800)
// Last-Updated: 一 10月 25 20:01:16 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
// URL: http://wuhongyi.cn 

{

  TCanvas *c1 = new TCanvas("c1","c1",600,400);
  

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
  gg5->SetMarkerColor(6);  

  double data[10000];
  double data1[10000];
  double data2[10000];
  double data3[10000];
  double data4[10000];
  double data5[10000];

  double tau = 2000;//2000
  int datapoint = 6000;
  double offset = 1000;

  int L1 = 200;
  int L2 = 50;
  
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

      if(i >= 3500) data[i] += 5000*TMath::Exp(-(i-3500)/tau);
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

  for (int i = 0; i < datapoint; ++i)
    {
      // if(i < L1) data1[i] = data[i];
      // else data1[i] = data[i-L1];

      // data2[i] = data[i]-data1[i];

      // if(i < L2) data3[i] = data2[i];
      // else data3[i] = data2[i-L2];

      // data4[i] = data2[i]-data3[i];

      // if(i == 0) data5[i] = 0;
      // else data5[i] = data5[i-1]+data4[i];

      // if(i == 0) data3[i] = 0;
      // else data3[i] = data3[i-1]+data2[i];

      // if(i < L2) data4[i] = data3[i];
      // else data4[i] = data3[i-L2];

      // data5[i] = data3[i]-data4[i];


      if(i< 2*L1+L2)
	{
	  data1[i] = 0;
	  data2[i] = 0;
	}
      else
	{
	  for (int j = 0; j < L1; ++j)
	    {
	      data1[i] = data1[i]+data[i-j]-data[i-j-L1-L2];
	    }

	  data2[i] = data2[i-1]+data[i]-data[i-L1]-data[i-L1-L2]+data[i-2*L1-L2];
	}
      
    }
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

  for (int i = 0; i < datapoint; ++i)
    {
      gg->SetPoint(i, i, data[i]);//i = 0~N-1
      gg1->SetPoint(i, i, data1[i]);
      gg2->SetPoint(i, i, data2[i]);
      // gg3->SetPoint(i, i, data3[i]);
      // gg4->SetPoint(i, i, data4[i]);
      // gg5->SetPoint(i, i, data5[i]);
    }

  c1->Divide(1,2);
  c1->cd(1);
  gg->Draw("AP");

  c1->cd(2);
  gg1->Draw("AP");
  gg2->Draw("Psame");

  
  
  c1->Update();
}

// 
// patent.cc ends here
