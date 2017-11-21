#ifndef __Cluster__h
#define __Cluster__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
namespace tbeam {
class cluster : public TObject {
 public:
  cluster() : firstStrip_(9999), firstRow_(9999), edge_(9999), column_(999), size_(999), threshold_(0) {}
    //cluster(unsigned int fStrip, unsigned int ftRow, unsigned int ed, unsigned int col, uint16_t s) :
    //  firstStrip(fStrip), firstRow(ftRow), edge(ed), column(col), size(s), threshold(0) {}
    
  cluster(unsigned int fStrip, unsigned int ftRow, unsigned int ed, unsigned int col, uint16_t s, uint16_t th = 0) :
    firstStrip_(fStrip), firstRow_(ftRow), edge_(ed), column_(col), size_(s), threshold_(th) {}
    virtual ~cluster(){}
    float center() const { return float(firstStrip()) + 0.5*(float(size()) - 1.); }
    std::pair< float, float > barycenter() const { return std::make_pair(column(), center()); }
    unsigned int firstStrip() const { return firstStrip_; }
    unsigned int firstRow() const   { return firstRow_; }
    unsigned int edge() const       { return edge_; }
    unsigned int column() const     { return column_; }
    uint16_t size() const           { return size_; }
    uint16_t threshold() const      { return threshold_; }
    
    friend std::ostream &operator<<(std::ostream& os, const tbeam::cluster& cls) {    
      os << "FStrip="  << std::setw(4) << cls.firstStrip() 
	 << " FRow="   << std::setw(4) << cls.firstRow() 
	 << " Edge="   << std::setw(4) << cls.edge() 
	 << " Column=" << std::setw(4) << cls.column() 
	 << " Size="   << std::setw(6) <<  cls.size() 
	 << " Threshold=" << std::setw(6) << cls.threshold() 
	 << " Center="    << std::setw(4) << cls.center() 
	 << " Barycenter(" << std::setw(4) << cls.barycenter().first << "," << cls.barycenter().second << ")" 
	 << std::endl; 
      return os;
    }
    
  private:
    unsigned int firstStrip_;
    unsigned int firstRow_;
    unsigned int edge_;
    unsigned int column_;
    uint16_t size_;
    uint16_t threshold_;
    ClassDef(cluster,1)
      };
}
#endif
