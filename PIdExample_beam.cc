#include <stdio.h>
#include <iostream>
#include "VDDCRec/kdcvd.h"
#include "VDDCRec/ktracks.h"
#include "KrdEdxPId/KrdEdxPId.hh"
#include "KDisplay/kdisplay_event.h"
#include "KrMu/mu_system.h"
#include "KrMu/mu_event.h"
#include "KrMu/mu_status.h"
#include <TApplication.h>
#include "TH1.h"
#include "TCanvas.h"

using namespace std; 
int main() {
  // default parameters
  int run=23665,Nevents=20000,cosmic=0;
  //int run=23665,Nevents=4000,cosmic=0;
  // for command string arguments
  int iarg;
  // for event reading in and reconstructuion
  char fname[80];
  int openerr,runerr,readerr,closeerr,recselect=0;
  // for access to tracks  

  TApplication* theApp=new TApplication("App", NULL, NULL);
  TCanvas* cP1=new TCanvas("TEST Ampl graph1","A Graph",1800,700);
  TH1F* p_mu=new TH1F("HistMuProb","Prob",150,-0.1,1.1);
  TH1F* p_pi=new TH1F("HistPiProb","Prob",150,-0.1,1.1);
  TH1F* p_K=new TH1F("HistKProb","Prob",150,-0.1,1.1);
  TH1F* p_p=new TH1F("HistpProb","Prob",150,-0.1,1.1);
  
  TH1F* dedx_mu=new TH1F("HistMudedx","dedx",100,0.0,100);
  TH1F* dedx_pi=new TH1F("HistPidedx","dedx",100,0.0,100);
  TH1F* dedx_K=new TH1F("HistKdedx","dedx",100,0.0,100);
  TH1F* dedx_p=new TH1F("Histpdedx","dedx",100,0.0,100);

  dcdedxpidinit(&run);
  bool msk[6];
  for(int i=0;i<6;i++) msk[i]=true;
  msk[4]=false; //proton
  //msk[3]=false; //kaon
  //msk[2]=false; //pion
  msk[1]=false; //muon
  msk[0]=false; //electron
  pidinitpar(0.00001,msk);
  //kDClayerOff(7,0);
  // kDClayerOff(6,0);
  // kDClayerOff(1,3);
  // general initialization
 if(mu_default_init(1))  cout<<"Mu init error"<<endl;
   if(mu_init_status())  cout<<"Mu init status error"<<endl;
   
 kdcvdnocosmic();
 //kdcvdcosmic();

 //sprintf(fname,"/space/runs/daq%06d.nat",run);
 sprintf(fname,"/store/users/ovtin/outDmeson/simulation/outsimD0_kppp/Signal/simDmeson000001.dat");
 ksimreal(1,run,run);
 kdcsimxt();
 kdcsimsigma();

 kdcsimsysterr();
 kdcscalesysterr(1.0);
 kdcscalesysterraz(6.0, 0.0);

 
  kedr_open_nat(fname,&openerr);
  if( openerr) {
    printf("open error for %s\n",fname);
    exit(1);
  }
  if(mu_get_db_status(run))  cout<<"Mu status error"<<endl;
  if(mu_get_db_clbr_for_run(run)<0)  cout<<"Mu calib error"<<endl;
  while(1) 
  {
    // read event
    kedr_read_nat(&readerr);
    if(readerr) break;
    // event reconstruction
    kdcvdrec(0,&recselect);
    cout<<"************************** "<<eNumber<<endl;
    pidevent();
    //kdisplay_event();
    for(int t=0; t<eTracksAll; t++) {
      // if(tP(t)<800.){
	unsigned short MuNhitsRaw = mu_next_event_good();
	//	cout<<"MuNhitsRaw  "<<MuNhitsRaw <<endl;
      //if(dcinfo.st[t]==1&&tP(t)>100.&&MuNhitsRaw!=0){
      if(tP(t)<500.){
	cout<<"Ptr "<<tP(t)<<" dedx "<<dcinfo.dedx[t]<<" res "<<dcinfo.res[t][1]<<" hypo "<<dcinfo.bhypo[t]<<endl;
	//if(dcinfo.p[t][1]>0.0) { p_mu->Fill(dcinfo.p[t][1]); dedx_mu->Fill(dcinfo.dedx[t]); } //muon
	if(dcinfo.p[t][2]>0.0) { p_pi->Fill(dcinfo.p[t][2]); dedx_pi->Fill(dcinfo.dedx[t]); } //pion
	if(dcinfo.p[t][3]>0.0) { p_K->Fill(dcinfo.p[t][3]);  dedx_K->Fill(dcinfo.dedx[t]); } //kaon
	//if(dcinfo.p[t][4]>0.0) { p_p->Fill(dcinfo.p[t][4]);  dedx_p->Fill(dcinfo.dedx[t]); } //proton

	//	kdisplay_event();
      }
    }
    if(eNumber > Nevents) 
      {
	cout<<"break \n";
	break;
      }
  }
   kedr_close_nat(&closeerr);

   cP1->Divide(2,1);

   cP1->cd(1);
   p_pi->Draw();
   p_K->SetLineColor(kRed);
   p_K->Draw("same");
   cP1->SetFillColor(0);
   
   cP1->cd(2);
   dedx_pi->Draw();  
   dedx_K->SetLineColor(kRed);
   dedx_K->Draw("same");

   cP1->Update();
   //cP1->SaveAs("/store/users/ovtin/outDmeson/D0kppp/dedx/dedx_beam_exp_wirtCutprob0.45.png");
   //cP1->SaveAs("/store/users/ovtin/outDmeson/D0kppp/dedx/dedx_beam_sim_wirtCutprob0.45.png");
   //cP1->SaveAs("/store/users/ovtin/outDmeson/D0kppp/dedx/dedx_beam_exp.png");
   cP1->SaveAs("/store/users/ovtin/outDmeson/D0kppp/dedx/dedx_beam_sim.png");
   theApp->Run();
   theApp->Terminate();  
}
