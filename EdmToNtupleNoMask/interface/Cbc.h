#ifndef __Cbc__h
#define __Cbc__h
#include<stdint.h>
#include<iostream>
#include<iomanip>
namespace tbeam {
  class cbc : public TObject {
  public:
  cbc() :  chipdebugstatus_(99), error_(99), pipelineAdd_(99), l1Id_(99) {}
  cbc(const uint32_t cdebugstatus, const uint16_t err, const uint16_t pAdd, const uint16_t  l1):
    chipdebugstatus_(99), error_(99), pipelineAdd_(99), l1Id_(99) {}
    virtual ~cbc(){}
    uint32_t chipdebugstatus() const { return chipdebugstatus_; }
    uint16_t error() const           { return error_; }
    uint16_t pipelineAddress() const { return pipelineAdd_; }
    uint16_t L1Id() const            { return l1Id_; }
    friend std::ostream &operator<<(std::ostream& os, const tbeam::cbc& chip) {    
      os << " Full chip debug" << std::hex << std::setw(8) << chip.chipdebugstatus() 
         << "  Chip Error"     << std::hex << std::setw(4) << chip.error()
         << "  Chip L1ID "     << int(chip.L1Id()) 
         << "  Chip PA   "     << int(chip.pipelineAddress()) 
         << std::endl;
      return os;
    }
  private:
    uint32_t chipdebugstatus_;
    uint16_t error_;
    uint16_t pipelineAdd_;
    uint16_t l1Id_;
    ClassDef(cbc,1)
      };
}
#endif
