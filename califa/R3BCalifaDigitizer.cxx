// ---------------------------------------------------------
// -----      R3BCalifaDigitizer source file           -----
#include "R3BCalifa.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "FairTask.h"
#include "R3BMCStack.h"
#include "TClonesArray.h"
#include "TGeoArb8.h"
#include "TGeoBBox.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoCone.h"
#include "TGeoMCGeometry.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoSphere.h"
#include "TGeoTube.h"
#include "TMCProcess.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"
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
R3BCalifaDigitizer::R3BCalifaDigitizer(const TString& geoFile,Int_t numGeoVersion) : FairTask("R3B CALIFA Digitizer"),
							   fCalifaPointDataCA(NULL),
							   fCalifaCryCalDataCA(NULL),
	 							 fNonUniformity(0),
    						 fGeometryVersion(1)						  
{
}

//Virtual R3BCalifaDigitizer: Destructor
R3BCalifaDigitizer::~R3BCalifaDigitizer()
{
  LOG(INFO) << "R3BCalifaDigitizer: Delete instance" << FairLogger::endl;
}


// -----   Public method Init   --------------------------------------------
InitStatus R3BCalifaDigitizer::Init()
{ 
  //INPUT DATA
  //Point data
  FairRootManager* rootManager = FairRootManager::Instance();
  if (!rootManager) { return kFATAL;}
  
  fCalifaPointDataCA = (TClonesArray*)rootManager->GetObject("CalifaPoint");
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
  
 
 
  /** Constructor with arguments
   *@param trackID  Index of MCTrack
   *@param detID    Detector ID
   *@param posIn    Ccoordinates at entrance to active volume [cm]
   *@param posOut   Coordinates at exit of active volume [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [GeV]
   *@param Nf       Fast component of CsI(Tl) light [a.u.]
   *@param Ns       Slow component of CsI(Tl) light [a.u.]
   **/
 /*																						en R3BCalifa.cxx
   R3BCalifaPoint* AddPoint(Int_t trackID,     //fTrackID
                             Int_t detID,      //fVolumeID
                             Int_t volid,      //fCrystal->crystalType
                             Int_t copy,       //fCrystal->crystalCopy
                             Int_t ident,       //fCrystal->crystalId
                             TVector3 posIn,    // TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z())
                             TVector3 pos_out,  //TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z())
                             TVector3 momIn,    //TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz())
                             TVector3 momOut,   //TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz())
                             Double_t time,     // fTime
                             Double_t length,   //fLength
                             Double_t eLoss,    //fELoss
                             Double_t Nf,       //fNf
                             Double_t Ns);      //fNs
                             */
                            
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
  TVector3 posIn;
  TVector3 posOut;
  TVector3 mom;
  
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
  
  
  
   	Bool_t existHit = 0;

/*
R3BCalifaCrystalCalData* AddCrystalCal(Int_t ident, 
  																			 Double_t energy,
  																			 Double_t Nf,
  																			 Double_t Ns,
  																			 ULong64_t time,
  																			 Double_t tot_energy);

*/



        if (nCrystalHits == 0)
            AddCrystalHit(fCrystal->crystalType,
                          fCrystal->crystalCopy,
                          fCrystal->crystalId,
                          NUSmearing(fELoss),
                          fNf,
                          fNs,
                          fTime,
                          fNSteps,
                          fEinc,
                          fTrackID,
                          fVolumeID,
                          fParentTrackID,
                          fTrackPID,
                          fUniqueID);
        else
        {
            for (Int_t i = 0; i < nCrystalHits; i++)
            {
                if (((R3BCalifaCrystalCalDataSim*)(fCaloCrystalHitCollection->At(i)))->GetCrystalId() ==
                    fCrystal->crystalId)
                {
                    ((R3BCalifaCrystalCalDataSim*)(fCaloCrystalHitCollection->At(i)))
                        ->AddMoreEnergy(NUSmearing(fELoss));
                    ((R3BCalifaCrystalCalDataSim*)(fCaloCrystalHitCollection->At(i)))->AddMoreNf(fNf);
                    ((R3BCalifaCrystalCalDataSim*)(fCaloCrystalHitCollection->At(i)))->AddMoreNs(fNs);
                    if (((R3BCalifaCrystalCalDataSim*)(fCaloCrystalHitCollection->At(i)))->GetTime() > fTime)
                    {
                        ((R3BCalifaCrystalCalDataSim*)(fCaloCrystalHitCollection->At(i)))->SetTime(fTime);
                    }
                    existHit = 1; // to avoid the creation of a new CrystalHit
                    break;
                }
            }
            if (!existHit)
                AddCrystalHit(fCrystal->crystalType,
                              fCrystal->crystalCopy,
                              fCrystal->crystalId,
                              NUSmearing(fELoss),
                              fNf,
                              fNs,
                              fTime,
                              fNSteps,
                              fEinc,
                              fTrackID,
                              fVolumeID,
                              fParentTrackID,
                              fTrackPID,
                              fUniqueID);
        }

        existHit = 0;
  
  
  
  
  
  
  }
}
