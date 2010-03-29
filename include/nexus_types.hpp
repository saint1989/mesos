#ifndef __NEXUS_TYPES_HPP__
#define __NEXUS_TYPES_HPP__

#include "nexus.h"
#include <cstdlib>
#include <sstream>
#include <string>

namespace nexus {

typedef framework_id FrameworkID; // Unique within master
typedef task_id TaskID;           // Unique within framework
typedef std::string SlaveID;         // Unique within master
typedef offer_id OfferID;         // Unique within master
typedef task_state TaskState;

}

#endif /* __NEXUS_TYPES_HPP__ */
