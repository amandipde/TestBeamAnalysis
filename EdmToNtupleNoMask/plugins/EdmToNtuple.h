#ifndef __TestBeamAnalysis_EdmToNtupleNoMask_EdmToNtupleNoMask_h
#define __TestBeamAnalysis_EdmToNtupleNoMask_EdmToNtupleNoMask_h


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/AnalysisObjects.h"

#include "TFile.h"
#include "TTree.h"
#include<stdint.h>

class CbcConfig
{
   public:
      CbcConfig(uint32_t cwdWord=0, uint32_t windowWord=0);
      int window;
      int offset1;
      int offset2;
      int CWD;
};

class EdmToNtupleNoMask : public edm::EDAnalyzer
{
 public:
  explicit EdmToNtupleNoMask(const edm::ParameterSet& iConfig);
  ~EdmToNtupleNoMask(){}
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  static bool sortEvent( const tbeam::Event& a,  const tbeam::Event& b);
  std::vector < tbeam::cluster *> clusterizer(std::vector <int> hits);

  uint32_t stubWordGenerator(std::vector <tbeam::stub*> stubs);
  //std::vector <tbeam::stub*> stubSimulator(std::vector<tbeam::cluster *> * seeding, std::vector< tbeam::cluster *> *matching);
  void stubSimulator (std::vector < tbeam::cluster*>* seeding, std::vector < tbeam::cluster*>* matching, const int CBCSIZE, 
                      std::vector<tbeam::stub*>& stubs, std::vector<tbeam::stub*>& fstubs);
  TTree* tree_;
  std::vector<tbeam::Event> v_evtInfo_;
  tbeam::Event ev;
  tbeam::condEvent* cEvent_;
  tbeam::dutEvent* dEvent_;
  CbcConfig cbcConfiguration; 
  std::map<int,std::string> detIdNamemap_;
  const int verbosity_;
  int nCBC_;
  int nStripsPerCBC_;
  uint32_t tdcAdd_;
  uint32_t hvAdd_;
  uint32_t DUTangAdd_;      
  uint32_t stubAdd_;
  uint32_t cwdAdd_;
  uint32_t tiltAdd_;
  uint32_t vcthAdd_;
  uint32_t offsetAdd_;
  uint32_t windowAdd_;
  uint32_t stubLatencyAdd_;
  uint32_t triggerLatencyAdd_;
  
  uint32_t stubWord_;
  uint32_t stubWordReco_;
  //int condData_;
  int cbc2Status_;
  unsigned int HVsettings_;
  unsigned int DUTangle_;
  bool useCustomizedCWDcuts_;
  unsigned int cwdCutcustomized_;
  unsigned int smatchWidndowCutcustomized_;
};

#endif
