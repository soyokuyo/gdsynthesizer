#ifndef SHARED_INSTRUMENTS_H
#define SHARED_INSTRUMENTS_H

#include <array>

#include "instrument.hpp"

class SharedInstruments {
public:
    // シングルトンインスタンスへのアクセス
    static SharedInstruments& getInstance();

    // 現在の楽器定義を取得
    const std::array<Instrument, Sequencer::numinstruments>& getInstruments() const;

    // 楽器定義を差し替え
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

