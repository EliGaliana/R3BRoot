///////////////////////////////////////////////////////////////////////
//// ** elisabet.galiana@usc.es
//// ** Universidad de Santiago de Compostela
//// ** Dpto. Física de Partículas
////
////	--- Check Resutls of CALIFA detector ---
////
////	Macro to check:
////	   -Mapped Data: CrystalId, Channels
////	   -Calibrated Data: CrystalId, Energy
////
////	General Usage:
////	1st: select the root file and the settings:
////	   -maximum energy & channels, range CryId of the petals 
////	   -Histograms: change the ranges and bins (OPTIONAL)
////
////	2nd: root -l checkResults.C
////
///////////////////////////////////////////////////////////////////////


void checkResults_Online() {
  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
  /////////////////////////////////////////////////////////////
  //// SETTINGS: SELECT HERE THE SETTING CONTROLING THE MACRO
  /////////////////////////////////////////////////////////////
  //INPUT FILE
  char inputFile[250] = "../../../../../Krakow2017/data_0001.root";  //change root file
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
  Int_t binEnergy;
  maxE=2000;//kev
  binEnergy=40000;
  
  //Range CrystalId 
  //Petal 1
  Int_t P1_1, P1_2;
  P1_1=64;
  P1_2=129;
  
  //Petal 2
  Int_t P2_1, P2_2;
  P2_1=383;
  P2_2=449;
  
  //Petal 3
  Int_t P3_1, P3_2;
  P3_1=448;
  P3_2=513;
  
  ////////////////////////////////////////////////////////////////
  //// END OF SETTINGS: HISTOGRAMS RANGE COULD BE MODIFIED BELOW
  ////////////////////////////////////////////////////////////////
    
  
  //HISTOGRAMS DEFINITION-----------------------------------------------------------
  TH1F *h1_Map, *h2_Map;
  TH1F *h1_Cal, *h2_Cal, *h3_Cal_P1, *h3_Cal_P2, *h3_Cal_P3;
	TH1F *h_cry1, *h_cry2, *h_cry3, *h_cry4, *h_cry5, *h_cry6, *h_cry7, *h_cry8, *h_cry9, *h_cry10, *h_cry11, *h_cry12, *h_cry13, *h_cry14, *h_cry15, *h_cry16;
  TH2F *h1, *h2_P1, *h2_P2, *h2_P3, *h3;
  
  //HISTOGRAMS CREATION ------------------------------------------------------------  Change these ranges and bins (OPTIONAL)
  h1_Map = new TH1F("h1_Map","Crystal ID",513,1,513);
  h2_Map = new TH1F("h2_Map","Channels energy",binChannels,0,maxChannels); 
  
  h1_Cal = new TH1F("h1_Cal","Crystal ID",513,1,513);
  h2_Cal = new TH1F("h2_Cal","Energy",binEnergy,0,maxE); 
  
  h3_Cal_P1 = new TH1F("h3_Cal_P1","Energy of Petal 1",binEnergy,0,maxE); 
  h3_Cal_P2 = new TH1F("h3_Cal_P2","Energy of Petal 2",binEnergy,0,maxE); 
  h3_Cal_P3 = new TH1F("h3_Cal_P3","Energy of Petal 3",binEnergy,0,maxE); 
  
  h1 = new TH2F("h1","Energy vs CrystalId",513,1,513,binEnergy,200,maxE);
  h2_P1 = new TH2F("h2_P1","Petal1: Energy vs CrylId",64,65,129,binEnergy,200,maxE);
  h2_P2 = new TH2F("h2_P2","Petal2: Energy vs CrylId",64,384,448,binEnergy,200,maxE);
  h2_P3 = new TH2F("h2_P3","Petal3: Energy vs CrylId",64,449,513,binEnergy,200,maxE);
	
	h3 = new TH2F("h3","Petal vs CrystalId",513,1,513,8,0,8);
	h_cry1 = new TH1F("h_cry1","Energy CryId=1",binEnergy,0,maxE);
	h_cry2 = new TH1F("h_cry2","Energy CryId=2",binEnergy,0,maxE);
	h_cry3 = new TH1F("h_cry3","Energy CryId=3",binEnergy,0,maxE);
	h_cry4 = new TH1F("h_cry4","Energy CryId=4",binEnergy,0,maxE);
	h_cry5 = new TH1F("h_cry5","Energy CryId=5",binEnergy,0,maxE);
	h_cry6 = new TH1F("h_cry6","Energy CryId=6",binEnergy,0,maxE);
	h_cry7 = new TH1F("h_cry7","Energy CryId=7",binEnergy,0,maxE);
	h_cry8 = new TH1F("h_cry8","Energy CryId=8",binEnergy,0,maxE);
	h_cry9 = new TH1F("h_cry9","Energy CryId=9",binEnergy,0,maxE);
	h_cry10 = new TH1F("h_cry10","Energy CryId=10",binEnergy,0,maxE);
	h_cry11 = new TH1F("h_cry11","Energy CryId=11",binEnergy,0,maxE);
	h_cry12 = new TH1F("h_cry12","Energy CryId=12",binEnergy,0,maxE);
	h_cry13 = new TH1F("h_cry13","Energy CryId=13",binEnergy,0,maxE);
	h_cry14 = new TH1F("h_cry14","Energy CryId=14",binEnergy,0,maxE);
	h_cry15 = new TH1F("h_cry15","Energy CryId=15",binEnergy,0,maxE);
	h_cry16 = new TH1F("h_cry16","Energy CryId=16",binEnergy,0,maxE);
	
  
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
      // h2_Cal->Fill(calData[h]->GetEnergy());//energy (keV)
      // h1->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
			
       //Petal1
       if (calData[h]->GetCrystalId()>P1_1 && calData[h]->GetCrystalId()<P1_2)
	 {
	   //h3_Cal_P1->Fill(calData[h]->GetEnergy());
	   //h2_P1->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
			h3->Fill(calData[h]->GetCrystalId()-64,0);
	 }
       
       //Petal2
       if (calData[h]->GetCrystalId()>P2_1 && calData[h]->GetCrystalId()<P2_2)
	 {
	   //h3_Cal_P2->Fill(calData[h]->GetEnergy());
	   //h2_P2->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
			h3->Fill(calData[h]->GetCrystalId()-384,1);
	 }
       
       //Petal3
       if (calData[h]->GetCrystalId()>P3_1 && calData[h]->GetCrystalId()<P3_2)
	 {
	   //h3_Cal_P3->Fill(calData[h]->GetEnergy());
	   //h2_P3->Fill(calData[h]->GetCrystalId(),calData[h]->GetEnergy());
			h3->Fill(calData[h]->GetCrystalId()-448,2);
	 }
     

			/*if(calData[h]->GetCrystalId()==450){
				h_cry1->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==451){
				h_cry2->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==452){
				h_cry3->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==453){
				h_cry4->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==454){
				h_cry5->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==455){
				h_cry6->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==456){
				h_cry7->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==457){
				h_cry8->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==458){
				h_cry9->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==459){
				h_cry10->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==460){
				h_cry11->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==461){
				h_cry12->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==462){
				h_cry13->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==463){
				h_cry14->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==464){
				h_cry15->Fill(calData[h]->GetEnergy());
			}
			if(calData[h]->GetCrystalId()==465){
				h_cry16->Fill(calData[h]->GetEnergy());
			}*/
			
  
     }
     
     if(mapHitsPerEvent)      delete[] mappedData;
     if(caloHitsPerEvent)     delete[] calData;
  }
  // END LOOP IN THE EVENTS----------------------------------------------------------------------------------------------------
  
  
  // HISTOGRAMS---------------------------------------------------------------------
  
  //Mapped Data  
  /*TCanvas* c0 = new TCanvas("CrystalId","CrystalId",0,0,400,800);
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
  h2_P3->GetYaxis()->SetTitle("Energy (keV)");*/
  
  



	/*TCanvas* c5 = new TCanvas("c5","Energy Crystals",0,0,400,800);
  c5->SetFillColor(0);
  c5->SetFrameFillColor(0);
  c5->Divide(4,4);

  c5->cd(1);
	h_cry1->Draw();	
	h_cry1->SetLineColor(kCyan+3);
	h_cry1->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(2);
	h_cry2->Draw();	
	h_cry2->SetLineColor(kCyan+3);
	h_cry2->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(3);
	h_cry3->Draw();	
	h_cry3->SetLineColor(kCyan+3);
	h_cry3->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(4);
	h_cry4->Draw();	
	h_cry4->SetLineColor(kCyan+3);
	h_cry4->GetXaxis()->SetTitle("Energy (keV)");

  c5->cd(5);
	h_cry5->Draw();
	h_cry5->SetLineColor(kCyan+3);	
	h_cry5->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(6);
	h_cry6->Draw();	
	h_cry6->SetLineColor(kCyan+3);
	h_cry6->GetXaxis()->SetTitle("Energy (keV)");

  c5->cd(7);
	h_cry7->Draw();	
	h_cry7->SetLineColor(kCyan+3);
	h_cry7->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(8);
	h_cry8->Draw();	
	h_cry8->SetLineColor(kCyan+3);
	h_cry8->GetXaxis()->SetTitle("Energy (keV)");

  c5->cd(9);
	h_cry9->Draw();	
	h_cry9->SetLineColor(kCyan+3);
	h_cry9->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(10);
	h_cry10->Draw();
	h_cry10->SetLineColor(kCyan+3);
	h_cry10->GetXaxis()->SetTitle("Energy (keV)");

  c5->cd(11);
	h_cry11->Draw();	
	h_cry11->SetLineColor(kCyan+3);
	h_cry11->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(12);
	h_cry12->Draw();	
	h_cry12->SetLineColor(kCyan+3);
	h_cry12->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(13);
	h_cry13->Draw();	
	h_cry13->SetLineColor(kCyan+3);
	h_cry13->GetXaxis()->SetTitle("Energy (keV)");

  c5->cd(14);
	h_cry14->Draw();	
	h_cry14->SetLineColor(kCyan+3);
	h_cry14->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(15);
	h_cry15->Draw();	
	h_cry15->SetLineColor(kCyan+3);
	h_cry15->GetXaxis()->SetTitle("Energy (keV)");

	c5->cd(16);
	h_cry16->Draw();	
	h_cry16->SetLineColor(kCyan+3);
	h_cry16->GetXaxis()->SetTitle("Energy (keV)");

	TCanvas* c6 = new TCanvas("c6","Energy Crystals",0,0,400,800);
  c6->SetFillColor(0);
  c6->SetFrameFillColor(0);
	h_cry1->Draw();	
	h_cry1->SetLineColor(kCyan+3);
	h_cry1->GetXaxis()->SetTitle("Energy (keV)");*/

	TCanvas* c7 = new TCanvas("c7","Petals vs Crystals",0,0,400,800);
  c7->SetFillColor(0);
  c7->SetFrameFillColor(0);
	h3->Draw("colz");	
	h3->GetXaxis()->SetTitle("CryId");
	h3->GetYaxis()->SetTitle("Petal");


}
