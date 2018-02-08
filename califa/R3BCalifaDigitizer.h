// ---------------------------------------------------------
// -----      R3BCalifaDigitizer header file           -----


#ifndef R3BCALIFADIGITISER_H
#define R3BCALIFADIGITISER_H


#include "FairTask.h"
#include "TClonesArray"
#include "R3BCalifaPoint.h"
#include "R3BCalifaCrystalCalData.h"


#include <TRandom3.h>
#include <string>


class TClonesArray;


class R3BCalifaDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  R3BCalifaDigitizer();
  
 /** Standard contructor **/
 //or R3BCalifaDigitizer(Double_t esigma, Double_t tsigma, Double_t ysigma);
 R3BCalifaDigitizer(const TString& geoFile,Int_t numGeoVersion);


  /** Destructor **/
  ~R3BCalifaDigitizer();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  virtual void Finish();
  virtual void Reset();
  
  //Accessors functions  ?¿ o hay que poner otras
  void SetEnergyResolution(Double_t resolution){fResolution=resolution;}
  void SetGeometryVersion(Double_t version){fGeometryVersion=version;}
  void SetNonUniformity(Double_t nonuniformity){fNonUniformity=nonuniformity;}
  
  //...r3bdata/califa/
  /** Constructor with arguments
  
  R3BCalifaCrystalCalData(Int_t ident, Double_t energy, Double_t Nf, Double_t Ns, ULong64_t time, Double_t tot_energy=0);
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
  R3BCalifaCrystalCalData* AddCrystalCal(Int_t cryid, 
  																			 Double_t energy,
  																			 Double_t Nf,
  																			 Double_t Ns,
  																			 ULong64_t time,
  																			 Double_t tot_energy);
  																			
  protected:
  
	  TClonesArray* fCalifaPointDataCA; //!  The crystal hit collection
	  TClonesArray* fCalifaCryCalDataCA; /**< Array with CALIFA Cal- output data. >*/
	  
  private:
  
   // Mapping of volume ID to crystal information
    std::map<Int_t, sCrystalInfo> fCrystalMap;

    // Current active crystal
    sCrystalInfo* fCrystal;

    /** Track information to be stored until the track leaves the
    active volume. **/
    Int_t fTrackID;                 //!  track index
    Int_t fTrackPID;                //!  particle identification
    Int_t fVolumeID;                //!  volume id
    Int_t fParentTrackID;           //!  parent track index
    Int_t fUniqueID;                //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut; //!  position
    TLorentzVector fMomIn, fMomOut; //!  momentum
    Double32_t fTime;               //!  time
    Double32_t fLength;             //!  length
    Double32_t fELoss;              //!  energy loss
    Double32_t fNf;                 //!  fast CsI(Tl) amplitude
    Double32_t fNs;                 //!  slow CsI(Tl) amplitude
    Int_t fPosIndex;                //!
    Int_t fNSteps;                  //!  Number of steps in the active volume
    Double32_t fEinc;               //!  Total incident energy
    Bool_t kGeoSaved;               //!
    TList* flGeoPar;                //!

  
    //Resolution of the CALIFA calorimeter
    Int_t fResolution;  
    // Selecting the geometry of the CALIFA calorimeter
    Int_t fGeometryVersion;
    // Adding some non-uniformity preliminary description
		Double_t fNonUniformity;
	
   ClassDef(R3BFi4Digitizer,1);
  
};

#endif
