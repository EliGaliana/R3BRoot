
typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
} EXT_STR_h101;


void califa_febex3_online(){
  TStopwatch timer;
  timer.Start();
  
  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
  
  // Create source using ucesb for input ---------------------------------------
  //TString filename = "--stream=localhost";
  TString filename = "--stream=lxg0898:6002";
  // TString filename = "/u/land/lynx.landexp/201810_s444/x86l-37/daq_bootstrap/data/004_2018-07-13_00-08-53/data_0001.lmd";
  TString outputFileName = "./data_online.root";
  TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/califa_s444/califa";
  EXT_STR_h101 ucesb_struct;
  
  R3BUcesbSource* source = 
    new R3BUcesbSource(filename, ntuple_options,
		       ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa,
					     offsetof(EXT_STR_h101, califa)));
  
  // Create online run ---------------------------------------------------------
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(181);

  run->ActivateHttpServer(2000,8004);//(timeout,outputport)
  //run->SetAutoFinish(kFALSE);
  run->SetOutputFile(outputFileName);

    // Runtime data base ---------------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //Choose Root or Ascii file	
  //1-Root file with the Calibartion Parameters  
  //FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();//Ascii
  //parIo1->open("Params.par","in");
  //FairParRootFileIo* parIo1 = new FairParRootFileIo();//Root
  //parIo1->open("Califa_CalibParam_DoublePetal.root","in");
  //rtdb->setFirstInput(parIo1);
  //rtdb->print();
  
  //2-Ascii file with the Calibartion Parameters
  /*FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open("Califa_CalibParam.par","out");
    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
    rtdb->print();*/

  
  // Create analysis task ---------------------->>---------------------------------

  //R3BCalifaMapped2CrystalCal ---
  R3BCalifaMapped2CrystalCal* Map2Cal = new R3BCalifaMapped2CrystalCal();
  run->AddTask(Map2Cal);

  // R3BOnlineSpectra ----------------------------------------------------------
  Bool_t ON=false;
  Int_t petals=7;
  //Int_t crystalId=65;
  
  R3BCalifaOnlineSpectra* online= new R3BCalifaOnlineSpectra();
  online->SetDisplayCalOn(ON);
  online->SetPetals(petals);
  // online->SetCalifaConfigFile("CalifaPetal_5122.txt");
  // online->SetOneCrystal(crystalId);
  run->AddTask(online);

  //R3BAmsOnlineSpectra* onlineams= new R3BAmsOnlineSpectra();  
  //run->AddTask(onlineams);
  
  // Initialize ----------------------------------------------------------------
  //run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  

  
  // Run -----------------------------------------------------------------------
  run->Init();
  run->Run(nev, 0);
  //rtdb->saveOutput();
  //delete run;

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished successfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " 
       << ctime << "s" << endl << endl;
}

