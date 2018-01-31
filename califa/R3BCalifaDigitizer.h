// ---------------------------------------------------------
// -----      R3BCalifaDigitizer header file           -----


#ifndef R3BCALIFADIGITISER_H
#define R3BCALIFADIGITISER_H


#include "FairTask.h"
#include "R3BCalifaPoint.h"
#include <TRandom3.h>
#include <string>


class TClonesArray;


class R3BCalifaDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  R3BCalifaDigitizer();
  
  /** Standard contructor **/
  R3BCalifaDigitizer(Double_t esigma, Double_t tsigma, Double_t ysigma);


  /** Destructor **/
  ~R3BCalifaDigitizer();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  virtual void Finish();
  virtual void Reset();
  
  void SetEnergyResolution(Double_t e);
  //void SetTimeResolution(Double_t t);
  //void SetYPositionResolution(Double_t y);
  
  //...r3bdata/califa/
  /** Constructor with arguments
  
  R3BCalifaCrystalCalData(Int_t ident, Double_t energy, Double_t Nf, Double_t Ns, ULong64_t time, Double_t tot_energy=0);
   *@param fCrystalId   Crystal unique identifier
   *@param fEnergy      Total energy deposited on the crystal ([GeV] in sim)
   *@param fToT_Energy  Total energy deposited on the crystal from ToT ([GeV] in sim)
   *@param fTime        Time since event start [ns]
   **/

  R3BCalifaCrystalCalData* AddCrystalCal(Int_t ident, 
  																			 Double_t energy,
  																			 Double_t Nf,
  																			 Double_t Ns,
  																			 ULong64_t time,
  																			 Double_t tot_energy);
  

  protected:
  
	  TClonesArray* fCaloCrystalHitCollection; //!  The crystal hit collection
	  
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

  
    // Selecting the geometry of the CALIFA calorimeter
    Int_t fGeometryVersion;
    // Adding some non-uniformity preliminary description
		Double_t fNonUniformity;
	
   ClassDef(R3BFi4Digitizer,1);
  
};

#endif
