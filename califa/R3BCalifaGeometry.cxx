#include <vector>

#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TMath.h>
#include <TVector3.h>
#include <TGeoNavigator.h>
#include <FairLogger.h>
#include "R3BCalifaGeometry.h"

#include <iostream>
#include <stdlib.h>

using std::cout;
using std::cerr;
using std::endl;

//R3BCalifaGeometry* R3BCalifaGeometry::inst = NULL;

/*R3BCalifaGeometry* R3BCalifaGeometry::Instance(int version)
{
  if(!inst)
    inst = new R3BCalifaGeometry(version);
  else if(inst->fGeometryVersion != version)
  {
    LOG(ERROR) << "R3BCalifaGeometry::Instance(): Existing instance with different geometry version than requested. "
              << "Undefined beheaviour possible!" << FairLogger::endl;

    inst = new R3BCalifaGeometry(version);
  }

  return inst;
}*/

/*R3BCalifaGeometry::R3BCalifaGeometry(int version)  : fGeometryVersion(version), fNavigator(NULL)
{
  LOG(DEBUG) << "Creating new R3BCalifaGeometry for version " << version << FairLogger::endl;

  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  geoPath += "/geometry/";

  switch(version)
  {
  case 16:
    // Barrel 8.11 with displaced crystal centers
    geoPath += "califa_16_v8.11_cc0.2.geo.root";
    break;

  case 17:
    // Barrel 8.11 with crystal centers at (0,0,0) in local system
    geoPath += "califa_17_v8.11_cc0.2.geo.root";
    break;

  case 0x438b:
    // s438b geometry (subset of 8.11 Barrel + rotation)
    geoPath += "califa_demo.geo.root";
    break;

  default:
    LOG(ERROR) << "R3BCalifaGeometry: Unsupported geometry version: " << version << FairLogger::endl;
    return;
  }

  if(gGeoManager && strcmp(gGeoManager->GetTopVolume()->GetName(), "cave") == 0)
  {
    // Already set up (MC mode)
    LOG(DEBUG) << "R3BCalifaGeometry: Using existing geometry" << FairLogger::endl;
    return;
  }

  // Stand alone mode
  LOG(DEBUG) << "R3BCalifaGeometry: Creating new geometry" << FairLogger::endl;
  TFile *f = new TFile(geoPath, "READ");
  TGeoVolume *v = dynamic_cast<TGeoVolume*>(f->Get("TOP"));
  if(!v)
  {
    LOG(ERROR) << "R3BCalifaGeometry: Could not open CALIFA geometry file: No TOP volume" << FairLogger::endl;
    return;
  }

  v->SetName("cave");
  if(!gGeoManager)
    gGeoManager = new TGeoManager();
  gGeoManager->SetTopVolume(v);

  fNavigator = new TGeoNavigator(gGeoManager);
}*/

void R3BCalifaGeometry::GetAngles(Int_t iD, Double_t & polar, //Working 
				 Double_t & azimuthal, Double_t & rho)
{

  Double_t local[3]={0,0,0};
  Double_t master[3];
  Int_t crystalType = 0;
  Int_t crystalCopy = 0;
  Int_t alveolusCopy =0;
  Int_t crystalInAlveolus=0;

    Char_t nameVolume[200];
		/*BARREL
		// First ring (single crystals)
						crystalType = 1;  //Alv type 1
						crystalCopy = iD;     //for Alv type 1 
						alveolusCopy = iD;    //Alv type 1 
						crystalInAlveolus =1;          //Alv type 1
								
		// Ring 2 - 16: 2x2 crystals
						Alv type or crystalType(2 to 16)
						CrystalCopy (1 to 128)
						Alveolus copy (1 to 32)
						Crystal number in alveolus (1 to 4)*/
   if (iD >= 1 && iD <= 1952) {
      if(iD<33)
      {// First ring (single crystals)
        crystalType = 1;  //Alv type 1
        crystalCopy = iD; 
        alveolusCopy = iD;
        crystalInAlveolus =1;
      }
      else
      {// Ring 2 - 16: 2x2 crystals
        crystalType = (Int_t)((iD-33)/128) + 2;  //Alv type (2 to 16)	
        crystalCopy = ((iD-33)%128) + 1;         //CrystalCopy (1 to 128)
        alveolusCopy =(Int_t)(((iD-33)%128)/4) +1; //Alveolus copy (1 to 32)
        crystalInAlveolus = (iD-33)%4 + 1;//Crystal number in alveolus (1 to 4)
      }
      Int_t alveoliType[16]={1,2,2,2,2,3,3,4,4,4,5,5,5,6,6,6};
   
      sprintf(nameVolume, 
	      "/cave_1/CalifaWorld_0/Alveolus_%i_%i/AlveolusInner_%i_1/CrystalWithWrapping_%i_%i_%i/Crystal_%i_%i_1",
	      crystalType, alveolusCopy-1, 
	      crystalType, alveoliType[crystalType-1], 
	      crystalInAlveolus, crystalInAlveolus-1, 
	      alveoliType[crystalType-1], crystalInAlveolus);

				//cout<<">>> CALIFA Geometry: ------ GetAngles ------"<<endl;
				//cout<<"iD= "<<iD<<endl;
				//cout<<"crystalType=1 or (Int_t)((iD-33)/128) + 2    ->"<<crystalType<<endl;
				//cout<<"crystalCopy=iD or ((iD-33)%128) + 1;     ->"<<crystalCopy<<endl;
				//cout<<"alveolusCopy=iD or (Int_t)(((iD-33)%128)/4) +1   ->"<<alveolusCopy<<endl;
				//cout<<"crystalInAlveolus=1 or (iD-33)%4 + 1     ->"<<crystalInAlveolus<<endl;
				//cout<<"nameVolume= "<<nameVolume<<endl<<endl;
//------------------------------------------------------------------------------------------------------   
// CHECK THIS with the new Geo!!
      // The definition of the crystals is different in this particular EndCap design:         
      // the origin for each crystal is the alveoli corner
      if (crystalType==1) {
	local[0]=27.108/8; local[1]=-28.0483/8; local[2]=0;
      } else if (crystalType==2 || crystalType==3 || 
		 crystalType==4 || crystalType==5) {
	if(crystalInAlveolus==1){
	  local[0]=37.4639/8; local[1]=-8.57573/8; local[2]=0;
	} else if(crystalInAlveolus==2) {
	  local[0]=37.4639/8; local[1]=-31.1043/8; local[2]=0;
	} else if(crystalInAlveolus==3) {
	  local[0]=9.52012/8; local[1]=-8.57573/8; local[2]=0;
	} else if(crystalInAlveolus==4){
	  local[0]=9.52012/8; local[1]=-31.1043/8; local[2]=0;
	}
      } else if (crystalType==6 || crystalType==7) {
	if(crystalInAlveolus==1){
	  local[0]=38.3282/8; local[1]=-5.49819/8; local[2]=0;
	} else if(crystalInAlveolus==2) {
	  local[0]=38.3282/8; local[1]=-23.0538/8; local[2]=0;
	} else if(crystalInAlveolus==3) {
	  local[0]=8.66384/8; local[1]=-5.49819/8; local[2]=0;
	} else if(crystalInAlveolus==4){
	  local[0]=8.66384/8; local[1]=-23.0538/8; local[2]=0;
	}
      } else if (crystalType==8 || crystalType==9 || crystalType==10) {
	if(crystalInAlveolus==1){
	  local[0]=38.3683/8; local[1]=-4.71618/8; local[2]=0;
	} else if(crystalInAlveolus==2) {
	  local[0]=38.3683/8; local[1]=-19.8438/8; local[2]=0;
	} else if(crystalInAlveolus==3) {
	  local[0]=8.43569/8; local[1]=-4.71618/8; local[2]=0;
	} else if(crystalInAlveolus==4){
	  local[0]=8.43569/8; local[1]=-19.8438/8; local[2]=0;
	}
      } else if (crystalType==11 || crystalType==12 || crystalType==13) {
	if(crystalInAlveolus==1){
	  local[0]=38.3495/8; local[1]=-4.70373/8; local[2]=0;
	} else if(crystalInAlveolus==2) {
	  local[0]=38.3495/8; local[1]=-19.8403/8; local[2]=0;
	} else if(crystalInAlveolus==3) {
	  local[0]=8.66654/8; local[1]=-4.70373/8; local[2]=0;
	} else if(crystalInAlveolus==4){
	  local[0]=8.66654/8; local[1]=-19.8403/8; local[2]=0;
	}
      } else if (crystalType==14 || crystalType==15 || crystalType==16) {
	if(crystalInAlveolus==1){
	  local[0]=37.9075/8; local[1]=-4.66458/8; local[2]=0;
	} else if(crystalInAlveolus==2) {
	  local[0]=37.9075/8; local[1]=-19.8474/8; local[2]=0;
	} else if(crystalInAlveolus==3) {
	  local[0]=9.07247/8; local[1]=-19.8474/8; local[2]=0;
	} else if(crystalInAlveolus==4){
	  local[0]=9.07247/8; local[1]=-4.66458/8; local[2]=0;
	}
      }		
//------------------------------------------------------------------------------------------------------
      gGeoManager->CdTop();

      if(gGeoManager->CheckPath(nameVolume)) gGeoManager->cd(nameVolume);
      else { 
	LOG(ERROR) << "R3BCalifaGeometry: Invalid crystal path: " << nameVolume
		   << FairLogger::endl;
	return; 
      }
      gGeoManager->LocalToMaster(local, master);

    } else{
		/*ENDCAP Volumes Info:		    
				crystalType from 1 to 24
				crystalCopy from 1 to 32
				crystalId from 3000 to 3767
		*/

      crystalType = ((iD - 3000) % 24) + 1; //from 1 to 24
      crystalCopy = (iD-3000 - crystalType + 1) / 24 + 1;//from 1 to 32
      Int_t alveoliType[24]={1,1,2,2,3,3,4,4,5,6,7,8,9,9,10,10,11,11,12,12,13,13,14,14};
      Int_t wrappingType[24]={1,1,2,2,3,3,4,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

      sprintf(nameVolume, "/cave_1/CalifaWorld_0/Alveolus_EC_%i_%i/CrystalWithWrapping_%i_1/Crystal_%i_1",
								      alveoliType[crystalType-1], crystalCopy-1, wrappingType[crystalType-1], crystalType);
      gGeoManager->cd(nameVolume);
      gGeoManager->LocalToMaster(local, master);
			//cout<<"CALIFA Geometry iD= "<<iD<<endl;
			//cout<<"crystalType=((iD - 3000) % 24) + 1   ->"<<crystalType<<endl;
			//cout<<"crystalCopy=(iD-3000 - crystalType + 1) / 24 + 1     ->"<<crystalCopy<<endl;
			//cout<<"alveoliType[crystalType-1   ->"<<alveoliType[crystalType-1]<<endl;
			//cout<<"wrappingType[crystalType-1]    ->"<<wrappingType[crystalType-1]<<endl;
			//cout<<"nameVolume= "<<nameVolume<<endl<<endl;
    }
    
		TVector3 masterV(master[0],master[1],master[2]);
		//masterV.Print();
		polar=masterV.Theta();
		azimuthal=masterV.Phi();
		rho=masterV.Mag();
		//cout<<"CALIFA Geometry: "<<"  polar="<<polar<<"  azimuthal="<<azimuthal<<" rho="<<rho<<endl<<endl;
}

const char* R3BCalifaGeometry::GetCrystalVolumePath(int iD) //WORKING
{
  // Must be static since function returns string
  // owned by nameVolume, which must not be destroyed
  static TString nameVolume;

  Int_t crystalType = 0;
  Int_t crystalCopy = 0;
  Int_t alveolusCopy =0;
  Int_t crystalInAlveolus=0;

    if (iD >= 1 && iD <= 1952)
    {
      // Barrel
      if(iD<33)
      {
        // First ring (single crystals)
        crystalType = 1;  //Alv type 1
        crystalCopy = iD;     //for Alv type 1 
        alveolusCopy = iD;    //Alv type 1 
        crystalInAlveolus =1;          //Alv type 1
      }
      else
      {
        // Ring 2 - 16: 2x2 crystals
        crystalType = (Int_t)((iD-33)/128) + 2;  //Alv type (2 to 16)	       OK! 
        crystalCopy = ((iD-33)%128) + 1;         //CrystalCopy (1 to 128)
        alveolusCopy =(Int_t)(((iD-33)%128)/4) +1; //Alveolus copy (1 to 32)
        crystalInAlveolus = (iD-33)%4 + 1;//Crystal number in alveolus (1 to 4)
      }
      Int_t alveoliType[16]={1,2,2,2,2,3,3,4,4,4,5,5,5,6,6,6};
    
      nameVolume = TString::Format( 
	      "/cave_1/CalifaWorld_0/Alveolus_%i_%i/AlveolusInner_%i_1/CrystalWithWrapping_%i_%i_%i/Crystal_%i_%i_1",
	      crystalType, alveolusCopy-1, 
	      crystalType, alveoliType[crystalType-1], 
	      crystalInAlveolus, crystalInAlveolus-1, 
	      alveoliType[crystalType-1], crystalInAlveolus);
    }
    else if(iD >= 3000)
    {
      //For CC iPhos+phoswich endcap
      crystalType = ((iD - 3000) % 24) + 1;  //crystalType (1 to 24)      										OK!
      crystalCopy = (iD-3000 - crystalType + 1) / 24 + 1;//crystalCopy (1 to 32)  						 OK!
      Int_t alveoliType[24]={1,1,2,2,3,3,4,4,5,6,7,8,9,9,10,10,11,11,12,12,13,13,14,14};			//OK
      Int_t wrappingType[24]={1,1,2,2,3,3,4,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};			//OK
     
      nameVolume = TString::Format("/cave_1/CalifaWorld_0/Alveolus_EC_%i_%i/CrystalWithWrapping_%i_1/Crystal_%i_1",
              alveoliType[crystalType-1], crystalCopy-1, wrappingType[crystalType-1], crystalType);

    }
    else
    {
      LOG(ERROR) << "R3BCalifaGeometry: Invalid crystal ID " << iD << FairLogger::endl;
      return NULL;
    }
		//cout<<"CALIFA Geometry iD= "<<iD<<endl;
		//cout<<"nameVolume= "<<nameVolume<<endl<<endl;

  return nameVolume.Data();
}




double R3BCalifaGeometry::GetDistanceThroughCrystals(TVector3 &startVertex, TVector3 &direction, TVector3 *hitPos, int *numCrystals, int *crystalIds)
{
 int maxNumCrystals = 0;

  if(numCrystals != NULL && crystalIds != NULL)
  {
    maxNumCrystals = *numCrystals;
		//cout<<"maxNumCrystals="<<maxNumCrystals<<endl;
    *numCrystals = 0;
  }

  TGeoNode *n;
//InitTrack() Setting both initial point and direction and finding the state: 
  gGeoManager->InitTrack(startVertex.X(), startVertex.Y(), startVertex.Z(),
      direction.X()/direction.Mag(), direction.Y()/direction.Mag(), direction.Z()/direction.Mag());

  double distance = 0;
  const Double_t *pos;
  bool inCrystal = false, wasInCrystal = false;
  TString nodeName;

//find the distance to the next boundary and then extrapolate the current point/direction 
//with this distance making sure that the boundary was crossed. Finally the goal would be to find the next state 
//after crossing the boundary
  while((n = gGeoManager->FindNextBoundaryAndStep()))
  {
    nodeName = n->GetName();
		//cout<<"nodeName="<<nodeName<<endl;

    if(inCrystal) distance += gGeoManager->GetStep();
			//cout<<"distance="<<distance<<endl;

    inCrystal = nodeName.BeginsWith("CrystalWithWrapping_");//can't enter to Crystal volume
		//cout<<"inCrystal="<<inCrystal<<endl;
		//cout<<"maxNumCrystals="<<maxNumCrystals<<endl;

		//maxNumCrystals=500;

    if(inCrystal && maxNumCrystals != 0)//this works if we put a maxNumCrystals different of 0 
    {
      int cid = GetCrystalId(gGeoManager->GetPath());
			//cout<<"----------------------  HEREEEEE                       cid="<<cid<<endl;
      if(cid != -1 && (*numCrystals == 0 || cid != crystalIds[(*numCrystals)-1]))
      {
        crystalIds[(*numCrystals)++] = cid;
        maxNumCrystals--;
      }
    }

    if(hitPos != NULL && inCrystal && !wasInCrystal)
    {
      pos = gGeoManager->GetCurrentPoint();
      hitPos->SetXYZ(pos[0], pos[1], pos[2]);
      wasInCrystal = true;
    }
  }
	
  return distance;
}





//Waiting for what we should do in this function, PROBLEM: path size is variable
//path=/cave_1/CalifaWorld_0/Alveolus_3_13/AlveolusInner_3_1/CrystalWithWrapping_2_2_1/Crystal_2_2_1
int R3BCalifaGeometry::GetCrystalId(const char *volumePath)
{
  std::vector<const char*> volumeNames;
  std::vector<int> nodeCopies;
  TGeoNode *n;

  int crystalId = -1;

	//cout<<">>>CALIFA Geometry: Volume path "<<volumePath<<endl;

  /*for(fNavigator->cd(volumePath); (n = fNavigator->GetCurrentNode()) != NULL; fNavigator->CdUp())
  {
    volumeNames.push_back(n->GetName());
    nodeCopies.push_back(n->GetNumber());
  }
    
    if(volumeNames.size() < 4)
    {
      LOG(ERROR) << "R3BCalifaGeometry::GetCrystalId(): Invalid path: " << volumePath << FairLogger::endl;
      return -1;
    }

    int cp1 = nodeCopies[0];
    int cpCry = nodeCopies[1];
    int cpAlv = nodeCopies[2];
    int cpSupAlv = nodeCopies[3];

cout<<">> CALIFA Geometry path= "<<volumePath<<endl;
cout<<">> CALIFA Geometry cp1="<<cp1<<endl;
cout<<">> CALIFA Geometry cpCry="<<cpCry<<endl;
cout<<">> CALIFA Geometry cpAlv="<<cpAlv<<endl;
cout<<">> CALIFA Geometry cpSupAlv="<<cpSupAlv<<endl;

    int crystalType, crystalCopy;

    const char *alveolusECPrefix = "Alveolus_EC";
    const char *alveolusPrefix = "Alveolus";
    const char *volumeName = volumeNames[3];
    const char *volumeNameCrystal ="";

    // Workaround to fix the hierarchy difference between Barrel and Endcap
    if (strncmp("CalifaWorld", volumeName,10) == 0) {
      volumeName = volumeNames[2];
      volumeNameCrystal = volumeNames[0];
    }
    if (strncmp(alveolusECPrefix, volumeName, 11) == 0) {
      crystalType = atoi(volumeNameCrystal+8);     //converting to int the crystal index       Comprobar que volumeNameCrystal==Crystal_i_1
      crystalCopy = cpAlv+1;

      if(crystalType < 9 && crystalType%2 == 0) {						//?????POR¿
	  crystalType -= 1;
      }
      crystalId = 3000 + cpAlv*24 + (crystalType-1);
      
      if (crystalType>24 || crystalType<1 ||
	  crystalCopy>32 || crystalCopy<1 || 
          crystalId<3000 || crystalId>4800)
      {
	LOG(ERROR) << "R3BCalifaGeometry: Wrong crystal number in geometryVersion 16+ (CC). " 
		   << FairLogger::endl;
        return -1;
      }
    //if BARREL
    } else if (strncmp(alveolusPrefix, volumeName,8) == 0) {

      crystalType = atoi(volumeName+9);//converting to int the alveolus index
      if (crystalType==1) {
	//only one crystal per alveoli in this ring, running from 1 to 32
        crystalCopy = cpSupAlv+1; 
        crystalId = cpSupAlv+1;                    
      } else if (crystalType>1 && crystalType<17) {
	//running from 0*4+0+1=1 to 31*4+3+1=128
        crystalCopy = cpSupAlv*4+cpCry+1;
	//running from 32+0*128+0*4+0+1=1 to 32+14*128+31*4+3+1=1952
        crystalId = 32+(crystalType-2)*128+cpSupAlv*4+cpCry+1; 
      }
      if (crystalType>16 || crystalType<1 || crystalCopy>128 || 
	  crystalCopy<1 || crystalId>1952 || crystalId<1) 
      {
        LOG(ERROR) << "R3BCalifaGeometry: Wrong crystal number in geometryVersion 16+ (BARREL)." 
		   << FairLogger::endl;
        return -1;
      }
    }
    else
    {
      LOG(ERROR) << "R3BCalifaGeometry: Impossible crystalType for geometryVersion 16+." 
		      << FairLogger::endl;
      return -1;
    }*/
	//cout<<">> CALIFA Geometry crystalId="<<crystalId<<endl;
  return crystalId;
}

ClassImp(R3BCalifaGeometry);
