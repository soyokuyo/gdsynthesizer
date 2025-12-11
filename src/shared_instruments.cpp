#include "shared_instruments.hpp"

SharedInstruments* SharedInstruments::instance_ = nullptr;

SharedInstruments& SharedInstruments::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new SharedInstruments();
    }
    return *instance_;
}

SharedInstruments::SharedInstruments()
    : instruments_(defaultInstruments) {}

const std::array<Instrument, Sequencer::numinstruments>& SharedInstruments::getInstruments() const {
    return instruments_;
}

void SharedInstruments::setInstruments(const std::array<Instrument, Sequencer::numinstruments>& instruments) {
    instruments_ = instruments;
}

