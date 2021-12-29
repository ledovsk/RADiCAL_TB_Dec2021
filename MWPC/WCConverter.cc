#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TPaveStats.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TStyle.h"
#include <algorithm>
#include <math.h>
#include <complex>
// #include <TDCacheFile.h>

using namespace std;

typedef struct Hit
{
	int spill;
	int event;
	int eventinspill;
	int module;
	int channel;
	int TDC;
}Hit;

struct WCData{
	int xy[2];//0-x; 1-y
	int t[2];
    std::vector<int> xyv[2];
    std::vector<int> tv[2];
	int wc;
};

int main(int argc, char *argv[])
{
	char hname[300];
	TFile* outroot=new TFile("output.root","recreate");
	TH2F* WCXY[4];TH1F* TDC_WC[4];TH1F* XProf[4];TH1F* YProf[4]; TH1F* WCX[4]; TH1F* WCY[4];
	TH2F* TDC_vs_X[4];TH2F* TDC_vs_Y[4];
    TH2F *h101[4];
    TH1F *h102[4];
    TH1F *h103[4];
    TH1F *h104[4];
    TH1F *h105[4];
    
    TH1F * h00 = new TH1F("h00", "", 100, -0.5, 99.5);
    TH1F * h01 = new TH1F("h01", "", 100, -0.5, 99.5);
    TH1F * h02 = new TH1F("h02", "", 18, -0.5, 17.5);
    TH1F * h110 = new TH1F("h110", "", 100, -0.5, 99.5);
    TH1F * h111 = new TH1F("h111", "", 100, -0.5, 99.5);
    TH1F * h03[16];
    for(int i1=0; i1<16; i1++){
        sprintf(hname, "module_%d", i1+1);
        h03[i1] = new TH1F(hname, "", 131, -1.5, 129.5);
    }

    for(int i1=0;i1<4;i1++)
	{
		sprintf(hname,"WC%d",i1+1);
		WCXY[i1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"TDC_WC%d",i1+1);
		TDC_WC[i1]=new TH1F(hname,hname,400,-0.5,399.5);
		sprintf(hname,"WCX%d",i1+1);
		WCX[i1]=new TH1F(hname,hname,128,-0.5,127.5);
		sprintf(hname,"WCY%d",i1+1);
		WCY[i1]=new TH1F(hname,hname,128,-0.5,127.5);
		sprintf(hname,"TDC_vs_X_%d",i1+1);
        TDC_vs_X[i1] = new TH2F(hname,hname,128, -0.5, 127.5, 400, -0.5, 399.5);
		sprintf(hname,"TDC_vs_Y_%d",i1+1);
        TDC_vs_Y[i1] = new TH2F(hname,hname,128, -0.5, 127.5, 400, -0.5, 399.5);
		sprintf(hname,"h101_%d",i1+1);
        h101[i1] = new TH2F(hname,hname,10, -0.5, 9.5, 10, -0.5, 9.5);
		sprintf(hname,"h102_%d",i1+1);
        h102[i1] = new TH1F(hname,hname,128, -0.5, 127.5);
		sprintf(hname,"h103_%d",i1+1);
        h103[i1] = new TH1F(hname,hname,400, -0.5, 399.5);
		sprintf(hname,"h104_%d",i1+1);
        h104[i1] = new TH1F(hname,hname,128, -0.5, 127.5);
		sprintf(hname,"h105_%d",i1+1);
        h105[i1] = new TH1F(hname,hname,400, -0.5, 399.5);
	}
	
// 	sprintf(hname,"t1041_%s.dat",RunNo.c_str());
//	sprintf(hname,"WCData_raw/t1041_%s.dat",RunNo.c_str());
	ifstream infile("in.dat");
	
//	sprintf(hname,"WCData/WCData_%s.txt",RunNo.c_str());
	ofstream outfile("out.dat");
	
	string fileline;
	int lineind=0;
	Hit current;
	current.spill=-1;current.event=-1;current.eventinspill=-1;current.module=-1;current.channel=-1;current.TDC=-1;
	vector <Hit> eventHit;
	while ( getline (infile, fileline ).good () )
	{
		istringstream iss(fileline);
		vector <string> entries;
		copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter<vector<string> >(entries));
		if(entries[0]=="SPILL")
		{
			current.spill=atoi(entries[1].c_str());
		}
		if(entries[0]=="EVENT")
		{
            h00->Fill( eventHit.size() );
            h110->Fill( current.spill );
			if(eventHit.size()>0)
			{
				WCData wcd[4];
				for(int i1=0;i1<4;i1++){
                    wcd[i1].xy[0]=-1;
                    wcd[i1].xy[1]=-1;
                    wcd[i1].t[0]=1000;
                    wcd[i1].t[1]=1000;
                    wcd[i1].wc=i1;
                    wcd[i1].xyv[0].clear();
                    wcd[i1].xyv[1].clear();
                    wcd[i1].tv[0].clear();
                    wcd[i1].tv[1].clear();                    
                }
				for(int i1=0;i1<eventHit.size();i1++)
				{
                    h01->Fill( eventHit[i1].spill );
                    h02->Fill( eventHit[i1].module );
                    if(eventHit[i1].module > 0 && eventHit[i1].module < 17){
                        h03[eventHit[i1].module-1]->Fill( eventHit[i1].channel );
                    }
                    
					int xyind=(((eventHit[i1].module-1)%4<=1)?0:1);
					int wcID=(eventHit[i1].module-1)/4;
                    int chID=(((eventHit[i1].module%2==1)?0:1)*64+eventHit[i1].channel);
					TDC_WC[wcID]->Fill(eventHit[i1].TDC);
                    wcd[wcID].tv[xyind].push_back(eventHit[i1].TDC);
                    wcd[wcID].xyv[xyind].push_back(chID);
					if(eventHit[i1].TDC < wcd[wcID].t[xyind])
					{
						wcd[wcID].t[xyind]=eventHit[i1].TDC;
						wcd[wcID].xy[xyind]=(((eventHit[i1].module%2==1)?0:1)*64+eventHit[i1].channel);
					}
				}
				outfile<<eventHit[0].spill<<" "<<eventHit[0].event<<" "<<eventHit[0].eventinspill;
				for(int i1=0;i1<4;i1++)
				{
//					outfile<<" "<<wcd[i1].xy[0]<<" "<<wcd[i1].xy[1]<<" "<<wcd[i1].tv[0].size()<<" "<<wcd[i1].tv[1].size();
					if(wcd[i1].xy[0]>=0) WCX[i1]->Fill(wcd[i1].xy[0]);
					if(wcd[i1].xy[1]>=0) WCY[i1]->Fill(wcd[i1].xy[1]);
					if(wcd[i1].xy[0]>=0 && wcd[i1].xy[1]>=0) WCXY[i1]->Fill(wcd[i1].xy[0],wcd[i1].xy[1]);
				}
				outfile<<endl;
                
                // Fill distributions for previous event
  
                h111->Fill( eventHit[0].spill );
                
                int ch_[4][2][128];
                int tdc_[4][2][128];
                for(int i1=0; i1<4; i1++){
                    for(int i2=0; i2<2; i2++){
                        for(int i3=0; i3<128; i3++){
                            ch_[i1][i2][i3] = 0;
                            tdc_[i1][i2][i3] = 1000;
                        }
                    }
                }
                for(int i1=0; i1<4; i1++)
                {
                    for(unsigned int j=0; j<wcd[i1].tv[0].size(); j++){
                        TDC_vs_X[i1]->Fill( wcd[i1].xyv[0][j], wcd[i1].tv[0][j] );
                        ch_[i1][0][wcd[i1].xyv[0][j]] = 1;
                        if(wcd[i1].tv[0][j] <= tdc_[i1][0][wcd[i1].xyv[0][j]]){
                            tdc_[i1][0][wcd[i1].xyv[0][j]] = wcd[i1].tv[0][j];
                        }
                    }
                    for(unsigned int j=0; j<wcd[i1].tv[1].size(); j++){
                        TDC_vs_Y[i1]->Fill( wcd[i1].xyv[1][j], wcd[i1].tv[1][j] );
                        ch_[i1][1][wcd[i1].xyv[1][j]] = 1;
                        if(wcd[i1].tv[1][j] <= tdc_[i1][1][wcd[i1].xyv[1][j]]){
                            tdc_[i1][1][wcd[i1].xyv[1][j]] = wcd[i1].tv[1][j];
                        }
                    }
                }
                int nh_[4][2];
                for(int i1=0; i1<4; i1++){
                    for(int i2=0; i2<2; i2++){
                        nh_[i1][i2] = 0;
                        for(int i3=0; i3<128; i3++){
                            if(ch_[i1][i2][i3]!=0){
                                nh_[i1][i2]++;
                            }
                        }
                    }
                }

                for(int i1=0; i1<4; i1++){
                    h101[i1]->Fill( nh_[i1][0], nh_[i1][1] );
                    for(int i3=0; i3<128; i3++){
                        if(ch_[i1][0][i3] > 0){
                            h102[i1]->Fill( i3 );
                            h103[i1]->Fill( tdc_[i1][0][i3] );
                        }
                        if(ch_[i1][1][i3] > 0){
                            h104[i1]->Fill( i3 );
                            h105[i1]->Fill( tdc_[i1][1][i3] );
                        }
                    }
                }
			}
			eventHit.clear();
			current.event=atoi(entries[1].c_str());
			current.eventinspill=atoi(entries[2].c_str());
		}
		if(entries[0]=="Module")
		{
			current.module=atoi(entries[1].c_str());
		}
		if(entries[0]=="Channel")
		{
			current.channel=atoi(entries[1].c_str());
			current.TDC=atoi(entries[2].c_str());
			Hit h;
			h.spill=current.spill;
			h.event=current.event;
			h.eventinspill=current.eventinspill;
			h.module=current.module;
			h.channel=current.channel;
			h.TDC=current.TDC;
			eventHit.push_back(h);
//           cout << h.spill << " " << h.event << " " << h.eventinspill << " " << h.module << " " << h.channel << " " << h.TDC <<endl;
		}
//		cout<<current.spill<<" "<<current.event<<" "<<current.eventinspill<<" "<<current.module<<" "<<current.channel<<" "<<current.TDC<<endl;
	}
	outroot->Write();
	outroot->Close();
	outfile.close();
	infile.close();
}








