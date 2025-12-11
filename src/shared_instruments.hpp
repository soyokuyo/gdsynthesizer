#ifndef SHARED_INSTRUMENTS_H
#define SHARED_INSTRUMENTS_H

#include <array>

#include "instrument.hpp"

class SharedInstruments {
public:
    // Access singleton instance
    static SharedInstruments& getInstance();

    // Get current instruments
    const std::array<Instrument, Sequencer::numinstruments>& getInstruments() const;

    // Replace instruments
    void setInstruments(const std::array<Instrument, Sequencer::numinstruments>& instruments);

private:
    SharedInstruments();
    SharedInstruments(const SharedInstruments&) = delete;
    SharedInstruments& operator=(const SharedInstruments&) = delete;
    SharedInstruments(SharedInstruments&&) = delete;
    SharedInstruments& operator=(SharedInstruments&&) = delete;
    std::array<Instrument, Sequencer::numinstruments> instruments_;
};

#endif

