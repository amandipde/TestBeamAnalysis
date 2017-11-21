#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Event.h" 
#include<iostream>

static constexpr unsigned int MASK_BITS_8  = 0xFF;
static constexpr unsigned int MASK_BITS_4 = 0xF;

tbeam::Event::Event():
  run(999999), 
  lumiSection(999999), 
  event(999999),  
  time(999999), 
  unixtime(999999), 
  dt(999999),
  isSparisified(false),
  tdcPhase(999999),
  HVsettings(999999),
  DUTangle(999999),
  stubLatency(999999),
  triggerLatency(999999)
{
}

void tbeam::Event::reset(){
  run = 0;//
  lumiSection = 0;//
  event = 0;//
  time = 0;//
  unixtime = 0;//
  dt = 0;//is it used?
  isSparisified = 0;//
  
  dataFormatVersion = 0;//
  condData = 0;//
  debugMode = 0;//
  readoutMode = 0;//
  dataType = 0;//
  glibStatusCode = 0;//
  numberOfCBC = 0;//
  //condition data
  tdcPhase = 0;//
  HVsettings = 0;//
  DUTangle = 0;//
  vcth = 0;//
  
  window = 0;
  offset = 0;
  cwd = 0;
  tilt = 0;
  stubLatency = 0;
  triggerLatency = 0;
  
  //clear the maps
  conddatamap.clear();//
  cbcs.clear();//
  dutHits.clear();//
  cbcClusters.clear();//only in sparsified mode
  cbcStubs.clear();//
  offlineClusters.clear();//only in unsparsified mode
  offlineStubs.clear();
}

void tbeam::Event::dumpEvent(std::ostream& os) {
  os << "Run:"   <<  run
     << " Lumi:" <<  lumiSection
     << " Event:" << event
     << std::endl;
  os << "******Tracker Header information Start*****" << std::endl;
  os << " Version  : " << std::hex << std::setw(2) << (int)dataFormatVersion << std::endl;
  os << " Debug Mode     : " << std::hex << std::setw(2) << (int)debugMode << std::endl;
  os << " Readout Mode : " << std::hex << std::setw(2) << (int)readoutMode << std::endl;
  os << " Data Type     : " << std::hex << std::setw(2) << (int)dataType << std::endl;
  os << " Condition Data : " << std::hex << (condData) << std::endl;;
  os << " Data Type  : " << ( dataType ? "Real" : "Fake" ) << "\n";
  os << " Glib Status   : " << std::hex << std::setw(2) << (int)glibStatusCode << std::endl;
  os << " No. of CBC   : " << std::hex << std::setw(2) << (int)numberOfCBC << std::endl;
  os << "******Tracker Header information End*******" << std::endl;
  os << "******Tracker Condition Data information Start*****" << std::endl;
  for(auto& it : conddatamap){ 
    uint8_t uid     = (it.first >> 24)  & MASK_BITS_8;
    uint8_t i2cReg  = (it.first >> 16)  & MASK_BITS_8;
    uint8_t i2cPage = (it.first >> 12)  & MASK_BITS_4;
    uint8_t roId    = (it.first >> 8)   & MASK_BITS_4;
    uint8_t feId    = (it.first)        & MASK_BITS_8;
    os <<  std::hex << "key: "      << it.first
       <<  " uid: "     << std::setw(8) << (int)uid  
       <<  " i2cReg: "  << std::setw(8) << (int)i2cReg  
       <<  " i2cPage: " << std::setw(8) << (int)i2cPage  
       <<  " roId: "    << std::setw(8) << (int)roId 
       <<  " feId: "    << std::setw(8) << (int)feId  
       << std::hex << " value: "   << it.second << " (hex) "
       << std::dec                 << it.second << " (dec) " << std::endl;
  } 
  os << "Vcth:" << (int)vcth << "\tHV:" << HVsettings << "\tTDC:" << tdcPhase << "\tAngle:" << DUTangle;
  os << "******Tracker Condition Data information End*******" << std::endl;
  os << "******Tracker CBC staus Start*******" << std::endl;
  for(auto& c : cbcs) {
    os << "FeId:" << c.first	 << std::endl;
    for(auto& s : c.second) os << s;
  }
  os << "******Tracker CBC staus End*******" << std::endl;
  
  os << "******Tracker Hit Data Start(unsparsified mode)*******" << std::endl;
  for(auto& detH : dutHits) {
    os << "DetId:" << detH.first << std::endl;
    for(auto& hit : detH.second)   os << std::dec << hit; 
    }
  os << "******Tracker Hit Data End(unsparsified mode)*******" << std::endl;
  os << "******Tracker CBC Cluster1D Data Start(sparisified mode)*******" << std::endl;
  for(auto& detC : cbcClusters) {
    os << "DetId:" << detC.first << std::endl;
    for(auto& cls : detC.second) os << std::dec << cls;
  }
  os << "******Tracker CBC Cluster1D Data End(sparisified mode)*********" << std::endl;
  os << "******Tracker Offline Cluster1D Data Start(unsparisified mode)*******" << std::endl;
  for(auto& detC : offlineClusters) {
    os << "DetId:" << detC.first << std::endl;
    for(auto& cls : detC.second) os << std::dec << cls;
    }
  os << "******Tracker Offline Cluster1D Data End(unsparisified mode)*********" << std::endl;
  
  os << "******Tracker CBC Stub Data Start*******" << std::endl;
  for(auto& detS : cbcStubs) {
    os << "DetId:" << detS.first	 << std::endl;
    for(auto& stub : detS.second) os << std::dec << stub;
  }
  os << "******Tracker CBC Stub Data End*********" << std::endl;
  
  os << "******Tracker offline Stub Data Start*******" << std::endl;
  for(auto& detS : offlineStubs) {
    os<< "DetId:" << detS.first	 << std::endl;
    for(auto& stub : detS.second) os << std::dec << stub;
  }
  os << "******Tracker CBC offline Data End*********" << std::endl;

  os << "******Track Data(Should be Empty if DUT only Ntuple!!!)*******" << std::endl;
  for(auto& tk : tracks) {
    os << tk;
  }
  os << "******Track Data(Should be Empty if DUT only Ntuple!!!)*******" << std::endl;
}
