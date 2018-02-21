// ---------------------------------------------------------
// -----      R3BCalifaDigitizer source file           -----

#include "R3BCalifaDigitizer.h"
#include "R3BCalifa.h"
#include "TClonesArray.h"
#include "FairRootManager.h"
#include "TRandom.h"
#include "TArrayF.h"
#include "TVector3.h"
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
	TArrayF cryId[nHits];
	TArrayF energyId[nHits];
  TArrayF Sumaenergy[nHits];
  
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
    
    
    //cryId[i]=crystalId;
    cryId->SetAt(crystalId,i);
    //energyId[i]=energy;
    energyId->SetAt(energy,i);
				
								/*if (nHits==1){
				
									//Energy  
									//momentum module:P^2 = E^2 - M^2 = (T + M)^2 - M^2 ===> P=E (M=0)
									//energy= sqrt(pow(pxOut,2)+ (pow(pyOut,2)+ (pow(pzOut,2))
				
									AddCrystalCal(crystalId, energy, Nf, Ns, time, tot_energy); 
				
								}
								else{
				
									while(crystalId==pointData[i+1]->GetCrystalId()){
					
										energy=energy+pointData[i+1]->GetEnergyLoss();
									}
									AddCrystalCal(crystalId, energy, Nf, Ns, time, tot_energy);
									energy=0.;*/
    
  
 }  
  	// void SetAt(Double_t v, Int_t i) { AddAt((Float_t)v, i); }
  
  for(Int_t i=0;i<nHits;i++){
  	
  	//Sumaenergy[i]=energyId[i];
  	Sumaenergy->SetAt(energyId->GetAt(i),i);
  	
  	if(nHits>1){
			//Sumaenergy[i+1]=energyId[i+1];
			
			for(Int_t j=i+1;j<nHits;j++){
				
				if(cryId->GetAt(i)==cryId->GetAt(j)){
					//Sumaenergy[i]=Sumaenergy[i]+energyId[j];
					Sumaenergy->SetAt(Sumaenergy->GetAt(i)+energyId->GetAt(j),i);
				}//else if(cryId[j]==cryId[j+1]){
					//Sumaenergy[j]=Sumaenergy[j]+energyId[j+1];		//MIRAR si es posa¿?
				//} 	
			}
  	
  	}
  }
  
  for(Int_t i=0;i<nHits;i++){
  	if(Sumaenergy->GetAt(i)>0){
  			AddCrystalCal(cryId->GetAt(i), Sumaenergy->GetAt(i), pointData[i]->GetNf(), pointData[i]->GetNs(), pointData[i]->GetTime(), tot_energy);
  															//NUSmearing(Sumaenergy[i])
  	}
  } 
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
   // if (fVerboseLevel > 1)
   // {
        LOG(INFO) << "-I- R3BCalifaDigitizer: Adding CrystalCalData "
                  << " with unique identifier " << ident << " entering with "
                  << energy * 1e06 << " keV Nf=" << Nf << " Ns=" << Ns <<" Time="<<time
                   << "tot_energy" <<tot_energy << FairLogger::endl;
   // }
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

