#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/AnalysisObjects.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Phase2TrackerDigi/interface/Phase2TrackerCommissioningDigi.h"

#include <vector>
namespace {
struct dictionary {

 tbeam::Event rv1;
 std::vector<tbeam::Event> vrv1;
 std::map<std::string, std::vector< tbeam::cluster *> > msvpfi;
 tbeam::cluster  cp;
 tbeam::cluster *cpp;
 tbeam::stub s;
 tbeam::cbc c;
 tbeam::dutEvent  dE;
 tbeam::condEvent cE;
 std::vector< tbeam::stub > vs;
 std::vector< tbeam::cbc > vc;
 std::vector< tbeam::cluster> vcl;
 std::vector< tbeam::cluster*> vclp;
 std::vector<int> vrvi;
 std::vector<tbeam::stub *> vps;
 std::vector<unsigned short> vrvs;
 std::map< std::string,std::vector<int> >  msvi;
 std::map< std::string,std::vector<unsigned short> > msvs;
 std::vector<std::vector<Phase2TrackerCommissioningDigi> > mdigi;
 edm::Wrapper<std::vector<std::vector<Phase2TrackerCommissioningDigi> > > vdigi;
};
}
