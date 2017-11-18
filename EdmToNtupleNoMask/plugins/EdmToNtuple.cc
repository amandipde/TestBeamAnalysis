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
  isSparisifiedMode_(iConfig.getParameter<bool>("isSparsified"))
{

  std::vector<int> detId(iConfig.getParameter< std::vector<int> >("detIdVec"));
  std::vector<std::string> detNames(iConfig.getParameter< std::vector<std::string> >("detNamesVec"));
  if( detId.empty() || detId.size() != detNames.size() ) {
    std::cout << "detId information error!! Please check PSet" << std::endl;
    exit(EXIT_FAILURE);
  } 
  else {
    for( unsigned int i = 0; i<detId.size(); i++ ) {
      detIdNamemap_[detId[i]] = detNames[i];    
    }
  }
  if(verbosity_)
    for( auto& e: detIdNamemap_ )
      std::cout << e.first << "=" << e.second << std::endl;
   
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
    std::cout << " buffer size : " << buffer.bufferSize() << std::endl;
    std::cout << " fed id      : " << fedIndex << std::endl;
    printHeader(buffers);
    //Fill info in object
    Phase2TrackerHeaderDigi tr_header = Phase2TrackerHeaderDigi(buffer.trackerHeader());
    ev_.dataFormatVersion = tr_header.getDataFormatVersion();
    ev_.debugMode = tr_header.getDebugMode();
    ev_.readoutMode = tr_header.getReadoutMode();
    ev_.dataType = tr_header.getDataType();
    ev_.glibStatusCode = tr_header.getGlibStatusCode();
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
  }
  //IF unsparisified read channel data!!Otherwise read clusters
  if(!isSparisifiedMode_) {
    //Unspasified Data
    edm::Handle< edm::DetSetVector<Phase2TrackerDigi> > usp_digi_detset;
    iEvent.getByToken(unsparsifiedDigiToken_, usp_digi_detset);
    printDigi(usp_digi_detset);
    for(auto& det : *usp_digi_detset) {
      std::vector<tbeam::hit> temphits;
      for(auto& hit : det) {
        temphits.emplace_back(tbeam::hit(hit.row(), hit.column(), hit.strip(), hit.edge(),
                                         hit.channel(), hit.overThreshold()) );
      }
      ev_.dutHits.insert({std::to_string(det.id), temphits});
      std::vector<tbeam::cluster> tempclus;
      offlineclusterizer(temphits, ev_.numberOfCBC, 127, tempclus);
      ev_.offlineClusters.insert({std::to_string(det.id), tempclus});
      std::cout << "**********Offline Clustesr************" << std::endl;
      std::cout << "Detid=" << det.id << std::endl;
      for(auto& c : tempclus) {
        std::cout << "fStrip:" << std::setw(8)<< c.firstStrip
                  << " size:" << std::setw(8)<< c.size
                  << " center:" << std::setw(8)<< c.center()
        << std::endl;
      }
      std::cout << "**********Offline Clustesr************" << std::endl;
    }
  } else {
    //Cluster 1D
    edm::Handle< edmNew::DetSetVector<Phase2TrackerCluster1D> > sp_cluster1D_detset;
    iEvent.getByToken(sparsifiedClusterToken_, sp_cluster1D_detset);
    for(auto& det : *sp_cluster1D_detset) {
      std::cout << "DetId:" << det.id() << std::endl;
      std::vector<tbeam::cluster> tempclus;
      for(auto& hit : det) {
        tempclus.emplace_back( tbeam::cluster( hit.firstStrip(), hit.firstRow(),
                                               hit.edge(), hit.column(),
                                               hit.size() ) );
      }
      ev_.cbcClusters.insert({std::to_string(det.id()), tempclus});
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
  tree_->Fill();
  ev_.reset();
}

void EdmToNtupleNoMask::printHeader(const edm::Handle<FEDRawDataCollection>& buffers) {
  for(size_t  fedIndex = Phase2Tracker::FED_ID_MIN; fedIndex < Phase2Tracker::CMS_FED_ID_MAX; ++fedIndex ) {
    const FEDRawData& fed = buffers->FEDData(fedIndex);
    if(fed.size()==0) continue;
    // construct buffer
    Phase2Tracker::Phase2TrackerFEDBuffer buffer(fed.data(),fed.size());
    std::cout << " buffer size : " << buffer.bufferSize() << std::endl;
    std::cout << " fed id      : " << fedIndex << std::endl;
    //Read Tracker Header Data
    std::cout << "******Tracker Header information Start*****" << std::endl;
    Phase2TrackerHeaderDigi tr_header = Phase2TrackerHeaderDigi(buffer.trackerHeader());
    std::cout << " Version  : " << std::hex << std::setw(2) << (int) tr_header.getDataFormatVersion() << std::endl;
    std::cout << " Debug Mode     : " << std::hex << std::setw(2) << (int)tr_header.getDebugMode() << std::endl;
    std::cout << " Readout Mode : " << std::hex << std::setw(2) << (int)tr_header.getReadoutMode() << std::endl;
    std::cout << " Data Type     : " << std::hex << std::setw(2) << (int) tr_header.getDataType() << std::endl;
    std::cout << " Condition Data : " << std::hex << std::bitset<8>(tr_header.getConditionData()) << "\n";
    std::cout << " Data Type  : " << ( tr_header.getDataType() ? "Real" : "Fake" ) << "\n";
    std::cout << " Glib Status   : " << std::hex << std::setw(2) << (int) tr_header.getGlibStatusCode() << std::endl;
    std::cout << " No. of CBC   : " << std::hex << std::setw(2) << (int) tr_header.getNumberOfCBC() << std::endl;
    std::cout << "******Tracker Header information End*******" << std::endl;

    //print condition data
    std::cout << "******Tracker Condition Data information Start*****" << std::endl;
    std::map<uint32_t,uint32_t> conddata_map = buffer.conditionData();
    for(auto& it : conddata_map){ 
      uint8_t uid     = (it.first >> 24)  & MASK_BITS_8;
      uint8_t i2cReg  = (it.first >> 16)  & MASK_BITS_8;
      uint8_t i2cPage = (it.first >> 12)  & MASK_BITS_4;
      uint8_t roId    = (it.first >> 8)   & MASK_BITS_4;
      uint8_t feId    = (it.first)        & MASK_BITS_8;
      std::cout <<  std::hex << "key: "      << it.first
                <<  " uid: "     << std::setw(8) << (int)uid  
                <<  " i2cReg: "  << std::setw(8) << (int)i2cReg  
                <<  " i2cPage: " << std::setw(8) << (int)i2cPage  
                <<  " roId: "    << std::setw(8) << (int)roId 
                <<  " feId: "    << std::setw(8) << (int)feId  
                << std::hex << " value: "   << it.second << " (hex) "
                << std::dec                 << it.second << " (dec) " << std::endl;
    } 
    std::cout << "******Tracker Condition Data information End*******" << std::endl;
  }
}
void EdmToNtupleNoMask::printDigi(const edm::Handle< edm::DetSetVector<Phase2TrackerDigi> >& usp_digi_detset){
    //if(usp_digi_detset)  std::cout << "Unsparsified digi collection present!!" << std::endl;
    std::cout << "******Tracker Unsparsified Data Start*******" << std::endl;
    for(auto& det : *usp_digi_detset) {
      std::cout << "DetId:" << det.id << std::endl;
      for(auto& hit : det) {
	std::cout << "channel=" << std::setw(4) << hit.channel() 
		  << "  row=" << std::setw(4) << hit.row() 
		  << "  column=" << std::setw(4) << hit.column() 
		  << "  strip=" << std::setw(4) << hit.strip() 
		  << "  edge=" << std::setw(4) << hit.edge()
		  << "  overThreshold=" << std::setw(2) << hit.overThreshold() 
		  << std::endl;
      }
    }
    std::cout << "******Tracker Unsparsified Data End*********" << std::endl;
}
void EdmToNtupleNoMask::printClus1D(const edm::Handle< edmNew::DetSetVector<Phase2TrackerCluster1D> >& sp_cluster1D_detset) {
    //if(sp_cluster1D_detset)  std::cout << "Cluster 1D collection present!!" << std::endl;
    std::cout << "******Tracker Cluster1D Data Start*******" << std::endl;
    for(auto& det : *sp_cluster1D_detset) {
      std::cout << "DetId:" << det.id() << std::endl;
      for(auto& hit : det) {
	std::cout << "FStrip="  << std::setw(4) << hit.firstStrip() 
		  << " FRow="   << std::setw(4) << hit.firstRow() 
		  << " Edge="   << std::setw(4) << hit.edge() 
		  << " Column=" << std::setw(4) << hit.column() 
		  << " Size="   << std::setw(6) << (int) hit.size() 
		  << " Threshold=" << std::setw(6) << (int) hit.threshold() 
		  << " Center="    << std::setw(4) << hit.center() 
		  << " Barycenter(" << std::setw(4) << hit.barycenter().first << "," << hit.barycenter().second << ")" 
		  << std::endl; 
      }
    }
    std::cout << "******Tracker Cluster1D Data End*********" << std::endl;
}
void EdmToNtupleNoMask::printStub(const edm::Handle< edmNew::DetSetVector<Phase2TrackerStub> >& stub_detset){
  std::cout << "******Tracker Stub Data Start*******" << std::endl;
  for(auto& det : *stub_detset) {
    std::cout << "DetId:" << det.id() << std::endl;
    for(auto& hit : det) {
      std::cout << "PosX="   << std::setw(8) << (int) hit.getPositionX()
                << " PosY="   << std::setw(8) << (int) hit.getPositionY() 
                << " Bend="   << std::setw(6) <<  hit.getBend() 
                << std::endl;
    }
  }
  std::cout << "******Tracker Stub Data End*********" << std::endl;
}

void EdmToNtupleNoMask::offlineclusterizer(const std::vector<tbeam::hit>& hits, const unsigned int nCbc,
                                           const unsigned int nStripsPerCBC, std::vector<tbeam::cluster>& clusVec ){
  if (hits.empty())  return; 
  unsigned int fStrip = hits.at(0).strip;//get strip of first hit
  unsigned int ftRow  = hits.at(0).row;
  unsigned int ed     = hits.at(0).edge;
  unsigned int col    = hits.at(0).column;
  unsigned int size=1;
  unsigned int edge = -1;
  if (nCbc==16) edge = 8*nStripsPerCBC;
  for (unsigned int i = 1; i < hits.size(); i++){
    if (hits.at(i).strip == fStrip + size && !(hits.at(i).strip == edge)){
      size++;
    }
    else{
      tbeam::cluster clust(fStrip, ftRow, ed, col, size);
      clusVec.push_back(clust);
      size=1;
      fStrip = hits.at(i).strip;//get strip of first hit
      ftRow  = hits.at(i).row;
      ed     = hits.at(i).edge;
      col    = hits.at(i).column;
    }  
  }       
  tbeam::cluster clust(fStrip, ftRow, ed, col, size);
  clusVec.push_back(clust);
}
void EdmToNtupleNoMask::endJob()
{
  //delete ev_;
}
//define this as a plug-in
DEFINE_FWK_MODULE(EdmToNtupleNoMask);
