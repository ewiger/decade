#ifndef __JSON_EMITTER_H_
#define __JSON_EMITTER_H_

#include "m_api_helper.h"
#include "m_parser.hpp"

class JsonEmitter: public virtual MEmitter {
  public:
    virtual void emitNull();
};

#endif //__JSON_EMITTER_H