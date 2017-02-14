#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/AnalysisObjects.h"
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
  stubWord(999999),
  stubLatency(999999),
  triggerLatency(999999)
{
   cbcs=std::vector<tbeam::cbc>();
}


tbeam::cbc::cbc():
   pipelineAdd(0),
   status(0),
   error(0)
{
}

tbeam::cluster::cluster():
   x(0),
   fx(-99.),
   size(0)
{
  // stubs=std::vector<tbeam::stub*>();
}

tbeam::cluster::~cluster(){
}

tbeam::stub::stub():
   x(0),
   fx(-99.),
   direction(0)
{
   seeding=0;
   matched=0;
}
tbeam::stub::stub(const tbeam::stub& t) 
{
  seeding = new tbeam::cluster(*(t.seeding));
  matched = new tbeam::cluster(*(t.matched));
  x = t.x;
  fx = t.fx;
  direction = t.direction;
}

tbeam::dutEvent::dutEvent():
   stubWord(0),
   stubWordReco(0)
{
//isGood(1)
   stubs=std::vector<tbeam::stub*>();
   fstubs=std::vector<tbeam::stub*>();
}
tbeam::dutEvent::dutEvent(const tbeam::dutEvent& t) 
{
  //std::map < std::string, std::vector <tbeam::cluster*> > clusters;
  for(auto& d : t.clusters ) {
    std::vector<tbeam::cluster*> ctemp;
    for(auto cp : d.second) {
      tbeam::cluster* cc = new tbeam::cluster(*cp);
      ctemp.push_back(cc);
    }
    clusters[d.first] = ctemp;
  }
  //std::map< std::string,std::vector<int> > dut_channel;
  dut_channel = t.dut_channel;
  //std::map< std::string,std::vector<int> > dut_row;
  dut_row = t.dut_row;
  //std::vector <tbeam::stub*> stubs;
  for(auto& sp : t.stubs) {
    tbeam::stub* ss = new tbeam::stub(*sp);
    stubs.push_back(ss);
  }
  for(auto& sp : t.fstubs) {
    tbeam::stub* ss = new tbeam::stub(*sp);
    fstubs.push_back(ss);
  }
  stubWord = t.stubWord;
  stubWordReco = t.stubWordReco;
}


tbeam::dutEvent::~dutEvent(){
   //std::cout << "Entering dutEvent destructor!" << std::endl;   
   /*
   for (unsigned int i=0; i<stubs.size(); i++){ if(stubs.at(i))   delete stubs.at(i);}
   for(std::map<std::string,std::vector<tbeam::cluster *> >::iterator it = clusters.begin(); it != clusters.end(); ++it) {
      for(std::vector<tbeam::cluster *>::iterator cl = it->second.begin(); cl!=it->second.end(); ++cl){
         if(*cl)  delete *cl;
      }
   }*/
   //std::cout << "Leaving dutEvent destructor!" << std::endl;   
}

tbeam::condEvent::condEvent() :
   run(999999), 
   lumiSection(999999), 
   event(999999),  
   time(999999), 
   unixtime(999999), 
   tdcPhase(999999),
   HVsettings(999999),
   DUTangle(999999),
   window(999999),
   offset(999999),
   cwd(999999),
   tilt(999999),
   vcth(999999),
   stubLatency(999999),
   triggerLatency(999999),
   condData(999),
   glibStatus(9999)
{
  cbcs = std::vector<tbeam::cbc>();
}


