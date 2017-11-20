#ifndef __Cbc__h
#define __Cbc__h
#include<stdint.h>
namespace tbeam {
  class cbc : public TObject {
  public:
    cbc() :  chipdebugstatus(99), error(99), pipelineAdd(99), l1ID(99) {}
    cbc(const uint32_t cdebugstatus, const uint16_t err, const uint16_t pAdd, const uint16_t  l1):
      chipdebugstatus(99), error(99), pipelineAdd(99), l1ID(99) {}
    virtual ~cbc(){}
    uint32_t chipdebugstatus;
    uint16_t error;
    uint16_t pipelineAdd;
    uint16_t  l1ID;
    ClassDef(cbc,1)
      };
}
#endif
