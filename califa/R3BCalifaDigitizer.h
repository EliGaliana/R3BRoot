// ---------------------------------------------------------
// -----      R3BCalifaDigitizer header file           -----


#ifndef R3BCALIFADIGITIZER_H
#define R3BCALIFADIGITIZER_H

#include "R3BCalifa.h"
#include "FairTask.h"
#include "TClonesArray.h"
#include "R3BCalifaPoint.h"
#include "R3BCalifaCrystalCalData.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "string"

class R3BCalifaDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  R3BCalifaDigitizer();
  
 /** Standard contructor **/
 //or R3BCalifaDigitizer(Double_t esigma, Double_t tsigma, Double_t ysigma);
 R3BCalifaDigitizer(const TString& geoFile);

 /** Destructor **/
 ~R3BCalifaDigitizer();

 /** Virtual method Init **/
 virtual InitStatus Init();

 /** Virtual method Exec **/
 virtual void Exec(Option_t* opt);
  
 /** Virtual method EndOffEvent **/ 
 virtual void EndOfEvent();
  
 /** Virtual method Register **/
 virtual void Register();
 
 /** Virtual method Reset **/
 virtual void Reset();
 
 /** Virtual method FinishEvent **/
 virtual void FinishEvent();
   
 //Accessors functions  ?¿ 
 void SetEnergyResolution(Double_t resolution){fResolution=resolution;}
 void SetGeometryVersion(Double_t version){fGeometryVersion=version;}
 
  inline void ResetParameters()
	{
	};
 
  //...r3bdata/califa/  
  /** Constructor with arguments
   *@param fCrystalId   Crystal unique identifier
   *@param fEnergy      Total energy deposited on the crystal ([GeV] in sim)
   *@param fNf  				Total Nf (fast)
   *@param fNs					Total Ns (slow)
   *@param fTime        Time since event start [ns]
   *@param fToT_Energy  Total energy deposited on the crystal from ToT ([GeV] in sim)
   **/

	 /** Private method AddCrystalCal
   **
   ** Adds a CalifaCrystalCal data
   **/
  R3BCalifaCrystalCalData* AddCrystalCal(Int_t ident, 
  																			 Double_t energy,
  																			 Double_t Nf,
  																			 Double_t Ns,
  																			 ULong64_t time,
  																			 Double_t tot_energy=0.);
  																			   																			
  protected:
  
	  TClonesArray* fCalifaPointDataCA; //!  The crystal hit collection
	  TClonesArray* fCalifaCryCalDataCA; /**< Array with CALIFA Cal- output data. >*/
	  
  private:
      
    Int_t fResolution;  						//Resolution of the CALIFA calorimeter
    Int_t fGeometryVersion; 				//Selecting the geometry of the CALIFA calorimeter
		Double_t fNonUniformity;				//Adding some non-uniformity preliminary description
		
		
    /** Private method NUSmearing
     **
     ** Smears the energy according to some non-uniformity distribution
     ** Very simple preliminary scheme where the NU is introduced as a flat random
     ** distribution with limits fNonUniformity (%) of the energy value.
     **/
     Double_t NUSmearing(Double_t inputEnergy);
     
     /** Public method SetNonUniformity
     **
     ** Defines the fNonUniformity parameter in % deviation from the central value
     *@param nonU  Double parameter setting the maximum non-uniformity allowed
     **/
     void SetNonUniformity(Double_t nonU);
	
   ClassDef(R3BCalifaDigitizer,1);
  
};

#endif
