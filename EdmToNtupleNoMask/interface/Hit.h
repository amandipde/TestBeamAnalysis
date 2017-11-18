#ifndef __Hit__h
#define __Hit__h
#include<stdint.h>
namespace tbeam {
  class hit : public TObject {
  public:
  hit():
    row(0),
      column(0),
      strip(0),
      edge(0),
      channel(0),
      overThreshold(0)
	{
	}
    hit(const unsigned int r, const unsigned int c,
	const unsigned int s, const unsigned int e,
	const unsigned int ch, const bool oTh) 
      {
	row = r;
	column = c;
	strip = s;
	edge = e;
	channel = ch;
	overThreshold = oTh;
      }
    
    virtual ~hit(){}
    unsigned int row;
    unsigned int column;
    unsigned int strip;
    unsigned int edge;
    unsigned int channel;
    bool overThreshold;
    ClassDef(hit,1)
      };
}
#endif
