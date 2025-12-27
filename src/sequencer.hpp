/**************************************************************************/
/*  sequencer.hpp                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GDSynthesizer                              */
/**************************************************************************/
/* Copyright (c) 2023-2024 Soyo Kuyo.                                     */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <cmath>
#include "smfparser.hpp"
#include <vector>
#include <array>
#include <functional>
#include <godot_cpp/classes/image.hpp>

#define PI (float)Math_PI
#define FLOAT_LONGTIME 36000000.0f

enum class BaseWave {
    WAVE_SIN,         //  0
    WAVE_SQUARE,      //  1
    WAVE_TRIANGLE,    //  2
    WAVE_SAWTOOTH,    //  3
    WAVE_SINSAWx2,    //  4

    WAVE_TAIL
};


enum class NoiseDistributType {
    NOISEDTYPE_FLAT,       //  0
    NOISEDTYPE_TRIANGULAR, //  1
    NOISEDTYPE_COS4ThPOW,  //  2

    NOISEDTYPE_TAIL
};

enum class NoiseColorType {
    NOISECTYPE_WHITE,   //  0
    NOISECTYPE_PINK,    //  1

    NOISECTYPE_TAIL
};

struct Instrument{
    float totalGain;
    
    float atackSlopeTime;
    float decayHalfLifeTime;
    float sustainRate;
    float releaseSlopeTime;

    float baseVsOthersRatio;
    float side1VsSide2Ratio;

    float baseOffsetCent1;
    BaseWave baseWave1;
    float baseOffsetCent2;
    BaseWave baseWave2;
    float baseOffsetCent3;
    BaseWave baseWave3;

    float noiseRatio;
    NoiseColorType noiseColorType;
    
    float delay0Time;
    float delay1Time;
    float delay2Time;
    float delay0Ratio;
    float delay1Ratio;
    float delay2Ratio;
    
    float freqNoiseCentRange;
    NoiseDistributType freqNoiseType;
    
    float fmCentRange;
    float fmFreq;
    float fmPhaseOffset;
    int32_t fmSync;
    BaseWave fmWave;

    float amLevel;
    float amFreq;
    float amPhaseOffset;
    int32_t amSync;
    BaseWave amWave;
};

struct Percussion{
    int32_t program;
    int32_t key;
};

class PinkNoise {
private:
    static constexpr int32_t tapNum  = 16;
    float z[tapNum];
    float k[tapNum];
    float t = 0.0;
public:
    PinkNoise();
    ~PinkNoise();
    float makeNoise (float);    
};

// Shared LUT manager for all Sequencer instances (Singleton pattern)
class SharedLUT {
private:
    static constexpr int32_t waveLUTSize = 32768;
    static constexpr int32_t pow2_x_1200LUT_size = 7200;
    static constexpr int32_t lowFrequencyCorrectionLUT_size = 4096;
    
    // Singleton instance
    static SharedLUT* instance;
    static int32_t refCount;
    
    // Wave LUT (shared across all instances)
    std::array<std::array<float, waveLUTSize>, static_cast<int32_t>(BaseWave::WAVE_TAIL)> waveLUT;
    bool waveLUTInitialized;
    
    // Slope LUTs (shared, but size depends on sampling rate)
    std::unique_ptr<float[]> atackSlopeLUT;
    std::unique_ptr<float[]> releaseSlopeLUT;
    std::unique_ptr<float[]> decaySlopeLUT;
    int32_t numAtackSlopeLUT;
    int32_t numReleaseSlopeLUT;
    int32_t numDecaySlopeLUT;
    float atackSlopeTime;
    float releaseSlopeTime;
    float decaySlopeTime;
    float atackSlopeHz;
    float releaseSlopeHz;
    float decaySlopeHz;
    float decayHalfLifeTime;
    float samplingRate; // Last used sampling rate
    
    // Noise LUTs (shared, but size depends on buffer size)
    std::unique_ptr<float[]> whiteNoiseLUT;
    std::unique_ptr<float[]> pinkNoiseLUT;
    std::unique_ptr<float[]> triangularDistributionLUT;
    std::unique_ptr<float[]> cos4thPowDistributionLUT;
    int32_t noiseBufferSize;
    
    // Other shared LUTs
    std::unique_ptr<float[]> pow2_x_1200LUT;
    std::unique_ptr<float[]> velocity2powerLUT;
    std::unique_ptr<float[]> lowFrequencyCorrectionLUT;
    
    // Private constructor (Singleton pattern)
    SharedLUT();
    
    // Prevent copying
    SharedLUT(const SharedLUT&) = delete;
    SharedLUT& operator=(const SharedLUT&) = delete;
    
public:
    // Get singleton instance
    static SharedLUT& getInstance();
    
    // Initialize shared LUTs
    bool initialize(float rate, int32_t noiseBufSize);
    
    // Cleanup shared LUTs (called when last instance is destroyed)
    void cleanup();
    
    // Increment reference count
    void addRef();
    
    // Decrement reference count and cleanup if needed
    void removeRef();
    
    // Accessors for LUTs
    const std::array<std::array<float, waveLUTSize>, static_cast<int32_t>(BaseWave::WAVE_TAIL)>& getWaveLUT() const { return waveLUT; }
    const float* getAtackSlopeLUT() const { return atackSlopeLUT.get(); }
    const float* getReleaseSlopeLUT() const { return releaseSlopeLUT.get(); }
    const float* getDecaySlopeLUT() const { return decaySlopeLUT.get(); }
    const float* getWhiteNoiseLUT() const { return whiteNoiseLUT.get(); }
    const float* getPinkNoiseLUT() const { return pinkNoiseLUT.get(); }
    const float* getTriangularDistributionLUT() const { return triangularDistributionLUT.get(); }
    const float* getCos4thPowDistributionLUT() const { return cos4thPowDistributionLUT.get(); }
    const float* getPow2_x_1200LUT() const { return pow2_x_1200LUT.get(); }
    const float* getVelocity2powerLUT() const { return velocity2powerLUT.get(); }
    const float* getLowFrequencyCorrectionLUT() const { return lowFrequencyCorrectionLUT.get(); }
    
    int32_t getNumAtackSlopeLUT() const { return numAtackSlopeLUT; }
    int32_t getNumReleaseSlopeLUT() const { return numReleaseSlopeLUT; }
    int32_t getNumDecaySlopeLUT() const { return numDecaySlopeLUT; }
    float getAtackSlopeTime() const { return atackSlopeTime; }
    float getReleaseSlopeTime() const { return releaseSlopeTime; }
    float getDecaySlopeTime() const { return decaySlopeTime; }
    float getDecayHalfLifeTime() const { return decayHalfLifeTime; }
    
    static constexpr int32_t getWaveLUTSize() { return waveLUTSize; }
    static constexpr int32_t getPow2_x_1200LUT_size() { return pow2_x_1200LUT_size; }
    static constexpr int32_t getLowFrequencyCorrectionLUT_size() { return lowFrequencyCorrectionLUT_size; }
};


class Sequencer {
public:
    // constant control params.
    static constexpr int32_t numinstruments = 256;
    static constexpr int32_t numPercussions = 128;

private:
    // constant control params.
    static constexpr int32_t numTone = 64;
    static constexpr float delayBufferDuration = 500.0;// msec

    struct Tone {
        // from smf
        Note note;
        // instrument is shared (singleton), keep pointer only
        const Instrument* instrument = nullptr;
        // for delay (buffer pointer only; indices/ratios are SoA)
        float* delayBuffer = nullptr;
    };
    SMFParser midi;
    int32_t delayBufferSize = 0;
    int32_t delayBufferMask = 0; // for power-of-two ring buffer
    std::vector<float> delayBufferPool;
    float unitOfTime = 60000.0;
    std::array<Tone, numTone> toneInstances;
    std::vector<int32_t> activeToneIndices;
    std::vector<int32_t> freeToneIndices;

    // SoA hot data (indexed by toneInstances index) - order tuned for hot-path locality
    std::array<float, numTone> phase1{};
    std::array<float, numTone> phase2{};
    std::array<float, numTone> phase3{};
    std::array<float, numTone> fmPhase{};
    std::array<float, numTone> amPhase{};
    std::array<float, numTone> fmIncrement{};
    std::array<float, numTone> amIncrement{};
    std::array<float, numTone> baseIncrement1{};
    std::array<float, numTone> baseIncrement2{};
    std::array<float, numTone> baseIncrement3{};
    std::array<float, numTone> frequency{};
    std::array<float, numTone> strength{};
    std::array<float, numTone> atackedStrength{};
    std::array<float, numTone> decayedStrength{};
    std::array<float, numTone> atackedStrengthfloor{};
    std::array<float, numTone> atackSlopeRatio{};
    std::array<float, numTone> decaySlopeRatio{};
    std::array<float, numTone> releaseSlopeRatio{};
    std::array<float, numTone> base1ratio{};
    std::array<float, numTone> base2ratio{};
    std::array<float, numTone> base3ratio{};
    std::array<float, numTone> freqNoiseCentharfRange{};
    std::array<float, numTone> velocity_f{};
    std::array<float, numTone> restartVelocity_f{};
    std::array<float, numTone> tempo_f{};
    std::array<float, numTone> restartTempo_f{};
    std::array<float, numTone> waitDuration{};
    std::array<float, numTone> restartWaitDuration{};
    std::array<float, numTone> mainteinDuration{};
    std::array<float, numTone> maxDelayTime{};
    std::array<int32_t, numTone> noteStartTime{}; // currentTime when note was registered
    std::array<int32_t, numTone> delayBufferIndex{};
    std::array<int32_t, numTone> delay0Index{};
    std::array<int32_t, numTone> delay1Index{};
    std::array<int32_t, numTone> delay2Index{};
    std::array<float, numTone> delay0Ratio{};
    std::array<float, numTone> delay1Ratio{};
    std::array<float, numTone> delay2Ratio{};
    std::array<float, numTone> mainRatio{};
    std::array<int32_t, numTone> passed{};
    std::array<int32_t, numTone> key{};
    std::array<int32_t, numTone> realKey1{};
    std::array<int32_t, numTone> realKey2{};
    std::array<int32_t, numTone> realKey3{};
    std::array<int32_t, numTone> program{};
    std::array<int32_t, numTone> velocity{};
    std::array<int32_t, numTone> restartVelocity{};
    std::array<int32_t, numTone> tempo{};
    std::array<int32_t, numTone> restartTempo{};
    // hot-path flags / modes
    std::array<uint8_t, numTone> useFM{};
    std::array<uint8_t, numTone> useAM{};
    std::array<uint8_t, numTone> useDelay{};
    std::array<uint8_t, numTone> useFreqNoise{};
    std::array<uint8_t, numTone> freqNoiseMode{};  // 0: white, 1: triangular, 2: cos4th
    std::array<uint8_t, numTone> noiseColorMode{}; // 0: white, 1: pink
    std::array<Percussion, numPercussions> percussions;

    struct EmittedEvent {
        int32_t msg = 0;
        struct NotePayload {
            int32_t onOff = 0;
            int32_t trackNum = 0;
            int32_t channel = 0;
            int32_t velocity = 0;
            int32_t program = 0;
            int32_t key = 0;
            int32_t instrumentNum = 0;
            int32_t key2 = 0;
        } note;
        struct LevelPayload {
            int32_t max_level = 0;
            int32_t frame_level = 0;
        } level;
    };
    std::vector<EmittedEvent> eventQueue;
    static constexpr int32_t initialEventCapacity = 64;
    static constexpr int32_t maxEventCapacity = 1024;

    float samplingRate = 44100.0f;
    float bufferingTime = 0.05f;
    int32_t bufferSamples;

    int32_t currentTime = 0;
    int32_t frameCount = 0;
    int32_t noiseBufSize;
    int32_t noiseBuffer;
    bool isSet = false;
    // LUTs are now shared via SharedLUT class
    
    float sustainRate = 0.0;
    
    float asumedConcurrentTone = 4.0f;
    float preOnTime = 0.0f; // Pre-on signal time in milliseconds (0 = disabled)
    std::vector<std::pair<int32_t, int32_t>> preOnOffActiveNotes; // Track active pre_note_on events (channel, key) - using vector for FIFO matching, same as normal sequence's ringingIdx logic
    bool checkNewNote(Note, bool forPreOnOff = false);
    int32_t logLevel = 1;
public:
    double maxValue = 0.0;
    float noteFrequency(int8_t);
    float centFrequency(float, float);
    bool initParam(double, double, int32_t);
    godot::Array getInstruments(void);
    void setInstruments(const godot::Array);
    void setControlParams(const godot::Dictionary);
    godot::Dictionary getControlParams(void);
    void setPercussions(const godot::Array);
    godot::Array getPercussions(void);
    void incertNoteOn(const godot::Dictionary);
    void incertNoteOff(const godot::Dictionary);
    godot::Ref<godot::Image> getMiniWavePicture(const godot::Dictionary);
    bool feed(double*);
    bool smfLoad(const char*, double);
    bool smfLoad(const godot::String &, double);
    bool smfUnload(void);
    std::function<void(const godot::Dictionary dic)> emitSignal;
    Sequencer();
    ~Sequencer();
private:
    void enqueueNoteEvent(int32_t onOff, const Tone& tone, int32_t instrumentNum, int32_t key2, int32_t msg = 0);
    void enqueueNoteEvent(int32_t onOff, const Note& note, int32_t msg = 0); // For preOnOff signals (no Tone)
    void enqueueLevelEvent(double maxValue, double maxFrameValue);
    void flushEvents();
};

#endif // SEQUENCER_H

