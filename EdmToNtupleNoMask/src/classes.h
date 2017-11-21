#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Event.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Hit.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Cbc.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Cluster.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Stub.h"
#include "TestBeamAnalysis/EdmToNtupleNoMask/interface/Track.h"

#include <vector>
namespace {
struct dictionary {

 tbeam::Event rv1;
 std::vector<tbeam::Event> vrv1;
 std::map<std::string, std::vector< tbeam::cluster *> > msvpfi;
 tbeam::hit  h;
 std::vector<tbeam::hit>  vh;
 std::map<std::string,std::vector<tbeam::hit>>  mvh;

 tbeam::cluster  cp;
 tbeam::cluster *cpp;
 tbeam::stub s;
 tbeam::cbc c;
 std::vector< tbeam::stub > vs;
 std::vector< tbeam::cbc > vc;
 std::vector< tbeam::cluster> vcl;
 std::vector< tbeam::cluster*> vclp;
 std::vector<int> vrvi;
 std::vector<tbeam::stub *> vps;
 std::vector<unsigned short> vrvs;
 std::map< std::string,std::vector<int> >  msvi;
 std::map< std::string,std::vector<unsigned short> > msvs;
 std::map<std::string, std::vector<tbeam::cbc> > mcbc;
 tbeam::Track tk;
 std::vector<tbeam::Track>  vtk;
};
}
