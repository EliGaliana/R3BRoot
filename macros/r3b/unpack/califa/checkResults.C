///////////////////////////////////////////////////////////////////////
//// ** elisabet.galiana@usc.es
//// ** Universidad de Santiago de Compostela
//// ** Dpto. FÃ­sica de PartÃ­culas
////
////	--- Check Resutls of CALIFA detector ---
////
////	Macro to check:
////	   -Mapped Data: CrystalId, Channels
////	   -Calibrated Data: CrystalId, Energy
////
////	General Usage:
////	1st: select the root file and the settings:
////	   -max,min energy & channels, range CryId of the petals 
////	   -Histograms: change the ranges and bins (OPTIONAL)
////
////	2nd: root -l checkResults.C
////
///////////////////////////////////////////////////////////////////////


void checkResults() {
  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
  /////////////////////////////////////////////////////////////
  //// SETTINGS: SELECT HERE THE SETTING CONTROLING THE MACRO
  /////////////////////////////////////////////////////////////
  //INPUT FILE
  char inputFile[250] = "dataCal_003_60Co.root";  //change root file dataCal_0008_0066_22Na.root
  TFile *file1 = TFile::Open(inputFile);
  //READING TREE
  TTree* tree = (TTree*)file1->Get("evt");
  
  //VARIABLES DEFINITION  
  //Maximum Channels
  Int_t maxChannels;
  Int_t binChannels;
  maxChannels=1000;
  binChannels=maxChannels+1;
  
  //Maximum energy
  Double_t maxE;
  Double_t minE;
  Int_t binEnergy;
  minE=1100;
  maxE=1400;//kev
  binEnergy=150;
  
  //Range CrystalId 
  //Petal 1
  Int_t P1_1, P1_2;
  P1_1=0;
  P1_2=64;
  
  //Petal 2
  Int_t P2_1, P2_2;
  P2_1=64;
  P2_2=128;
  
  //Petal 3
  Int_t P3_1, P3_2;
  P3_1=256;
  P3_2=320;

  
  //Petal 4
  Int_t P4_1, P4_2;
  P4_1=320;
  P4_2=384;  
  ////////////////////////////////////////////////////////////////
  //// END OF SETTINGS: HISTOGRAMS RANGE COULD BE MODIFIED BELOW
  ////////////////////////////////////////////////////////////////
    
  
  //HISTOGRAMS DEFINITION-----------------------------------------------------------
  TH1F *h1_Map, *h2_Map;
  TH1F *h1_Cal, *h2_Cal, *h3_Cal_P1, *h3_Cal_P2, *h3_Cal_P3;
  TH2F *h1, *h2_P1, *h2_P2, *h2_P3;
  
  //HISTOGRAMS CREATION ------------------------------------------------------------  Change these ranges and bins (OPTIONAL)
  h1_Map = new TH1F("h1_Map","Crystal ID",512,0.5,512.5);
  h2_Map = new TH1F("h2_Map","Channels energy",binChannels,0,maxChannels); 
  
  h1_Cal = new TH1F("h1_Cal","Crystal ID",512,0.5,512.5);
  h2_Cal = new TH1F("h2_Cal","Energy",binEnergy,minE,maxE); 
  
  h3_Cal_P1 = new TH1F("h3_Cal_P1","Energy of Petal 1",binEnergy,minE,maxE); 
  h3_Cal_P2 = new TH1F("h3_Cal_P2","Energy of Petal 2",binEnergy,minE,maxE); 
  h3_Cal_P3 = new TH1F("h3_Cal_P3","Energy of Petal 3",binEnergy,minE,maxE);
  h3_Cal_P4 = new TH1F("h3_Cal_P4","Energy of Petal 4",binEnergy,minE,maxE); 
  
  h1 = new TH2F("h1","Energy vs CrystalId",512,0.5,512.5,binEnergy,minE,maxE);
  
  h2_P1 = new TH2F("h2_P1","Petal1: Energy vs CrylId",64,64.5,127.5,binEnergy,minE,maxE);
  h2_P2 = new TH2F("h2_P2","Petal2: Energy vs CrylId",64,128.5,195.5,binEnergy,minE,maxE);
  h2_P3 = new TH2F("h2_P3","Petal3: Energy vs CrylId",64,256.5,319.5,binEnergy,minE,maxE);
  h2_P4 = new TH2F("h2_P4","Petal4: Energy vs CrylId",64,320.5,383.5,binEnergy,minE,maxE);

  h2_P5 = new TH2F("h2_P5","Petal5: Energy vs CrylId",64,64.5,127.5,binEnergy,minE,maxE);
  h2_P6 = new TH2F("h2_P6","Petal6: Energy vs CrylId",64,128.5,195.5,binEnergy,minE,maxE);
  h2_P7 = new TH2F("h2_P7","Petal7: Energy vs CrylId",64,256.5,319.5,binEnergy,minE,maxE);
  h2_P8 = new TH2F("h2_P8","Petal8: Energy vs CrylId",64,320.5,383.5,binEnergy,minE,maxE);

  
  //----- CALIFA detector--------------//
  
  //Mapped Data
  TClonesArray* mappedDataCA;
  R3BCalifaMappedData** mappedData;
  mappedDataCA = new TClonesArray("R3BCalifaMappedData",5);
  TBranch *branchCalifaMappedData = tree->GetBranch("CalifaMappedData");
  branchCalifaMappedData->SetAddress(&mappedDataCA);
  
  //Calibrated Data
  TClonesArray* calDataCA;
  R3BCalifaCrystalCalData** calData;
  calDataCA = new TClonesArray("R3BCalifaCrystalCalData",5);
  TBranch *branchCalifaCalData = tree->GetBranch("CalifaCrystalCalData");
  branchCalifaCalData->SetAddress(&calDataCA);
  
  Int_t mapHitsPerEvent = 0;
  Int_t caloHitsPerEvent= 0;
  
  //TREE ENTRIES--------------------------------------------------------------------
  Long64_t nevents = tree->GetEntries();
  
  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    tree->GetEvent(i);
    
    mapHitsPerEvent = mappedDataCA->GetEntries();
    caloHitsPerEvent= calDataCA->GetEntries();
    
     if(mapHitsPerEvent>0) {
       mappedData = new R3BCalifaMappedData*[mapHitsPerEvent];
       for(Int_t j=0;j<mapHitsPerEvent;j++){
	 mappedData[j] = new R3BCalifaMappedData;
	 mappedData[j] = (R3BCalifaMappedData*) mappedDataCA->At(j);
       }
    }
     
     if(caloHitsPerEvent>0) {
       calData = new R3BCalifaCrystalCalData*[caloHitsPerEvent];
       for(Int_t j=0;j<caloHitsPerEvent;j++){
	 calData[j] = new R3BCalifaCrystalCalData;
	 calData[j] = (R3BCalifaCrystalCalData*) calDataCA->At(j);
      }
     }
     
     //ANALYSIS of the Results ------------------------
    
     //LOOP in Map Hits ----
     for(Int_t h=0;h<mapHitsPerEvent;h++){
       h1_Map->Fill(mappedData[h]->GetCrystalId());
       h2_Map->Fill(mappedData[h]->GetEnergy());//channels
     }
     
     //LOOP in Cal Hits ----
     for(Int_t h=0;h<caloHitsPerEvent;h++){
       h2_Cal->Fill(calData[h]->GetEnergy());//energy (keV)
       h1->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
			
       //Petal1
       if (calData[h]->GetCrystalId()>P1_1 && calData[h]->GetCrystalId()<P1_2)
	 {
	   h3_Cal_P1->Fill(calData[h]->GetEnergy());
	   h2_P1->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
	 }
       
       //Petal2
       if (calData[h]->GetCrystalId()>P2_1 && calData[h]->GetCrystalId()<P2_2)
	 {
	   h3_Cal_P2->Fill(calData[h]->GetEnergy());
	   h2_P2->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
	 }
       
       //Petal3
       if (calData[h]->GetCrystalId()>P3_1 && calData[h]->GetCrystalId()<P3_2)
	 {
	   h3_Cal_P3->Fill(calData[h]->GetEnergy());
	   h2_P3->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
	 }
       //Petal4
       if (calData[h]->GetCrystalId()>P4_1 && calData[h]->GetCrystalId()<P4_2)
	 {
	   h3_Cal_P4->Fill(calData[h]->GetEnergy());
	   h2_P4->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
	 }
       
     }
     
     if(mapHitsPerEvent)      delete[] mappedData;
     if(caloHitsPerEvent)     delete[] calData;
  }
  // END LOOP IN THE EVENTS----------------------------------------------------------------------------------------------------
  
  
  // HISTOGRAMS---------------------------------------------------------------------
  
  //Mapped Data  
  TCanvas* c0 = new TCanvas("CrystalId","CrystalId",0,0,400,800);
  c0->SetFillColor(0);
  c0->SetFrameFillColor(0);
  h1_Map->Draw("");
  h1_Map->SetLineColor(9);
  h1_Map->GetXaxis()->SetTitle("CrystalId");
  h1_Map->GetYaxis()->SetTitle("Counts");
  
  TCanvas* c1 = new TCanvas("MapData: Channels","MapData: Channels",0,0,400,800);
  c1->SetFillColor(0);
  c1->SetFrameFillColor(0);
  h2_Map->Draw("");
  h2_Map->SetLineColor(9);
  h2_Map->GetXaxis()->SetTitle("Channels");
  h2_Map->GetYaxis()->SetTitle("Counts");
  
  //Cal Data  
  TCanvas* c2 = new TCanvas("CalData: Energy","CalData: Energy",0,0,400,800);
  c2->SetFillColor(0);
  c2->SetFrameFillColor(0);
  h2_Cal->Draw("");
  h2_Cal->SetLineColor(9);
  h2_Cal->GetXaxis()->SetTitle("Energy (keV)");
  h2_Cal->GetYaxis()->SetTitle("Counts");
  
  TCanvas* c3 = new TCanvas("Energy vs CrystalId","Energy vs CrystalId",0,0,720,900);
  c3->SetFillColor(0);
  c3->SetFrameFillColor(0);
  h1->Draw("colz");
  h1->GetXaxis()->SetTitle("CrystalId");
  h1->GetYaxis()->SetTitle("Energy (keV)");
  
  TCanvas* c4 = new TCanvas("Petals Energy","Petals Energy",0,0,400,800);
  c4->SetFillColor(0);
  c4->SetFrameFillColor(0);
  c4->Divide(2,2);
  c4->cd(1);
  h3_Cal_P1->Draw();
  h3_Cal_P1->SetLineColor(2);
  h3_Cal_P1->GetXaxis()->SetTitle("Energy (keV)");
  h3_Cal_P1->GetYaxis()->SetTitle("Counts");
  c4->cd(2);
  h3_Cal_P2->Draw();
  h3_Cal_P2->SetLineColor(8);
  h3_Cal_P2->GetXaxis()->SetTitle("Energy (keV)");
  h3_Cal_P2->GetYaxis()->SetTitle("Counts");
  c4->cd(3);
  h3_Cal_P3->Draw();
  h3_Cal_P3->SetLineColor(13);
  h3_Cal_P3->GetXaxis()->SetTitle("Energy (keV)");
  h3_Cal_P3->GetYaxis()->SetTitle("Counts");
  c4->cd(4);
  h3_Cal_P4->Draw();
  h3_Cal_P4->SetLineColor(13);
  h3_Cal_P4->GetXaxis()->SetTitle("Energy (keV)");
  h3_Cal_P4->GetYaxis()->SetTitle("Counts");

  
  TCanvas* c5 = new TCanvas("Petals Energy vs CryId","Petals Energy vs CryId",0,0,400,800);
  c5->SetFillColor(0);
  c5->SetFrameFillColor(0);
  c5->Divide(2,2);
  c5->cd(1);
  h2_P1->Draw("colz");
  h2_P1->GetXaxis()->SetTitle("CryId");
  h2_P1->GetYaxis()->SetTitle("Energy (keV)");
  c5->cd(2);
  h2_P2->Draw("colz");
  h2_P2->GetXaxis()->SetTitle("CryId");
  h2_P2->GetYaxis()->SetTitle("Energy (keV)");
  c5->cd(3);
  h2_P3->Draw("colz");
  h2_P3->GetXaxis()->SetTitle("CryId");
  h2_P3->GetYaxis()->SetTitle("Energy (keV)");
  c5->cd(4);
  h2_P4->Draw("colz");
  h2_P4->GetXaxis()->SetTitle("CryId");
  h2_P4->GetYaxis()->SetTitle("Energy (keV)");
  
}

