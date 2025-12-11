#include "shared_instruments.hpp"

SharedInstruments& SharedInstruments::getInstance() {
    static SharedInstruments instance;
    return instance;
}

SharedInstruments::SharedInstruments()
    : instruments_(defaultInstruments) {}

const std::array<Instrument, Sequencer::numinstruments>& SharedInstruments::getInstruments() const {
    return instruments_;
}

void SharedInstruments::setInstruments(const std::array<Instrument, Sequencer::numinstruments>& instruments) {
    instruments_ = instruments;
}

