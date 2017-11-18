#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Event.h" 
#include<iostream>

tbeam::Event::Event():
  run(999999), 
  lumiSection(999999), 
  event(999999),  
  time(999999), 
  unixtime(999999), 
  dt(999999),
  tdcPhase(999999),
  HVsettings(999999),
  DUTangle(999999),
  stubLatency(999999),
  triggerLatency(999999)
{
}

void tbeam::Event::reset(){
  run = 999999; 
  lumiSection = 999999; 
  event = 999999;
  time = 999999;
  unixtime = 999999;
  dt = 999999;
  tdcPhase = 999999;
  HVsettings = 999999;
  DUTangle = 999999;
  stubLatency = 999999; 
  triggerLatency = 999999;
  //clear the maps
  cbcs.clear();
  dutHits.clear();
  cbcClusters.clear();
  cbcStubs.clear();
}
