#include <stdio.h>
#include <iostream>
#include "KDisplay/kdisplay_event.h"
#include "VDDCRec/kdcvd.h"
#include "VDDCRec/ktracks.h"
#include "KrdEdxPId/KrdEdxPId.hh"
#include "KrMu/mu_system.h"
#include "KrMu/mu_event.h"
#include "KrMu/mu_status.h"
#include <TApplication.h>
#include "TH1.h"
#include "TCanvas.h"
#include <argp.h>


using namespace std; 

static char doc[] ="jps exp";
/* A description of the arguments we accept*/
static char args_doc[] = "";
static struct argp_option options[] = {
   {"nrun", 'r', "run",  0, "Number of the Run",2 },
  { 0,0,0,0,0,0 }
};

struct arguments
{
    int nrun;
};
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  union ArG{
    struct arguments *arguments; 
    void *void_arg;
  };
  ArG arg_union;
  arg_union.void_arg=state->input;
  switch (key) {
  case 'r': arg_union.arguments->nrun = atoi(arg); break;
  }
    return 0;
};
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv){
  // default parameters
  int Nevents=10000,cosmic=0;
  // for command string arguments
  int iarg;
  // for event reading in and reconstructuion
  char fname[80];
  int openerr,runerr,readerr,closeerr,recselect=0;
  // for access to tracks  
  struct arguments arguments;
  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  cout << " run number "<<arguments.nrun<< endl;  
  int run=arguments.nrun;
  TApplication* theApp=new TApplication("App", NULL, NULL);
  TCanvas* cP1=new TCanvas("TEST Ampl graph1","A Graph",900,700);
  TH1F* p_mu=new TH1F("HistMuProb","Prob",150,-0.1,15.1);

  dcdedxpidinit(&run);
  bool msk[6];
  for(int i=0;i<6;i++) msk[i]=true;
  //msk[3]=false;
  msk[2]=false;
  msk[0]=false;
  pidinitpar(0.00001,msk);
  
  // general initialization
 if(mu_default_init(1))  cout<<"Mu init error"<<endl;
   if(mu_init_status())  cout<<"Mu init status error"<<endl;
   // kdcvdnocosmic();
kdcvdcosmic();

 sprintf(fname,"/space/runs/daq%06d.nat",run);
// sprintf(fname,"/store/users/offline/Skim/Cosm/cos%06d.nat",run);
 
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
     kdisplay_event();
    for(int t=0; t<eTracksAll; t++) {
      // if(tP(t)<800.){
	unsigned short MuNhitsRaw = mu_next_event_good();
	//	cout<<"MuNhitsRaw  "<<MuNhitsRaw <<endl;
      if(dcinfo.st[t]==1&&tP(t)>100.&&MuNhitsRaw!=0){
	cout<<"Ptr "<<tPt(t)<<" dedx "<<dcinfo.dedx[t]<<" rms "<<dcinfo.rms[t]<<" hypo "<<dcinfo.bhypo[t]<<endl;
	if(dcinfo.p[t][1]>0.0) p_mu->Fill(dcinfo.dedx[t]);
	  }
    }
    if(eNumber > Nevents) 
      {
	cout<<"break \n";
	break;
      }
  }
   kedr_close_nat(&closeerr);
   p_mu->Draw();
   cP1->SetFillColor(0);
   cP1->Update();
   theApp->Run();
   theApp->Terminate();  
}
