#ifndef __Cbc__h
#define __Cbc__h
#include<stdint.h>
namespace tbeam {
  class cbc : public TObject {
  public:
    cbc() : pipelineAdd(999), status(99), error(99) {}
    cbc(const uint16_t pAdd, const uint8_t stat, const uint8_t err) :
      pipelineAdd(pAdd), 
      status(stat), 
      error(err)
    {}
    virtual ~cbc(){}
    uint16_t pipelineAdd;
    uint8_t status;
    uint8_t error;
    ClassDef(cbc,1)
      };
}
#endif
