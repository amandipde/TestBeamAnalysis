#ifndef __TestBeamAnalysis_EdmToNtupleNoMask_EdmToNtupleNoMask_h
#define __TestBeamAnalysis_EdmToNtupleNoMask_EdmToNtupleNoMask_h


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Event.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Hit.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Cbc.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Cluster.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Stub.h"

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

static constexpr unsigned int MASK_BITS_8  = 0xFF;
static constexpr unsigned int MASK_BITS_4  = 0xF;
class EdmToNtupleNoMask : public edm::one::EDAnalyzer<>
{
 public:
  explicit EdmToNtupleNoMask(const edm::ParameterSet& iConfig);
  ~EdmToNtupleNoMask(){}
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  const int verbosity_;
  void offlineclusterizer(const std::vector<tbeam::hit>& hits, const unsigned int nCbc,
                          const unsigned int nChanPercbc, std::vector<tbeam::cluster>& clusVec);
  void stubSimulator (const std::vector<tbeam::cluster>& seeding, const std::vector<tbeam::cluster>& matching, std::vector<tbeam::stub>& stubVec);
  //variables passed from config
  const edm::EDGetTokenT<FEDRawDataCollection> fedrawdataToken_;
  const edm::EDGetTokenT<edm::DetSetVector<Phase2TrackerDigi>>  unsparsifiedDigiToken_;
  const edm::EDGetTokenT<edmNew::DetSetVector<Phase2TrackerCluster1D>>  sparsifiedClusterToken_;
  const edm::EDGetTokenT<edmNew::DetSetVector<Phase2TrackerStub>>       stubToken_;
  std::vector<std::string> detIdBottomVec_;
  std::vector<std::string> detIdTopVec_;
  const bool isSparisifiedMode_;
  
  //variables of the ntuplizer
  TTree* tree_;
  tbeam::Event ev_;
  CbcConfig cbcConfiguration; 

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
};

#endif
