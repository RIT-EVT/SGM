#include <SGM.hpp>

namespace IO = EVT::core::IO;

namespace SGM {

SGM::SGM(DEV::StrainGauge gauges[NUM_GAUGES]) : gauges{gauges[0], gauges[1], gauges[2], gauges[3]} {}

CO_OBJ_T* SGM::getObjectDictionary() {
    return objectDictionary;
}

uint8_t SGM::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

// Update with proper Node ID given by integration!
uint8_t SGM::getNodeID() {
    return NODE_ID;
}

void SGM::process() {
    for (int i = 0; i < NUM_GAUGES; i++) {
        force[i] = gauges[i].getForce();
    }
}

}//namespace SGM
