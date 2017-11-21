#ifndef __Stub__h
#define __Stub__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
namespace tbeam {
 class stub : public TObject {
  public:
  stub():posx_(0),posy_(0),bend_(0.){}
  stub(const uint32_t posx, const uint8_t posy, const float bend):posx_(posx),posy_(posy),bend_(bend) {}
    virtual ~stub() {}
    uint32_t positionX() const { return posx_; }
    uint8_t positionY() const  { return posy_; }
    float bend() const         { return bend_; }
    friend std::ostream &operator<<(std::ostream& os, const tbeam::stub& s) {
      os << "PosX= "  << std::setw(4) << s.positionX()
         << " PosY= " << std::setw(4) << (int)s.positionY()
         << " Bend= " << std::setw(4) << s.bend()
         << std::endl;
      return os;
    }
  private:
    uint32_t posx_;
    uint8_t posy_;
    float bend_;
    ClassDef(stub,1)
      };
}
#endif
