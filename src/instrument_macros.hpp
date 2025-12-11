#ifndef INSTRUMENT_MACROS_H
#define INSTRUMENT_MACROS_H

// BaseWave shorthand
#define BW_SIN         BaseWave::WAVE_SIN
#define BW_SQUARE      BaseWave::WAVE_SQUARE
#define BW_TRIANGLE    BaseWave::WAVE_TRIANGLE
#define BW_SAWTOOTH    BaseWave::WAVE_SAWTOOTH
#define BW_SINSAWx2    BaseWave::WAVE_SINSAWx2 // in base wave OC, it's used as doubled sawtooth on sin wave.
#define BW_SAWT_INV    BaseWave::WAVE_SINSAWx2 // in LFO, it's used as inverted sawtooth wave.

// Noise distribution shorthand
#define NDT_FLAT       NoiseDistributType::NOISEDTYPE_FLAT
#define NDT_TRIANGULAR NoiseDistributType::NOISEDTYPE_TRIANGULAR
#define NDT_COS4ThPOW  NoiseDistributType::NOISEDTYPE_COS4ThPOW

// Noise color shorthand
#define NCT_WHITE      NoiseColorType::NOISECTYPE_WHITE
#define NCT_PINK       NoiseColorType::NOISECTYPE_PINK

#endif

