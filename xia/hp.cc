// hp.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 3月 12 15:05:27 2022 (+0800)
// Last-Updated: 日 3月 20 16:43:30 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 34
// URL: http://wuhongyi.cn 

#include <iostream>
#include "TMath.h"
#include <iomanip>

void hp()
{
  double tau1 = 55.0;//us
  double tau2 = 1000;//us
  double beta = 0.00;
  double m = 504;
  double g = 160;
  double deltat = 10;//ns

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  double F = (beta*tau1)/((1-beta)*tau2);
  
  double b1 = TMath::Exp(-deltat/1000.0/tau1);
  double b2 = TMath::Exp(-deltat/1000.0/tau2);

  double r1 = 1.0/(1-b1);
  double r2 = 1.0/(1-b2);

  double A1g = (1-TMath::Power(b1,g))/(1-b1);
  double A2g = (1-TMath::Power(b2,g))/(1-b2);
  
  double A1m = (1-TMath::Power(b1,m))/(1-b1);
  double A2m = (1-TMath::Power(b2,m))/(1-b2);

  double A1m2 = (1-TMath::Power(b1,m/2))/(1-b1);
  double A2m2 = (1-TMath::Power(b2,m/2))/(1-b2);

  double a1g = A1g*(1-TMath::Power(b1,-g));
  double a2g = A2g*(1-TMath::Power(b2,-g));
  
  double a1m = A1m*(1-TMath::Power(b1,-m-g));
  double a2m = A2m*(1-TMath::Power(b2,-m-g));
  
  double a1m2 = A1m2*(1-TMath::Power(b1,m/2));
  double a2m2 = A2m2*(1-TMath::Power(b2,m/2));


  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  double ele[16] = {
		    (1-beta)*A1m+beta*A2m, -A1m-F*A2m, a1m, a2m,
		    ((1-beta)*A1g+beta*A2g)/r1, -(2-TMath::Power(b1,g))-F*r2/r1*(2-TMath::Power(b2,g)), a1g/r1, a2g/r1,
		    -(1-beta)*a1m2-beta*a2m2, a1m2+F*a2m2, -a1m2, -a2m2,
		    0, 0, -TMath::Power(b1,-m-g)*a1m2, -TMath::Power(b2,-m-g)*a2m2

  };





  
  double a[16] = {12,36,-12,95,36,12,65,48,-65,78,-15,-98,46,36,95,-15};
  
  TMatrixT<double> mat(4, 4, ele);
  
  mat.Print();

  double det = -1;
  TMatrixT<double> mattmp = mat.Invert(&det);
  std::cout<<det<<std::endl;
  mattmp.Print();


  std::cout<<setprecision(16) <<mattmp(0,0)<<"  "<<mattmp(0,1)/r1<<"  "<<mattmp(0,2)<<"  "<<mattmp(0,3)<<std::endl;
}

// 
// hp.cc ends here
