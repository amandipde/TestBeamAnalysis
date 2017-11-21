#ifndef __Hit__h
#define __Hit__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
namespace tbeam {
  class hit : public TObject {
  public:
  hit(): row_(0), column_(0), strip_(0), edge_(0), channel_(0), overThreshold_(0) {}
    hit(const unsigned int r, const unsigned int c, const unsigned int s, const unsigned int e, const unsigned int ch, const bool oTh) {
      row_ = r;
      column_ = c;
      strip_ = s;
      edge_ = e;
      channel_ = ch;
      overThreshold_ = oTh;
    }
    virtual ~hit(){}
    
    unsigned int row()     const { return row_; }
    unsigned int column()  const { return column_; }
    // Access to digi information - strip sensors
    unsigned int strip()   const { return strip_; }//same as row* 
    unsigned int edge()    const { return edge_; } //same as col *
    // Access to the (raw) channel number
    unsigned int channel() const { return channel_; }
    // Access Overthreshold bit
    bool overThreshold()  const { return overThreshold_; }
    
    friend std::ostream &operator<<(std::ostream& os, const tbeam::hit& h) {
      os << " Row=" << std::setw(6)  <<  h.row()
	 << " Col=" << std::setw(4)  << h.column()
	 << " Strip=" << std::setw(6) << h.strip()
	 << " Edge=" <<  std::setw(4)    << h.edge()
	 << " Channel=" <<  std::setw(6)  << h.channel()
	 << " OverTh=" <<  std::setw(2)  << h.overThreshold()
	 << std::endl;
      return os;
    }
  private:
    unsigned int row_;
    unsigned int column_;
    unsigned int strip_;
    unsigned int edge_;
    unsigned int channel_;
    bool overThreshold_;
    ClassDef(hit,1)
      };
}
#endif
