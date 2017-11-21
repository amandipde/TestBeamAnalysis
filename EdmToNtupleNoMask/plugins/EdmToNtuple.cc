// -*- C++ -*-
//
// Package:    EdmToNtupleNoMask
// Class:      EdmToNtupleNoMask
// 
/**\class EdmToNtupleNoMask EdmToNtupleNoMask.cc TestBeamAnalysis/TreeMaker/plugins/EdmToNtupleNoMask.cc
 Description: Main class to process unpacked test beam data 
 Implementation:
     [Notes on implementation]
*/
//
// Author:  Ali Harb, Suvankar Roy Chowdhury, Martin Delcourt, Nicolas Chanon, Kirill Skovpen
// 
//
#include "EventFilter/Phase2TrackerRawToDigi/interface/Phase2TrackerFEDBuffer.h"
#include "EventFilter/Phase2TrackerRawToDigi/interface/Phase2TrackerFEDHeader.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigiCollection.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/src/fed_header.h"

#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerDigi.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerCommissioningDigi.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerCommissioningDigi.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerDigi.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerHeaderDigi.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerStub.h"
#include "DataFormats/Phase2TrackerCluster/interface/Phase2TrackerCluster1D.h"

#include "TestBeamAnalysis/EdmToNtupleNoMask/plugins/EdmToNtuple.h"
#include<vector>
#include<algorithm>
#include<iomanip>
#include<bitset>
using namespace Phase2Tracker;

CbcConfig::CbcConfig(uint32_t cwdWord, uint32_t windowWord){
  window = windowWord >>4;
  offset1 = (cwdWord)%4;
  if ((cwdWord>>2)%2) offset1 = -offset1;
  offset2 = (cwdWord>>3)%4;
  if ((cwdWord>>5)%2) offset2 = -offset2;
  CWD = (cwdWord>>6)%4;
}
EdmToNtupleNoMask::EdmToNtupleNoMask(const edm::ParameterSet& iConfig) :
  verbosity_(iConfig.getUntrackedParameter<int>("verbosity", 0)),
  fedrawdataToken_(consumes<FEDRawDataCollection>(iConfig.getParameter<edm::InputTag>("fedRawData"))),
  unsparsifiedDigiToken_(consumes<edm::DetSetVector<Phase2TrackerDigi>>(iConfig.getParameter<edm::InputTag>("unsparisifiedDigiColl"))),
  sparsifiedClusterToken_(consumes<edmNew::DetSetVector<Phase2TrackerCluster1D>>(iConfig.getParameter<edm::InputTag>("cluster1DColl"))),
  stubToken_(consumes<edmNew::DetSetVector<Phase2TrackerStub>>(iConfig.getParameter<edm::InputTag>("stubColl"))),
  detIdBottomVec_(iConfig.getParameter< std::vector<std::string> >("detIdBottomVec")),
  detIdTopVec_(iConfig.getParameter< std::vector<std::string> >("detIdTopVec")),
  isSparisifiedMode_(iConfig.getParameter<bool>("isSparsified"))
{
  if( detIdBottomVec_.empty() || detIdTopVec_.empty() || detIdBottomVec_.size() != detIdTopVec_.size() ) {
    std::cout << "DetId information error!! Please check PSet" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void EdmToNtupleNoMask::beginJob()
{
  //ev_ = new tbeam::Event();
  edm::Service<TFileService> fs;
  fs->file().cd("/");
  tree_ = fs->make<TTree>("tbeamTree", "AnalysisTree no mask");
  //ev is a dummy variable of type tbeam::Event
  tree_->Branch("Event",&ev_);
}

void EdmToNtupleNoMask::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // event info
  ev_.run = iEvent.id().run();
  ev_.event = iEvent.id().event();  
  ev_.lumiSection = iEvent.luminosityBlock();
  edm::Timestamp itime = iEvent.time();
  ev_.time = itime.value();
  ev_.unixtime = itime.unixTime();
  //Tracker Header
  edm::Handle<FEDRawDataCollection> buffers;
  iEvent.getByToken(fedrawdataToken_,buffers);
  size_t fedIndex;
  for( fedIndex = Phase2Tracker::FED_ID_MIN; fedIndex < Phase2Tracker::CMS_FED_ID_MAX; ++fedIndex ) {
    const FEDRawData& fed = buffers->FEDData(fedIndex);
    if(fed.size()==0) continue;
    // construct buffer
    Phase2Tracker::Phase2TrackerFEDBuffer buffer(fed.data(),fed.size());
    if(verbosity_)  std::cout << " buffer size : " << buffer.bufferSize() << std::endl;
    if(verbosity_)  std::cout << " fed id      : " << fedIndex << std::endl;
    //Fill info in object
    Phase2TrackerHeaderDigi tr_header = Phase2TrackerHeaderDigi(buffer.trackerHeader());
    ev_.dataFormatVersion = tr_header.getDataFormatVersion();
    ev_.debugMode = tr_header.getDebugMode();
    ev_.readoutMode = tr_header.getReadoutMode();
    ev_.dataType = tr_header.getDataType();
    ev_.glibStatusCode = tr_header.getGlibStatusCode();
    ev_.condData = tr_header.getConditionData(); 
    ev_.numberOfCBC = tr_header.getNumberOfCBC();   
    //loop over condition data
    ev_.conddatamap = buffer.conditionData();
    for(auto& it : buffer.conditionData()){ 
      uint8_t uid     = (it.first >> 24)  & MASK_BITS_8;
      //The following can be saved if necessary
      //uint8_t i2cReg  = (it.first >> 16)  & MASK_BITS_8;
      //uint8_t i2cPage = (it.first >> 12)  & MASK_BITS_4;
      //uint8_t roId    = (it.first >> 8)   & MASK_BITS_4;
      //uint8_t feId    = (it.first)        & MASK_BITS_8;
      if(uid == 1)       ev_.vcth = (int)it.second;
      else if(uid == 3)  ev_.tdcPhase =  (int)it.second;  
      else if(uid == 5)  ev_.HVsettings = (int)it.second;  
      else if(uid == 4)  ev_.DUTangle = (int)it.second;  
    }
    //get cbc status
    for (unsigned int fi = 0; fi < buffer.trackerHeader().CBCStatus().size(); fi++) { 
      Phase2TrackerFEDFEDebug& FE_it = buffer.trackerHeader().CBCStatus()[fi];
      if(FE_it.IsOn()) {
        std::vector<tbeam::cbc> tempCbc;
        for (int ci=0; ci<tr_header.getNumberOfCBC(); ci++) {
          tempCbc.emplace_back(tbeam::cbc(FE_it.getChipDebugStatus(ci), FE_it.getChipError(ci),
                                          FE_it.getChipPipelineAddress(ci), FE_it.getChipL1ID(ci)) ); 
        }
        ev_.cbcs.insert({fi,tempCbc});
      }
    }
  }
  ev_.isSparisified = isSparisifiedMode_;
  //IF unsparisified read channel data!!Otherwise read clusters
  if(!isSparisifiedMode_) {
    //Unspasified Data
    edm::Handle< edm::DetSetVector<Phase2TrackerDigi> > usp_digi_detset;
    iEvent.getByToken(unsparsifiedDigiToken_, usp_digi_detset);
    
    for(auto& det : *usp_digi_detset) {
      std::vector<tbeam::hit> temphits;
      for(auto& hit : det) {
        temphits.emplace_back(tbeam::hit(hit.row(), hit.column(), 
                                         hit.strip(), hit.edge(),
                                         hit.channel(), hit.overThreshold()) );
      }
      ev_.dutHits.insert({std::to_string(det.id), temphits});
      std::vector<tbeam::cluster> tempclus;
      offlineclusterizer(temphits, ev_.numberOfCBC, 127, tempclus);
      ev_.offlineClusters.insert({std::to_string(det.id), tempclus});
    }
    //run the offline stub simulator
    for(unsigned int i = 0; i < detIdBottomVec_.size(); i++) {
      if(ev_.offlineClusters.find(detIdBottomVec_[i]) == ev_.offlineClusters.end() 
         || ev_.offlineClusters.find(detIdTopVec_[i]) == ev_.offlineClusters.end() )     continue;
      std::vector<tbeam::stub>  tempStubs;
      stubSimulator (ev_.offlineClusters[detIdBottomVec_[i]], ev_.offlineClusters[detIdTopVec_[i]], tempStubs);
      ev_.offlineStubs.insert({detIdBottomVec_[i], tempStubs});
    }
  } else {
    //Cluster 1D
    edm::Handle< edmNew::DetSetVector<Phase2TrackerCluster1D> > sp_cluster1D_detset;
    iEvent.getByToken(sparsifiedClusterToken_, sp_cluster1D_detset);
    for(auto& det : *sp_cluster1D_detset) {
      std::vector<tbeam::cluster> tempclus;
      for(auto& hit : det) {
        tempclus.emplace_back( tbeam::cluster( hit.firstStrip(), hit.firstRow(),
                                               hit.edge(), hit.column(),
                                               hit.size() ) );
      }
      ev_.cbcClusters.insert({std::to_string(det.id()), tempclus});
    }
    //run the offline stub simulator
    for(unsigned int i = 0; i < detIdBottomVec_.size(); i++) {
      if(ev_.cbcClusters.find(detIdBottomVec_[i]) == ev_.cbcClusters.end() 
         || ev_.cbcClusters.find(detIdTopVec_[i]) == ev_.cbcClusters.end() )     continue;
      std::vector<tbeam::stub>  tempStubs;
      stubSimulator (ev_.cbcClusters[detIdBottomVec_[i]], ev_.cbcClusters[detIdTopVec_[i]], tempStubs);
      ev_.offlineStubs.insert({detIdBottomVec_[i], tempStubs});
    }
  }
  //get stub info
  edm::Handle< edmNew::DetSetVector<Phase2TrackerStub> > stub_detset;
  iEvent.getByToken(stubToken_, stub_detset);
  for(auto& det : *stub_detset) {
    std::vector<tbeam::stub> tmpstubs;
    for(auto& hit : det) {
      tmpstubs.emplace_back( tbeam::stub( hit.getPositionX(), hit.getPositionY(), hit.getBend() ) );
    }
    ev_.cbcStubs.insert( { std::to_string(det.id()), tmpstubs } );
  }
  if(verbosity_)  ev_.dumpEvent();
  tree_->Fill();
  ev_.reset();
}

//Function to create clusters from unsparisified digi collection.
void EdmToNtupleNoMask::offlineclusterizer(const std::vector<tbeam::hit>& hits, const unsigned int nCbc,
                                           const unsigned int nStripsPerCBC, std::vector<tbeam::cluster>& clusVec ){
  if (hits.empty())  return; 
  unsigned int fStrip = hits.at(0).strip();//get strip of first hit
  unsigned int ftRow  = hits.at(0).row();
  unsigned int ed     = hits.at(0).edge();
  unsigned int col    = hits.at(0).column();
  unsigned int size=1;
  unsigned int edge = 2*nStripsPerCBC;
  if (nCbc==16) edge = 8*nStripsPerCBC;
  for (unsigned int i = 1; i < hits.size(); i++){
    if (hits.at(i).strip() == fStrip + size && !(hits.at(i).strip() == edge)){
      size++;
    }
    else{
      tbeam::cluster clust(fStrip, ftRow, ed, col, size);
      clusVec.push_back(clust);
      size=1;
      fStrip = hits.at(i).strip();//get strip of first hit
      ftRow  = hits.at(i).row();
      ed     = hits.at(i).edge();
      col    = hits.at(i).column();
    }  
  }       
  tbeam::cluster clust(fStrip, ftRow, ed, col, size);
  clusVec.push_back(clust);
}

//offline stub simulator from tbeam::cluster
//Assumptions - Cluster Width cut off = 3
//Max Cluster Correlation Window = 7
void EdmToNtupleNoMask::stubSimulator (const std::vector<tbeam::cluster>& seeding, const std::vector<tbeam::cluster>& matching, std::vector<tbeam::stub>& stubVec){
  for(auto& sCls : seeding) {
    if(sCls.size() > 3)       continue;//cut cluster size
    for(auto& mCls :matching) {
      if(mCls.size() > 3)     continue;//cut cluster size
      if(std::abs(sCls.center() - mCls.center()) <= 7.) {
        stubVec.emplace_back( tbeam::stub(sCls.firstStrip(), sCls.column(), sCls.center() - mCls.center()) );
      }
    }
  }
}

void EdmToNtupleNoMask::endJob()
{
  //delete ev_;
}
//define this as a plug-in
DEFINE_FWK_MODULE(EdmToNtupleNoMask);
