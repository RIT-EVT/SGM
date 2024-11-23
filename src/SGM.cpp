#include <SGM.hpp>

namespace IO = EVT::core::IO;

namespace SGM {

SGM::SGM(DEV::StrainGauge* gauges) : gauges{gauges} {}

CO_OBJ_T* SGM::getObjectDictionary() {
    return objectDictionary;
}

uint8_t SGM::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t SGM::getNodeID() {
    return NODE_ID;
}

void SGM::process() {
    for (int i = 0; i < NUM_GAUGES; i++) {
        force[i] = gauges[i].getForce();
    }
}

}//namespace SGM