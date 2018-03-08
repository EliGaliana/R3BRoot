// ---------------------------------------------------------
// -----      R3BCalifaDigitizer source file           -----

#include "R3BCalifaDigitizer.h"
#include "R3BCalifa.h"
#include "TClonesArray.h"
#include "FairRootManager.h"
#include "TRandom.h"
#include "TArrayD.h"
#include "TVector3.h"
#include "TMath.h"
#include <iostream>
#include <stdlib.h>
#include "R3BCalifaPoint.h"
#include "R3BCalifaCrystalCalData.h"

using std::cout;
using std::cerr;
using std::endl;

#define U_MEV 931.4940954

inline double BETA(const double M, const double E_kin) { return sqrt(1. - M * M / ((M + E_kin) * (M + E_kin))); }

inline double GAMMA(const double M, const double E_kin) { return 1. + E_kin / M; }

//R3BCalifaDigitizer: Default Constructor
R3BCalifaDigitizer::R3BCalifaDigitizer() : FairTask("R3B CALIFA Digitizer")					  
{
}

//R3BCalifaDigitizer: Constructor
R3BCalifaDigitizer::R3BCalifaDigitizer(const TString& geoFile) : FairTask("R3B CALIFA Digitizer"),
							   fCalifaPointDataCA(NULL),
							   fCalifaCryCalDataCA(NULL),
	 							 fNonUniformity(0),
    						 fGeometryVersion(1)						  
{
}

//Virtual R3BCalifaDigitizer: Destructor
R3BCalifaDigitizer::~R3BCalifaDigitizer()
{

		if (fCalifaPointDataCA)
    {
        fCalifaPointDataCA->Delete();
        delete fCalifaPointDataCA;
    }
    if (fCalifaCryCalDataCA)
    {
        fCalifaCryCalDataCA->Delete();
        delete fCalifaCryCalDataCA;
    }


	//Nota: delete TArrayF cryId[nHits], energyId[nHits], Sumaenergy[nHits];
  LOG(INFO) << "R3BCalifaDigitizer: Delete instance" << FairLogger::endl;
}


// -----   Public method Init   --------------------------------------------
InitStatus R3BCalifaDigitizer::Init()
{ 
  //INPUT DATA
  //Point data
  FairRootManager* rootManager = FairRootManager::Instance();
  if (!rootManager) { return kFATAL;}
  
  fCalifaPointDataCA = (TClonesArray*)rootManager->GetObject("CrystalPoint");
  if (!fCalifaPointDataCA) { return kFATAL;}
   
  //OUTPUT DATA
  //CrystalCal data
  fCalifaCryCalDataCA = new TClonesArray("R3BCalifaCrystalCalData",10);
  rootManager->Register("CalifaCrystalCalData", "CALIFA Crystal Cal", fCalifaCryCalDataCA, kTRUE);
  
  return kSUCCESS;
}


// -----   Public method Execution   --------------------------------------------
void R3BCalifaDigitizer::Exec(Option_t* option)
{
    
  //Reading the Input -- Point data --
  Int_t nHits = fCalifaPointDataCA->GetEntries();
  if(!nHits) return;
  
  R3BCalifaPoint** pointData;
  pointData=new R3BCalifaPoint*[nHits];
  
                            
   /*Accessors 
  Int_t GetCrystalType()   const { return fCrystalType; }
  Int_t GetCrystalCopy()   const { return fCrystalCopy; }
  Int_t GetCrystalId()   const { return fCrystalId; }
  Double_t GetXIn()   const { return fX; }
  Double_t GetYIn()   const { return fY; }
  Double_t GetZIn()   const { return fZ; }
  Double_t GetXOut()  const { return fX_out; }
  Double_t GetYOut()  const { return fY_out; }
  Double_t GetZOut()  const { return fZ_out; }
  Double_t GetPxOut() const { return fPx_out; }
  Double_t GetPyOut() const { return fPy_out; }
  Double_t GetPzOut() const { return fPz_out; }
  Double_t GetNf()   const { return fNf; }
	Double_t GetNs()   const { return fNs; }
  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }*/
 
  Int_t crystalType;
  Int_t crystalCopy;
  Int_t crystalId;
  Double_t xIn;
  Double_t yIn;
  Double_t zIn;
  Double_t xOut;
  Double_t yOut;
  Double_t zOut;
  Double_t pxOut;
  Double_t pyOut;
  Double_t pzOut;
  Double_t Nf;
  Double_t Ns;
  Double_t time;
  Double_t energy;
  TVector3 posIn;
  TVector3 posOut;
  TVector3 mom;
  
  Double_t tot_energy=0.;
  
  /*
	R3BCalifaCrystalCalData* AddCrystalCal(Int_t ident, 
																					 Double_t energy,
																					 Double_t Nf,
																					 Double_t Ns,
																					 ULong64_t time,
																					 Double_t tot_energy);
	*/
	//TArrayD cryId[nHits];
	//TArrayD energyId[nHits];
  //TArrayD Sumaenergy[nHits];
  
 /* TArrayF* cryId[nHits]= new TArrayF();
	TArrayF* energyId[nHits]= new TArrayF();
  TArrayF* Sumaenergy[nHits]= new TArrayF();*/
  
  Double_t Ary_cryId[nHits];
  Double_t Ary_energy[nHits];
  Double_t Ary_Nf[nHits];
  Double_t Ary_Ns[nHits];
  Double_t Sumaenergy[nHits];
  Int_t sorted[nHits];
  
  
  for(Int_t i = 0; i < nHits; i++) {
    pointData[i] = (R3BCalifaPoint*)(fCalifaPointDataCA->At(i));
    
    crystalType = pointData[i]->GetCrystalType();
    crystalCopy = pointData[i]->GetCrystalCopy();
    crystalId   = pointData[i]->GetCrystalId();
    xIn				  = pointData[i]->GetXIn();
    yIn				  = pointData[i]->GetYIn();
    zIn				  = pointData[i]->GetZIn();
    xOut			  = pointData[i]->GetXOut();
    yOut			  = pointData[i]->GetYOut();
    zOut			  = pointData[i]->GetZOut();
    pxOut			  = pointData[i]->GetPxOut();
    pyOut			  = pointData[i]->GetPyOut();
    pzOut			  = pointData[i]->GetPzOut();
    Nf          = pointData[i]->GetNf();
    Ns          = pointData[i]->GetNs();
    pointData[i]->PositionIn(posIn);
    pointData[i]->PositionOut(posOut);
    pointData[i]->MomentumOut(mom);
    
    time=pointData[i]->GetTime();
    energy=pointData[i]->GetEnergyLoss();
    
    
    Ary_cryId[i]=crystalId;
    //cryId->SetAt(crystalId,i);
    Ary_energy[i]=energy;
    //energyId->SetAt(energy,i);
    //time?
    //Nf¿
    Ary_Nf[i]=Nf;
    //Ns¿
    Ary_Ns[i]=Ns;
		
	}
	
	/*--------------------  NO!
	
	TMath::Sort(nHits,Ary_cryId,sorted,kFALSE);//increasing order
	Double_t suma_energy;
	
	for(Int_t i=0; i<nHits; i++){
	 
	 if (i == 0)
			suma_energy=Ary_energy[0];
			
		else{
					
					if (Ary_cryId[sorted[i]==Ary_cryId[sorted[i-1]]]){
					
						suma_energy=suma_energy+Ary_energy[sorted[i]];					
					}
					else{
						AddCrystalCal(Ary_cryId[i], Ary_energy[0], Ary_Nf[0], Ary_Ns[0], time, tot_energy);
					}
	
		}
	}*/
		
	 /*-------------------------------------------------------------------------------
	 como R3BCalifa la original
	 
	Int_t nCrystalHits = fCalifaCryCalDataCA->GetEntriesFast();
	Bool_t existHit = 0; 
	 
	 if (nCrystalHits == 0)
           AddCrystalCal(Ary_cryId[0], Ary_energy[0], Ary_Nf[0], Ary_Ns[0], time, tot_energy);
   else
   {
            for (Int_t i=0; i<nHits; i++)
            {
                if (Ary_cryId[sorted[i]==Ary_cryId[sorted[i-1]]])
                {
                    ((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreEnergy(Ary_energy[sorted[i]]);
                    ((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreNf(Ary_Nf[sorted[i]]);
                    ((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreNs(Ary_Ns[sorted[i]]);
                    //existHit = 1; // to avoid the creation of a new CrystalHit
                    //break;
                }
            }
           // if (!existHit)
           //     AddCrystalCal(Ary_cryId[i], Ary_energy[i], Ary_Nf[i], Ary_Ns[i], time, tot_energy);
		}
		existHit = 0;*/
	
	
	
	
	//void TMath::Sort 	(Index n, const Element* a, Index* index, Bool_t down = kTRUE) 	If down is false sort in increasing order (default is decreasing order).
	//root [0] double a[4] = {0.34,-2.28,-0.1,4.2}
  //root [2] Int_t idx[4]
  //root [3] TMath::Sort(4,a,idx)
  //root [4] for(int i=0; i<4; i++) cout << idx[i] << " " << a[i] << endl;
  //> 3 0.34
  //> 0 -2.28
  //> 2 -0.1
  //> 1 4.2
  //or
  //for(int i=0; i<4; i++) cout << idx[i] << " " << a[idx[i]] << endl
  
 /*---------------------------------------------------------------------------------------- 
 Algoritmo de Héctor*/
 
  TMath::Sort(nHits,Ary_cryId,sorted,kFALSE);//increasing order
  //for(Int_t i=0; i<nHits; i++) {
 	//	 cout<< sorted[i] << " " <<Ary_cryId[i] << endl;
  //}
	
	for(Int_t i=0; i<nHits; i++){
	
			if (i==0) AddCrystalCal(Ary_cryId[i], Ary_energy[i], Ary_Nf[i], Ary_Ns[i], time, tot_energy);

			else{
					
					if (Ary_cryId[sorted[i]==Ary_cryId[sorted[i-1]]]){
					
					//((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->SetEnergy(Ary_energy[sorted[i]]);
					((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreEnergy(Ary_energy[sorted[i]]);
					//((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreEnergy(Ary_energy[sorted[i]]);
					//((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreNf(Ary_Nf[sorted[i]]);
					//((R3BCalifaCrystalCalData*)(fCalifaCryCalDataCA->At(i)))->AddMoreNs(Ary_Ns[sorted[i]]);
					}
					else{ AddCrystalCal(Ary_cryId[i], Ary_energy[i], Ary_Nf[i], Ary_Ns[i], time, tot_energy);}
			}
	
	}
	
	/*----------------------------------------------------------------------------------
	Eli: Va mal
	if(nHits==1){
			
			AddCrystalCal(crystalId, energy, Nf, Ns, time, tot_energy);
			cout<<"  ////////  Adding CryCal nHits==1"<<endl;
			cout<<"crystalId= "<<crystalId<<"energy= "<<energy<<endl;
			cout<<"Nf= "<<Nf<<"Ns= "<<Ns<<endl;	
			cout<<"time= "<<time<<"tot_energy= "<<tot_energy<<endl;
		
	}else if (nHits>1){
		

		for (Int_t i=0;i<nHits;i++){
		
			Double_t E;
			E=energyId[i];
			Sumaenergy[i]=E;
			cout<<endl<<endl<<"Aqui 1"<<endl;
			cout<<"cryId={"<<cryId[i]<<"}"<<endl;
			cout<<"E=energyId="<<energyId[i]<<endl;
			cout<<"Sumaenergy=E="<<Sumaenergy[i]<<endl;
			
			for(Int_t j=i+1;j<nHits;j++){
							
				Int_t id_i;
				Int_t id_j;				
				id_i=cryId[i];
				id_j=cryId[j];
							
				//cout<<"Aqui 2"<<endl;
				//cout<<"id_i="<<id_i<<" id_j"<<id_j<<endl;
							
				if (id_i==id_j){
							
					cout<<"-----------------------------------------------  Aqui 3: igual id"<<endl;
					cout<<"id_i="<<id_i<<" id_j"<<id_j<<endl;
					cout<<"E="<<E<<" energyId[j]="<<energyId[j]<<endl;
					E=E+energyId[j];
					Sumaenergy[i]=E;
					//cout<<"E="<<E<<" Sumaenergy[i]"<<Sumaenergy[i]<<endl;
				}
			}  	
	 }
		
  
  for(Int_t i=0;i<nHits;i++){
  	if(Sumaenergy[i]>0){
  			AddCrystalCal(cryId[i], Sumaenergy[i], pointData[i]->GetNf(), pointData[i]->GetNs(), pointData[i]->GetTime(), tot_energy);
  															//NUSmearing(Sumaenergy[i])
  			cout<<"  ////////  Adding CryCal nHits>1"<<endl;
  			cout<<"crystalId= "<<cryId[i]<<"  Sumaenergy= "<<Sumaenergy[i]<<endl;
				cout<<"Nf= "<<pointData[i]->GetNf()<<"  Ns= "<<pointData[i]->GetNs()<<endl;
				cout<<"time= "<<pointData[i]->GetTime()<<"  tot_energy= "<<tot_energy<<endl;
  	}
  } 
 }*/
  
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BCalifaDigitizer::EndOfEvent()
{
  
    fCalifaCryCalDataCA->Clear();

    ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BCalifaDigitizer::Register()
{
     FairRootManager::Instance()->Register("CrystalCal", GetName(),
                                          fCalifaCryCalDataCA, kTRUE);
}

// -----   Public method Reset   ----------------------------------------------
void R3BCalifaDigitizer::Reset()
{
    fCalifaCryCalDataCA->Clear();
    ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method FinishEvent   ----------------------------------------------
void R3BCalifaDigitizer::FinishEvent() {
}
// ----------------------------------------------------------------------------

// -----   Private method AddCrystalHit   --------------------------------------------
R3BCalifaCrystalCalData* R3BCalifaDigitizer::AddCrystalCal(Int_t ident,
                                                     Double_t energy,
                                                     Double_t Nf,
                                                     Double_t Ns,
                                                     ULong64_t time,
                                                     Double_t tot_energy)
{
    TClonesArray& clref = *fCalifaCryCalDataCA;
    Int_t size = clref.GetEntriesFast();
    LOG(INFO) << "-I- R3BCalifaDigitizer: Adding CrystalCalData "
              << " with unique identifier " << ident << " entering with "
              << energy * 1e06 << " keV Nf=" << Nf << " Ns=" << Ns <<" Time="<<time
              << " tot_energy=" <<tot_energy << FairLogger::endl;
   
    return new (clref[size]) R3BCalifaCrystalCalData(ident, energy, Nf, Ns, time, tot_energy);
    
}
// ----------------------------------------------------------------------------

// -----   Private method NUSmearing  --------------------------------------------
Double_t R3BCalifaDigitizer::NUSmearing(Double_t inputEnergy)
{
    // Very simple preliminary scheme where the NU is introduced as a flat random
    // distribution with limits fNonUniformity (%) of the energy value.
    //
    return gRandom->Uniform(inputEnergy - inputEnergy * fNonUniformity / 100,
                            inputEnergy + inputEnergy * fNonUniformity / 100);
}

// -----  Public method SetNonUniformity  ----------------------------------
void R3BCalifaDigitizer::SetNonUniformity(Double_t nonU)
{
    fNonUniformity = nonU;
    LOG(INFO) << "R3BCalifaDigitizer::SetNonUniformity to " << fNonUniformity << " %" << FairLogger::endl;
}

