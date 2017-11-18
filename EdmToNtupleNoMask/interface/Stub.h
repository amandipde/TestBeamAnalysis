#ifndef __Stub__h
#define __Stub__h
#include<stdint.h>
namespace tbeam {
 class stub : public TObject {
  public:
  stub():posx_(0),posy_(0),bend_(0.){}
  stub(const uint32_t posx, const uint8_t posy, const float bend):posx_(posx),posy_(posy),bend_(bend) {}
    ~stub() {}
    uint32_t posx_;
    uint8_t posy_;
    float bend_;
    ClassDef(stub,1)
      };
}
#endif
