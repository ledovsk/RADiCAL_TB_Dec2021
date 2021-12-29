// C++ includes
#include <fstream>
#include <string>
#include <iostream>

// ROOT includes
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

//LOCAL INCLUDES
#include "Aux.hh"
//#include "Config.hh"

using namespace std;


//TStyle* style;



int main(int argc, char **argv) {
//  gROOT->SetBatch();
  
  FILE* fp1;
  char stitle[200];
  int dummy;

  //**************************************
  // Parse command line arguments
  //**************************************

  int numRequiredArgs = 2;
  if (argc - 1 < numRequiredArgs) {
    std::cerr <<  "[ERROR]: Usage --> ./maketree --inputFileName=<your_caen_drs4_file.dat> --outputFileName=<your_output_file.root>" << std::endl;
    return -1;
  }
  
  std::cout << "\n=== Beginning program ===\n" << std::endl;

  //****************************************
  // Getting Input FileName (CAEN DRS4 file)
  //****************************************
  std::string inputFileName = ParseCommandLine( argc, argv, "--inputFileName=" );
  if ( inputFileName == "" )
    {
      std::cerr << "[ERROR]: please provide a valid inputFileName. Use: --inputFileName=<your_input_file_name> " << std::endl;
      exit(0);
    }
  
  //*******************************
  // Getting Output FileName (ROOT)
  //*******************************
  std::string outputFileName = ParseCommandLine( argc, argv, "--outputFileName=" );
  if ( outputFileName == "" )
  {
      std::cerr << "[ERROR]: please provide a valid outputFileName. Use: --outputFileName=<your_output_file_name> " << std::endl;
      exit(0);
  }

  std::cout << "[INFO]: input file --> " << inputFileName << std::endl;
  std::cout << "[INFO]: output file --> " << outputFileName << std::endl;
  

  //**************************************
  // Load Voltage Calibration
  //**************************************

  std::cout << "\n=== Loading voltage calibration ===\n" << std::endl;
  double off_mean[2][9][1024];
  for( int i = 0; i < 2; i++ ){
      sprintf( stitle, "dt5742_offset_%d.txt", i );
      fp1 = fopen( stitle, "r" );
      printf("Loading offset data from %s\n", stitle);

      for( int k = 0; k < 1024; k++ ) {     
          for( int j = 0; j < 9; j++ ){      
              dummy = fscanf( fp1, "%lf ", &off_mean[i][j][k] ); 
          }
      }
      fclose(fp1);
  }

  //**************************************
  // Load Sample Calibration
  //**************************************

  std::cout << "\n=== Loading voltage calibration ===\n" << std::endl;
  double calib_sample[2][9][1024];
  for( int i = 0; i < 2; i++ ){
      sprintf( stitle, "dt5742_sample_%d.txt", i );
      fp1 = fopen( stitle, "r" );
      printf("Loading sample data from %s\n", stitle);

      for( int k = 0; k < 1024; k++ ) {     
          for( int j = 0; j < 9; j++ ){      
              dummy = fscanf( fp1, "%lf ", &calib_sample[i][j][k] ); 
          }
      }
      fclose(fp1);
  }

  //**************************************
  // Load Time Calibration
  //**************************************

  std::cout << "\n=== Loading time calibration ===\n" << std::endl;
  double fdummy;
  double tcal_dV[2][1024];
  for( int i = 0; i < 2; i++ ) {
      sprintf( stitle, "dt5742_axis_%d.txt", i );
      fp1 = fopen( stitle, "r" );
      printf("Loading dV data from %s\n", stitle);

      for( int k = 0; k < 1024; k++)      
          dummy = fscanf( fp1, "%lf", &tcal_dV[i][k] ); 
      fclose(fp1);
  }
  double dV_sum[2] = {0, 0};
  for( int i = 0; i < 2; i++ ) {
      for( int j = 0; j < 1024; j++ )
          dV_sum[i] += tcal_dV[i][j];
  }

  double fullTimeScale = 200.0;
  double tcal[2][1024];
  for( int i = 0; i < 2; i++) {
      tcal[i][0] = 0.;
      for( int j = 1; j < 1024; j++) {
          //          tcal[i][j] = tcal_dV[i][j] / dV_sum[i] * fullTimeScale;
          tcal[i][j] = tcal_dV[i][j] - tcal_dV[i][j-1];
      }
  }
  
  //*********************************************************
  // Check if has valid input file, otherwise exit with error
  //*********************************************************
  ifstream ifile(inputFileName);
  if (!ifile)
    {
      std::cerr << "[ERROR]: !USAGE! Input file does not exist. Please enter valid file name" << std::endl;
      exit(0);
    }
  

  
  //*********************************************************
  // Check if has valid input file, otherwise exit with error
  //*********************************************************
  bool saveRaw = false;
  
  //**************************************
  // Define output
  //**************************************

  TFile* file = new TFile( outputFileName.c_str(), "RECREATE", "CAEN V1742");
  TTree* tree = new TTree("pulse", "Digitized waveforms");

  int event;
  short tc[2];
  short raw[18][1024]; 
  float channel[18][1024]; 
  float times[2][1024];
  
  tree->Branch("event",  &event,    "event/I");
  if(saveRaw){
     tree->Branch("tc",      tc,       "tc[2]/s");
     tree->Branch("raw",     raw,      "raw[18][1024]/S");   
  }
  tree->Branch("channel", channel,  "channel[18][1024]/F");
  tree->Branch("times",   times,    "times[2][1024]/F");


  // temp variables for data input
  uint   event_header;
  uint   temp[3];
  ushort samples[9][1024];

  //*************************
  // Open Input File
  //*************************

  FILE* fpin = fopen( inputFileName.c_str(), "r" );

  //*************************
  //Event Loop
  //*************************

  std::cout << "\n=== Processing input data ===\n" << std::endl;
  int nGoodEvents = 0;
  int maxEvents = 999999;

  for( int iEvent = 0; iEvent < maxEvents; iEvent++){ 
      
      if ( iEvent % 100 == 0 ) {
          std::cout << "Event " << iEvent << "\n";
      }
      event = nGoodEvents; // for output tree
      
      // first header word
      dummy = fread( &event_header, sizeof(uint), 1, fpin);

      // second header word
      dummy = fread( &event_header, sizeof(uint), 1, fpin);  
      uint grM     = event_header & 0b11; // 2-bit channel group mask
      // std::cout << " Group mask = " << grM << std::endl;

      // third and fourth header words
      dummy = fread( &event_header, sizeof(uint), 1, fpin);  
      dummy = fread( &event_header, sizeof(uint), 1, fpin);  
      
      // check for end of file
      if (feof(fpin)) break;
      
      //*************************
      // Parse group mask into channels
      //*************************
      
      bool _isGR_On[2];
      _isGR_On[0] = (grM & 0x01);
      _isGR_On[1] = (grM & 0x02);
      
      int activeGroupsN = 0;
      int realGroup[2] = {-1, -1 };
      for ( int l = 0; l < 2; l++ ) {
          if ( _isGR_On[l] ) 
          {
              realGroup[activeGroupsN] = l; 
              activeGroupsN++;
          }
      }
      
      //************************************
      // Loop over channel groups
      //************************************
      
      for ( int group = 0; group < activeGroupsN; group++ ) {
          // Read group header
          dummy = fread( &event_header, sizeof(uint), 1, fpin);  
          // ushort tcn = (event_header >> 20) & 0xfff; // trigger counter bin
          ushort tcn = (event_header << 2) >> 22; // trigger counter bin
          tc[realGroup[group]] = tcn;
          uint TR = (event_header >> 12) & 0b1;
          // std::cout << " Group =  " << group << "   TR0 = " << TR << std::endl;
          
          // Check if all channels were active (if 8 channels active return 3072)
          int nsample = (event_header & 0xfff) / 3;
          // std::cout << " Group =  " << group << "   samples = " << nsample << std::endl;
          
          // Define time coordinate
          times[realGroup[group]][0] = 0.0;
          for( int i = 1; i < 1024; i++ ){
              times[realGroup[group]][i] = float(tcal[realGroup[group]][(i-1+tcn)%1024] + times[realGroup[group]][i-1]);
          }      
          
          //************************************
          // Read sample info for group
          //************************************      
          
          for ( int i = 0; i < nsample; i++ ) {
              dummy = fread( &temp, sizeof(uint), 3, fpin );  
              samples[0][i] =  temp[0] & 0xfff;
              samples[1][i] = (temp[0] >> 12) & 0xfff;
              samples[2][i] = (temp[0] >> 24) | ((temp[1] & 0xf) << 8);
              samples[3][i] = (temp[1] >>  4) & 0xfff;
              samples[4][i] = (temp[1] >> 16) & 0xfff;
              samples[5][i] = (temp[1] >> 28) | ((temp[2] & 0xff) << 4);
              samples[6][i] = (temp[2] >>  8) & 0xfff;
              samples[7][i] =  temp[2] >> 20;	
          }
          
          // Trigger channel
          if(TR){
              for(int j = 0; j < nsample/8; j++){
                  fread( &temp, sizeof(uint), 3, fpin);  
                  samples[8][j*8+0] =  temp[0] & 0xfff;
                  samples[8][j*8+1] = (temp[0] >> 12) & 0xfff;
                  samples[8][j*8+2] = (temp[0] >> 24) | ((temp[1] & 0xf) << 8);
                  samples[8][j*8+3] = (temp[1] >>  4) & 0xfff;
                  samples[8][j*8+4] = (temp[1] >> 16) & 0xfff;
                  samples[8][j*8+5] = (temp[1] >> 28) | ((temp[2] & 0xff) << 4);
                  samples[8][j*8+6] = (temp[2] >>  8) & 0xfff;
                  samples[8][j*8+7] =  temp[2] >> 20;
              }
          }else{
              for(int j = 0; j < nsample/8; j++){
                  samples[8][j*8+0] =  0;
                  samples[8][j*8+1] =  0;
                  samples[8][j*8+2] =  0;
                  samples[8][j*8+3] =  0;
                  samples[8][j*8+4] =  0;
                  samples[8][j*8+5] =  0;
                  samples[8][j*8+6] =  0;
                  samples[8][j*8+7] =  0;
              }
          }
          
          //************************************
          // Loop over channels 0-8
          //************************************      
          
          for(int i = 0; i < 9; i++) {
              
              int totalIndex = realGroup[group]*9 + i;
              
              for ( int j = 0; j < 1024; j++ ) {
                  raw[totalIndex][j] = 0;
              }
              
              // Fill pulses
              for ( int j = 0; j < 1024; j++ ) {
                  raw[totalIndex][j] = (short)(samples[i][j]);
                  channel[totalIndex][j] = (double)(samples[i][j]) - (double)(off_mean[realGroup[group]][i][(j+tcn)%1024]);
                  channel[totalIndex][j] -= (double)(calib_sample[realGroup[group]][i][j]);
                  channel[totalIndex][j] = 1000.*((channel[totalIndex][j] - 0)/4095. - 0.5) + 0;                  
              }
          }
          dummy = fread( &event_header, sizeof(uint), 1, fpin);
      }
      tree->Fill();
      nGoodEvents++;
  }
  
  fclose(fpin);
  cout << "\nProcessed total of " << nGoodEvents << " events\n";
  
  file->Write();
  file->Close();
  
  return 0;
}



