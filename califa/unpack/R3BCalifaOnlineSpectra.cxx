// ------------------------------------------------------------
// -----                  R3BCalifaOnlineSpectra          -----
// -----          Created April 13th 2016 by M.Heil       -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with detector variables which allow  
 * to test the detectors online
 *  
 */


#include "R3BCalifaOnlineSpectra.h"
#include "R3BCalifaMappedData.h"
#include "R3BCalifaCrystalCalData.h"
#include "R3BEventHeader.h"
//#include "R3BTCalEngine.h"

#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"

#include "TClonesArray.h"
#include <iostream>
#include <sstream>
#include "TMath.h"
#include <stdbool.h>
//#include "TStyle.h"

using namespace std;


R3BCalifaOnlineSpectra::R3BCalifaOnlineSpectra()
  : FairTask("OnlineSpectra", 1)
	, fMappedItemsCalifa(NULL)
	, fCalItemsCalifa(NULL)
  , fTrigger(-1)
  , fNEvents(0)
  , fCalifaNumPetals(1)
  , fCalifaOneCrystal(0)
	, fCalON(0)
{
}

R3BCalifaOnlineSpectra::R3BCalifaOnlineSpectra(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMappedItemsCalifa(NULL)
	, fCalItemsCalifa(NULL)
  , fTrigger(-1)
  , fNEvents(0)
  , fCalifaNumPetals(1)
  , fCalifaOneCrystal(0)
	, fCalON(0)
{
}

R3BCalifaOnlineSpectra::~R3BCalifaOnlineSpectra()
{
}

InitStatus R3BCalifaOnlineSpectra::Init()
{
  
  LOG(INFO) << "R3BCalifaOnlineSpectra::Init " << FairLogger::endl;
  
  
  
  // try to get a handle on the EventHeader. EventHeader may not be 
  // present though and hence may be null. Take care when using.
  
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
    FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "FairRootManager not found");
  header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");
  FairRunOnline *run = FairRunOnline::Instance();
  

    //get access to Mapped data
    fMappedItemsCalifa = (TClonesArray*)mgr->GetObject("CalifaMappedData");
    
    //get access to Cal data
    fCalItemsCalifa = (TClonesArray*)mgr->GetObject("CalifaCrystalCalData");
    
    
    if(fMappedItemsCalifa || fCalItemsCalifa){  
      
      //reading the file    
      ifstream *FileHistos = new ifstream(fCalifaFile);
      
      Double_t arry_bins[fCalifaNumPetals*64+1];
      Double_t arry_maxE[fCalifaNumPetals*64+1];
      Double_t arry_minE[fCalifaNumPetals*64+1];
      Double_t bins;
      Double_t maxE;
      Double_t minE;
      
      for (Int_t i=0; i<fCalifaNumPetals*64+1; i++){
				*FileHistos>>arry_bins[i]>>arry_minE[i]>>arry_maxE[i];	
				std::cout<<"i="<<i<<" "<<arry_bins[i]<<" "<<arry_minE[i]<<" "<<arry_maxE[i]<<endl;//READ ok!	
      }
      
      bins= arry_bins[fCalifaNumPetals*64];//the last line is a general setting
      maxE= arry_maxE[fCalifaNumPetals*64];
      minE= arry_minE[fCalifaNumPetals*64];
      
      //  CANVAS 1   ----------------------------------------------------  ok
      fh_Califa_cryId_petal = new TH2F("fh_Califa_cryId_petal", "Califa petal number vs cryId", 64, 0, 63, 8, 0, 7);    
      TCanvas *cCalifa1 = new TCanvas("Califa petal vs cryId", "Califa petal vs cryId", 10, 10, 500, 500);
      cCalifa1->Divide(2);
      cCalifa1->cd(1);
      fh_Califa_cryId_petal->Draw("COLZ");
      fh_Califa_cryId_petal->GetXaxis()->SetTitle("CrystalId");
      fh_Califa_cryId_petal->GetYaxis()->SetTitle("Petal number");
      cCalifa1->cd(0);
      run->AddObject(cCalifa1);
      
      //  CANVAS 2    ---------------------------------------------------  ok
      TString Name1;
      TString Name2; 
      TString Name3; 
      TString Yaxis1; 
      if(fCalON==true){
				Name1="fh_Califa_Cal_cryId_energy";
				Name2="Califa_Cal energy vs cryId";
				Name3="Califa_Cal energy vs cryId";
				Yaxis1="Energy (keV)";
      }
      else{	
				Name1="fh_Califa_Map_cryId_energy";
				Name2="Califa_Map energy vs cryId";
				Name3="Califa_Map energy vs cryId";
				Yaxis1="Energy (channels)";
      }
      
      fh_Califa_cryId_energy = new TH2F(Name1, Name2, 64*fCalifaNumPetals, 0., 64*fCalifaNumPetals-1, bins, minE, maxE);
			
      TCanvas *cCalifa2 = new TCanvas(Name3, Name3, 10, 10, 500, 500);
      fh_Califa_cryId_energy->Draw("COLZ");
      fh_Califa_cryId_energy->GetXaxis()->SetTitle("CrystalId");
      fh_Califa_cryId_energy->GetYaxis()->SetTitle(Yaxis1);
      run->AddObject(cCalifa2);
 
      
      //  CANVAS 3  ------------------------------------------------------------- ok
      TCanvas* cCalifa3[fCalifaNumPetals][4];		
      for(Int_t i=0; i<fCalifaNumPetals; i++){
	
				for (Int_t k=0;k<4;k++){

					char Name4[255];
					sprintf(Name4, "Califa Petal %d.%d", i+1,k+1);
					cCalifa3[i][k] = new TCanvas(Name4, Name4, 10, 10, 500, 500);
					cCalifa3[i][k]->Divide(4,4);
			
					for(Int_t j=0; j<16;j++){
					
						char Name5[255];
						char Name6[255];
						TString Xaxis1;
						if (fCalON==true){	    				
							sprintf(Name5, "h_Califa_Cal_Petal_%d_%d_Crystal_%d_energy", i+1, k+1, j+1);		  
							sprintf(Name6, "Califa_Cal Petal %d.%d Crystal %d energy", i+1,k+1, j+1); //cryId 1-16
							Xaxis1="Energy (keV)";
						}
						else{
							sprintf(Name5, "h_Califa_Map_Petal_%d_%d_Crystal_%d_energy", i+1, k+1, j+1);		  
							sprintf(Name6, "Califa_Map Petal %d.%d Crystal %d energy", i+1,k+1, j+1); //cryId 1-16
							Xaxis1="Energy (channels)";
						}
						fh_Califa_crystals[i][j+16*k] = new TH1F(Name5, Name6, arry_bins[j+16*k], arry_minE[j+16*k], arry_maxE[j+16*k]);
						cCalifa3[i][k]->cd(j+1);
						fh_Califa_crystals[i][j+16*k]->Draw();
						fh_Califa_crystals[i][j+16*k]->GetXaxis()->SetTitle(Xaxis1);
					}
					run->AddObject(cCalifa3[i][k]);
				}
      }
      
      //  CANVAS 4  -------------------------------     
      TCanvas* cCalifa4[fCalifaNumPetals];
      for (Int_t i=0;i<fCalifaNumPetals;i++){
	
				char Name7[255];
				char Name8[255];
				char Name9[255];
				TString Xaxis2;
	
				if (fCalON==true){	    				
					sprintf(Name7, "h_Califa_Cal_energy_per_petal_%d", i+1);	  
					sprintf(Name8, "Califa_Cal Energy per petal %d", i+1);
					sprintf(Name9, "Califa_Cal Energy petal %d", i+1);
					Xaxis2="Energy (keV)";
				}
				else{
					sprintf(Name7, "h_Califa_Map_energy_per_petal_%d", i+1);	  
					sprintf(Name8, "Califa_Map Energy per petal %d", i+1);
					sprintf(Name9, "Califa_Map Energy petal %d", i+1);
					Xaxis2="Energy (channels)";
				}
	
				fh_Califa_energy_per_petal[i] = new TH1F(Name7, Name8, bins, minE, maxE);
	
				cCalifa4[i] = new TCanvas(Name9, Name9, 10, 10, 500, 500);
				fh_Califa_energy_per_petal[i]->Draw();
				fh_Califa_energy_per_petal[i]->GetXaxis()->SetTitle(Xaxis2);
				run->AddObject(cCalifa4[i]);
	
      }
      
      //   CANVAS 5    --------------------------------------------------  ok!
      if(fCalifaOneCrystal>0){
	
				TString Name10;
				TString Name11;
				TString Name12;
				TString Xaxis3;
				if (fCalON==true){	    				
					Name10="fh_Califa_Cal_energy_oneCry";		  
					Name11="Califa_Cal energy for OneCrystal";
					Name12="Califa_Cal OneCrystal Energy";
					Xaxis3="Energy (keV)";
				}
				else{
					Name10="fh_Califa_Map_energy_oneCry";		  
					Name11="Califa_Map energy for OneCrystal";
					Name12="Califa_Map OneCrystal Energy";
					Xaxis3="Energy (channels)";
				}
	
				fh_Califa_energy_oneCry = new TH1F(Name10, Name11, arry_bins[fCalifaOneCrystal-1], arry_minE[fCalifaOneCrystal-1], arry_maxE[fCalifaOneCrystal-1]);

				TCanvas *cCalifa5 = new TCanvas(Name12, Name12, 10, 10, 500, 500);
				fh_Califa_energy_oneCry->Draw();
				fh_Califa_energy_oneCry->GetXaxis()->SetTitle(Xaxis3);
				run->AddObject(cCalifa5);
	
      }
      
    }//--------------------		
    
    return kSUCCESS;
}



void R3BCalifaOnlineSpectra::Exec(Option_t* option)
{
        
   if(fMappedItemsCalifa && fMappedItemsCalifa->GetEntriesFast()){
	   Int_t nHits = fMappedItemsCalifa->GetEntriesFast();
	   
	   for (Int_t ihit = 0; ihit < nHits; ihit++){
	       R3BCalifaMappedData* hit = (R3BCalifaMappedData*)fMappedItemsCalifa->At(ihit);
	       if (!hit) continue;
	       
	        
	       if (hit->GetCrystalId()==fCalifaOneCrystal){fh_Califa_energy_oneCry->Fill(hit->GetEnergy());}
	       
	       Int_t cryId;				
	       Int_t petal;
	       Int_t cryId_petal;
     
	       cryId=hit->GetCrystalId()-1;
	       petal=(Int_t)(cryId)/64;//from 0 to 7
	       cryId_petal=cryId-64*(petal);//from 0 to 63
	       
 				 fh_Califa_cryId_energy->Fill(hit->GetCrystalId(),hit->GetEnergy());
	       fh_Califa_crystals [petal][cryId_petal]->Fill(hit->GetEnergy());//individual energy histo for each crystalId
	       fh_Califa_energy_per_petal[petal]->Fill(hit->GetEnergy());//energy(channels) sum for each petal 
	       fh_Califa_cryId_petal->Fill(cryId_petal, petal);//crystalId vs petal number 
	     }
	 }

	 if(fCalItemsCalifa && fCalItemsCalifa->GetEntriesFast()){
	   Int_t nHits = fCalItemsCalifa->GetEntriesFast();
	   
	   for (Int_t ihit = 0; ihit < nHits; ihit++){
	       R3BCalifaCrystalCalData* hit = (R3BCalifaCrystalCalData*)fCalItemsCalifa->At(ihit);
	       if (!hit) continue;
	       
	       if (hit->GetCrystalId()==fCalifaOneCrystal){fh_Califa_energy_oneCry->Fill(hit->GetEnergy());}
	       
	       Int_t cryId;				
	       Int_t petal;
	       Int_t cryId_petal;
	       
	       cryId=hit->GetCrystalId()-1;
	       petal=(Int_t)(cryId)/64;//from 0 to 7
	       cryId_petal=cryId-64*(petal);//from 0 to 63
	       
 				 fh_Califa_cryId_energy->Fill(hit->GetCrystalId(),hit->GetEnergy());
	       fh_Califa_crystals [petal][cryId_petal]->Fill(hit->GetEnergy());//individual energy histo for each crystalId
	       fh_Califa_energy_per_petal[petal]->Fill(hit->GetEnergy());//energy(channels) sum for each petal 
	       fh_Califa_cryId_petal->Fill(cryId_petal, petal);//crystalId vs petal number 
	     }
	 }
       
       fNEvents += 1;
       
}

void R3BCalifaOnlineSpectra::FinishEvent()
{
		// CALIFA
		//fh_Califa_cryId_petal->Draw("COLZ");
    //fh_Califa_cryId_energy->Draw("COLZ");
	  //fh_Califa_energy_oneCry->Draw();  
		
    if (fCalItemsCalifa)
      {
	fCalItemsCalifa->Clear();
      }
    if (fMappedItemsCalifa)
      {
	fMappedItemsCalifa->Clear();
      }
    
}

void R3BCalifaOnlineSpectra::FinishTask()
{

	if (fMappedItemsCalifa || fCalItemsCalifa){


			// CALIFA
      //fh_Califa_cryId_petal->Draw("COLZ");
      //fh_Califa_cryId_energy->Draw("COLZ");
			//fh_Califa_energy_oneCry->Draw();

			//------------
			TFile *MyFile = new TFile("hist.root","RECREATE"); 
			if ( MyFile->IsOpen() ) cout << "File opened successfully"<<endl;
			fh_Califa_cryId_petal->Write();
      fh_Califa_cryId_energy->Write();

			for(Int_t i=0; i<fCalifaNumPetals; i++){
				for (Int_t k=0;k<4;k++){
					for(Int_t j=0; j<16;j++){
						fh_Califa_crystals[i][j+16*k]->Write();
					}
				}
			}

			for (Int_t i=0;i<fCalifaNumPetals;i++){
				fh_Califa_energy_per_petal[i]->Write();
			}
			fh_Califa_energy_oneCry->Write();
			MyFile->Print();
			MyFile->Close();
	 }
  
}

ClassImp(R3BCalifaOnlineSpectra)
