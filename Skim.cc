#include <stdio.h>
#include <iostream>
#include <fstream>
#include "KDisplay/kdisplay_event.h"
#include "KrdEdxPId/KrdEdxPId.hh"
#include "VDDCRec/kdcpar.h"
#include "VDDCRec/kdcvd.h"
#include "VDDCRec/ktracks.h"
#include "VDDCRec/khits.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TMinuit.h"
#include "VDDCRec/kdcswitches.h"
#include <KDB/kdb.h>
#include "KEmcRec/emc_system.h"
#include "KrToF/tof_system.h"
#include "VDDCRec/kedr_trigger.h"
#include "ReadNat/read_nat_c.h"  
#include "ReadNat/re_def.h"
#include "ReadNat/ss_def.h"
#include "KrMu/mu_system.h"
#include "KrMu/mu_event.h"
#include "KrMu/mu_status.h"
#include  "ReadNat/trg_argmask_c.h"
#include  "ReadNat/trg_df.h"
#include "TVector.h"
#include "TMatrix.h"
#include <argp.h>
#include "KrToF/sc_hits.h"
#include "VDDCRec/kvd.h"
#include "VDDCRec/ktof.h"

#define Pi 3.14159265
#define mpi 139.57
#define mK 497.672
#define mp 938.272 

double sqr(double x) {
  return x*x;
}
using namespace std;

static char doc[] ="mh skim";
/* A description of the arguments we accept*/
static char args_doc[] = "";
static struct argp_option options[] = {
  {"from", 'f', "RUN1",  0, "Run Number",1 },
  {"to", 't', "RUN2",  0, "Run Number",2 },
  { 0,0,0,0,0,0 }
};

struct arguments
{
 int from;
  int to;
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
  case 'f': arg_union.arguments->from = atoi(arg); break;
  case 't': arg_union.arguments->to = atoi(arg); break;
 }
    return 0;
};
static struct argp argp = { options, parse_opt, args_doc, doc };
  
extern "C" void kedr_pack_();
int main(int argc, char **argv)
{
 struct arguments arguments;
  arguments.from=0;
  arguments.to=0;
  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  cout << " run1 "<<arguments.from<< " run2 "<<arguments.to<< endl;  
 
  /*  kdcswitches_.KemcAllowed = 1;
  kdcswitches_.kCosmInSigRuns = 0;
  kdcswitches_.KcExp=1;
  kdcswitches_.kEmcPatch=1;
  kdcswitches_.kIPalternative=1;
  kdcswitches_.kUseDCIP=1; */
   
   char fname[256];
  int openerr,runerr,readerr,closeerr, recselect=0, nevt=0;
  int Nevents=50000;
  int    streamSMH=0;
  char fnameSMH[128]; 
 
  int numevskim=0;
  	
  snprintf(fnameSMH,128,"/spool/skim/Hadrons2017v2/run");
 
  streamSMH=write_nat(fnameSMH,mRunFiles);  
  //  nat_filter(streamSMH,fLuminosity);
  nat_filter(streamSMH,fAll);
  Double_t ERun=0,ERunErr=0;
  double CosArray[100][100];
  double CosArrayZ[100][100];
  int TrArray[100];
    KDBconn *conn = kdb_open();
  if (!conn)
  {
      printf("cannot connect to database\n");;      
      return 0;      
  }  

  if(mu_default_init(1))  cout<<"Mu init error"<<endl;
  if(mu_init_status())  cout<<"Mu init status error"<<endl;
  
  ///  kDClayerOff(7,0);
  // kDClayerOff(6,0);
  // kDClayerOff(1,3);

  int runC=24847;
  //dcdedxpidinit(&runC);
  //bool msk[6];
  //for(int i=0;i<6;i++) msk[i]=true;
  // pidinitpar(0.00001,msk);

  int run1=arguments.from;
  int run2=arguments.to;
  for(int run=run1; run<=run2; run++)
      {
  // int run=arguments.run;
      cout<<"run "<<run<<endl;
      cout<<"skim file: "<< fnameSMH<<endl;

  int  TableE=kdb_get_id(conn,"runenergy"); 
  int  LengthE =kdb_get_length(conn,TableE);
 
  int bufferE[16];
 int sizeR=kdb_read_for_run(conn,TableE,run,bufferE,LengthE);
 ERun=bufferE[1]*0.000001;
  ERunErr=bufferE[2]*0.000001;                        
  
  sprintf(fname,"/space/runs/daq%06d.nat",run);
  
  printf("file '%s'\n",fname);
  
	int trgerr=0;
	int PTOK=0,STOK=0,KlukvaErr=0,ynb=0;
	kedr_read_trg_("trigger.dat",&trgerr,11);
	float *abg=new float[1];
	abg[0]=40.;
	kemc_trigger_abg(abg);
  kedr_open_nat(fname,&openerr);
  
  if(mu_get_db_status(run))  cout<<"Mu status error"<<endl;
  if(mu_get_db_clbr_for_run(run)<0)  cout<<"Mu calib error"<<endl;


  if ( openerr)
    {
      printf("open error for %s\n",fname);
      continue;
    }
  
  while(1)
	{ 
	  kedr_read_nat(&readerr); 
	  if( readerr) break; 

	  kdcvdrec(0,&recselect); 

	  ynb=224;
	  kedr_trigger(ynb,&PTOK,&STOK,&KlukvaErr);
	  bool PTMSK=lvl2_c(l2sc2)+lvl1_c(l1csi1)*lvl1_c(l1csi2);

	  // if((eDaqNumber==16||eDaqNumber==1399||eDaqNumber==12133||eDaqNumber==48710||eDaqNumber==53975)&&PTOK==1&&STOK==1&&KlukvaErr==0&&PTMSK>0){ 

	    //pidevent(); 

      	  double Eemc=0,energy[100],phiemc[100],thetaemc[100],Elkr=0.;
	  int Nemc=semc.emc_ncls;
	  for(Int_t rcls=0;rcls<Nemc;rcls++)
	    {
	      Eemc+=semc.emc_energy[rcls];
	      if(semc.emc_type[rcls]==1) Elkr+=semc.emc_energy[rcls];
	      energy[rcls]=semc.emc_energy[rcls];
	      phiemc[rcls]=semc.emc_phi[rcls]*Pi/180.;
	      thetaemc[rcls]=semc.emc_theta[rcls]*Pi/180.;
	    }   
	
	  unsigned short MuNhitsRaw = mu_next_event_good();
	  int Ntrip=0;
	  int IsSelected=0;
	  
	  int Ntr=eTracksAll;
	  double P[100],Theta[100],Phi[100],E[100],RxyTr[100],RzTr[100];
	  for(int t=0;t<Ntr;t++)
	    {
	      P[t]=tP(t);
	      Theta[t]=ktrrec_.TETRAK[t]*Pi/180.;
	      Phi[t]= ktrrec_.FITRAK[t]*Pi/180.;
	      RxyTr[t]=tDr0(t);
	      RzTr[t]=tDz0(t);
	      E[t]=0.;
	      for(int k=0;k<semc.dc_emc_ncls[t];k++){
		int ind=semc.dc_emc_cls[t][k]-1;
		E[t]+=semc.emc_energy[ind];
	      }	 
	      if(TMath::Abs(RxyTr[t])<0.5&&TMath::Abs(RzTr[t])<13.&&tPt(t)>100.)Ntrip++;
	      //  if(dcinfo.dedx[t]>15.) IsSelected=1;
	    }

	  int ind1,ind2;
	  double en1=0,en2=0;
	  for(int i=0;i<Nemc;i++){
	    if(energy[i]>en1) {
	      en1=energy[i];
	      ind1=i;
	    }
	  }
	  for(int k=0;k<Nemc;k++){
	    if(energy[k]>en2&&k!=ind1) {
	      en2=energy[k];
	      ind2=k;
	    }
	  }	  
	  int ind3=0;
	  double p1=0;
	  for(int j=0;j<Ntr;j++){
	    if(P[j]>p1&&Theta[j]>0.5&&Theta[j]<2.64&&(RxyTr[j]<5.&&RxyTr[j]>-5.)&&(RzTr[j]<15.&&RzTr[j]>-15.)){
	      p1=P[j];
	      ind3=j;
	    }
	  }
	  
	int eesel=0;
	if(MuNhitsRaw==0&&Ntr>0&&en1>500&&en2>500&&cos(phiemc[ind1]-phiemc[ind2])<-0.9&&thetaemc[ind1]>0.56&&thetaemc[ind2]>0.56&&thetaemc[ind1]<2.58&&thetaemc[ind2]<2.58&&P[ind3]>1000&&Theta[ind3]>0.56&&Theta[ind3]<2.58&&(RxyTr[ind3]<3.0&&RxyTr[ind3]>-3.0)&&(RzTr[ind3]<15.&&RzTr[ind3]>-15.))eesel=1;
	 if(eesel==0&&MuNhitsRaw==0&&Ntr>0){
	//if(eesel==1){    
	  cout<<"*********\n run "<<run<<" eDaqNumber "<<eDaqNumber<<endl; 
	  for(int q=0;q<Ntr;q++){
	    // if(dcinfo.dedx[q]>15.)
	    //cout<<"  dedx "<<dcinfo.dedx[q]<<" Ptr "<<tPt(q)<<" P "<<tP(q)<<endl;
	    }
	     kdisplay_event();
	     // kedr_pack_();  
	     //int ok=put_event(streamSMH);   
	     // numevskim++;
	        }
	 //	}
	}
      kedr_close_nat(&closeerr);
       }


  kdb_close(conn);  
  cout<<"ok end \n"<<endl;
}


 
