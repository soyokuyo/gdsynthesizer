/**************************************************************************/
/*  sequencer.cpp                                                         */
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


#include "sequencer.hpp"
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
#include <godot_cpp/variant/utility_functions.hpp> // for "UtilityFunctions::print()".
#endif // DEBUG_ENABLED && WINDOWS_ENABLED

#include "instrument.hpp"
#include "shared_instruments.hpp"
#include <new>

const char* scale[] = {" C", "C#", " D", "D#", " E", " F", "F#", " G", "G#", " A", "A#", " B"};

// SharedLUT singleton instance
SharedLUT* SharedLUT::instance = nullptr;
int32_t SharedLUT::refCount = 0;

SharedLUT::SharedLUT() 
    : waveLUTInitialized(false)
    , numAtackSlopeLUT(0)
    , numReleaseSlopeLUT(0)
    , numDecaySlopeLUT(0)
    , atackSlopeTime(0.0f)
    , releaseSlopeTime(0.0f)
    , decaySlopeTime(0.0f)
    , atackSlopeHz(25.0f)
    , releaseSlopeHz(25.0f)
    , decaySlopeHz(1.0f)
    , decayHalfLifeTime(50.0f)
    , samplingRate(0.0f)
    , noiseBufferSize(0)
{
}

SharedLUT& SharedLUT::getInstance() {
    if (instance == nullptr) {
        instance = new SharedLUT();
    }
    return *instance;
}

void SharedLUT::addRef() {
    refCount++;
}

void SharedLUT::removeRef() {
    refCount--;
    if (refCount <= 0) {
        cleanup();
        delete instance; // Delete the singleton instance
        instance = nullptr;
        refCount = 0;
    }
}

bool SharedLUT::initialize(float rate, int32_t noiseBufSize) {
    // If already initialized with same parameters, skip
    if (waveLUTInitialized && samplingRate == rate && noiseBufferSize == noiseBufSize) {
        return true;
    }

    // Only cleanup if no other instances are using the LUTs
    // Note: This assumes all instances use the same sampling rate/buffer size
    if (samplingRate != 0.0f && (samplingRate != rate || noiseBufferSize != noiseBufSize)) {
        // Only cleanup if we're the only reference
        if (refCount <= 1) {
            cleanup();
        } else {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            godot::UtilityFunctions::print("[SharedLUT] init denied: samplingRate/noiseBufSize mismatch while refCount>1. current:", samplingRate, "/", noiseBufferSize, " requested:", rate, "/", noiseBufSize);
#endif // DEBUG_ENABLED && WINDOWS_ENABLED
            // Parameters changed but other instances exist - refuse to re-init
            return false;
        }
    }
    
    samplingRate = rate;
    noiseBufferSize = noiseBufSize;
    
    // Initialize wave LUT (only once, as it doesn't depend on sampling rate)
    if (!waveLUTInitialized) {
        int32_t s = SharedLUT::waveLUTSize;
        { // sin wave
            int32_t j = static_cast<int32_t>(BaseWave::WAVE_SIN);
            for (int32_t i = 0; i < s; i++){
                waveLUT[j][i] = sinf(2.0f*PI*(float)i/(float)s);
            }
        }
        { // square wave
            int32_t j = static_cast<int32_t>(BaseWave::WAVE_SQUARE);
            for (int32_t i = 0; i < s; i++){
                waveLUT[j][i] = (i < s/2) ? 1.0f : -1.0f;
            }
        }
        { // triangle wave
            int32_t j = static_cast<int32_t>(BaseWave::WAVE_TRIANGLE);
            for (int32_t i = 0; i < s; i++){
                waveLUT[j][(i+3*s/4)%s] = (i < s/2)?((float)i*4.0f)/((float)s)-1.0f:3.0f-((float)i*4.0f)/((float)s);
            }
        }
        { // sawtooth wave
            int32_t j = static_cast<int32_t>(BaseWave::WAVE_SAWTOOTH);
            for (int32_t i = 0; i < s; i++){
                waveLUT[j][(i+3*s/4)%s] = ((float)i*2.0f)/((float)s)-1.0f;
            }
        }
        { // sin on sawtooth2 wave
            int32_t l = static_cast<int32_t>(BaseWave::WAVE_SAWTOOTH);
            int32_t j = static_cast<int32_t>(BaseWave::WAVE_SIN);
            int32_t k = static_cast<int32_t>(BaseWave::WAVE_SINSAWx2);
            for (int32_t i = 0; i < s; i++){
                waveLUT[k][i] = ((waveLUT[j][i]+1.0f)+(waveLUT[l][(i*2)%s]+1.0f))/2.0f -1.0f;
            }
        }
        waveLUTInitialized = true;
    }
    
    // Initialize slope LUTs (depends on sampling rate)
    {
        numAtackSlopeLUT = (int32_t)((1.0f/atackSlopeHz/2.0f)*samplingRate);
        atackSlopeLUT = std::make_unique<float[]>(numAtackSlopeLUT);
        atackSlopeTime = 1.0f/atackSlopeHz/2.0f*1000.0f;
        for (int32_t i = 0; i < numAtackSlopeLUT; i++){
            atackSlopeLUT[i] = (1.0f-cosf(PI*(float)i/(float)numAtackSlopeLUT))/2.0f;
        }
    }
    {
        numReleaseSlopeLUT = (int32_t)((1.0f/releaseSlopeHz/2.0f)*samplingRate);
        releaseSlopeLUT = std::make_unique<float[]>(numReleaseSlopeLUT);
        releaseSlopeTime = 1.0f/releaseSlopeHz/2.0f*1000.0f;
        for (int32_t i = 0; i < numReleaseSlopeLUT; i++){
            releaseSlopeLUT[i] = (1.0f+cosf(PI*(float)i/(float)numReleaseSlopeLUT))/2.0f;
        }
    }
    {
        numDecaySlopeLUT = (int32_t)((1.0f/decaySlopeHz/2.0f)*samplingRate);
        decaySlopeLUT = std::make_unique<float[]>(numDecaySlopeLUT);
        decaySlopeTime = 1.0f/decaySlopeHz/2.0f*1000.0f;
        for (int32_t i = 0; i < numDecaySlopeLUT; i++){
            decaySlopeLUT[i] = 0.5f+tanhf(log10f(decayHalfLifeTime/(decaySlopeTime*(float)i/(float)numDecaySlopeLUT)))/2.0f;
        }
    }
    {
        float offset = decaySlopeLUT[numDecaySlopeLUT-1];
        float range = 1.0f - offset;
        for (int32_t i = 0; i < numDecaySlopeLUT; i++){
            decaySlopeLUT[i] = (decaySlopeLUT[i]-offset)/range;
        }
    }
    
    // Initialize noise LUTs (depends on buffer size)
    {
        godot::RandomNumberGenerator rand;
        whiteNoiseLUT = std::make_unique<float[]>(noiseBufferSize);
        triangularDistributionLUT = std::make_unique<float[]>(noiseBufferSize);
        cos4thPowDistributionLUT = std::make_unique<float[]>(noiseBufferSize);
        for (int32_t i = 0; i < noiseBufferSize; i++){
            whiteNoiseLUT[i] = (float)rand.randf_range(-1.0, 1.0);
            double r = godot::Math::absf((double)whiteNoiseLUT[i]);
            double c = godot::Math::absf(1-pow(cos(Math_PI*(r*0.5-0.5)), 4.0));
            triangularDistributionLUT[i] = (float)rand.randf_range(-r, r);
            cos4thPowDistributionLUT[i] = (float)rand.randf_range(-c, c);
        }
    }
    
    {
        pinkNoiseLUT = std::make_unique<float[]>(noiseBufferSize);
        PinkNoise pinkNoise = PinkNoise();
        for (int32_t i = 0; i < noiseBufferSize; i++){
            pinkNoiseLUT[i] = pinkNoise.makeNoise(whiteNoiseLUT[i]);
        }
        float head = pinkNoiseLUT[0];
        float tail = pinkNoise.makeNoise(whiteNoiseLUT[0]);
        float diff = (tail- head)/(float)noiseBufferSize;
        float max = -1.0f;
        float min =  1.0f;
        
        for (int32_t i = 0; i < noiseBufferSize; i++){
            pinkNoiseLUT[i] += diff*(float)i;
            if (pinkNoiseLUT[i] > max) max = pinkNoiseLUT[i];
            if (pinkNoiseLUT[i] < min) min = pinkNoiseLUT[i];
        }
        for (int32_t i = 0; i < noiseBufferSize; i++){
            pinkNoiseLUT[i] = (pinkNoiseLUT[i] - min)/(max - min)*2.0f - 1.0f;
        }
    }
    
    // Initialize pow2_x_1200LUT (only once)
    if (!pow2_x_1200LUT) {
        pow2_x_1200LUT = std::make_unique<float[]>(pow2_x_1200LUT_size);
        for (int32_t i = 0; i < pow2_x_1200LUT_size; i++){
            pow2_x_1200LUT[i] = powf(2.0f, (float)(i-pow2_x_1200LUT_size/2)/1200.0f);
        }
    }
    
    // Initialize velocity2powerLUT (only once)
    if (!velocity2powerLUT) {
        velocity2powerLUT = std::make_unique<float[]>(128);
        for (int32_t i = 0; i < 128; i++){
            velocity2powerLUT[i] = powf((float)(i+1)/128.0f, 2.2f);
        }
    }
    
    return true;
}

void SharedLUT::cleanup() {
    atackSlopeLUT.reset();
    releaseSlopeLUT.reset();
    decaySlopeLUT.reset();
    whiteNoiseLUT.reset();
    pinkNoiseLUT.reset();
    triangularDistributionLUT.reset();
    cos4thPowDistributionLUT.reset();
    // Note: waveLUT, pow2_x_1200LUT, and velocity2powerLUT are kept as they don't depend on sampling rate
    // They will be reused if needed
    samplingRate = 0.0f;
    noiseBufferSize = 0;
    // Note: We don't reset waveLUTInitialized to allow reuse of waveLUT
}

PinkNoise::PinkNoise() {
    for (int32_t i = 0; i < tapNum ; i++) z[i] = 0;
    k[tapNum - 1] = 0.5f;
    for (int32_t i = tapNum - 1; i > 0; i--)
        k[i- 1] = k[i] * 0.25f;
}

PinkNoise::~PinkNoise() {
}

float PinkNoise::makeNoise(float in) {
    float q = in;
    for (int32_t i = 0; i < tapNum; i++) {
        z[i] = (q*k[i] + z[i] * (1.0f - k[i]));
        q = (q + z[i]) * 0.5f;
    }
    t = 0.75f * q + 0.25f * t;
    return t;    
}

Sequencer::Sequencer() {
    SharedLUT::getInstance().addRef();
}

Sequencer::~Sequencer(){
    for (int32_t i = 0; i < std::size(toneInstances); i++) {
        delete [] toneInstances[i].delayBuffer;
        toneInstances[i].delayBuffer = nullptr;
    }
    SharedLUT::getInstance().removeRef();
}

float Sequencer::noteFrequency(int8_t note) {
    return (powf(2.0f, ((float)note - 69.0f) / 12.0f)) * 440.0f;
}

float Sequencer::centFrequency(float freq, float cent) {
    auto& lut = SharedLUT::getInstance();
    static const float h = (powf(2.0f,  120.0f/1200.0f)-1.0f)/120.0f;
    static const float l = (powf(2.0f, -120.0f/1200.0f)-1.0f)/120.0f;
    static const float t =  (float(SharedLUT::getPow2_x_1200LUT_size()/2));
    static const float b = -(float(SharedLUT::getPow2_x_1200LUT_size()/2));
    const float* pow2LUT = lut.getPow2_x_1200LUT();

    float result;
    if      (cent <=        b) result = freq * pow2LUT[0];
    else if (cent <   -120.0f) result = freq * pow2LUT[SharedLUT::getPow2_x_1200LUT_size()/2+(int32_t)cent];
    else if (cent <      0.0f) result = freq * (1.0f - cent*l);
    else if (cent <=   120.0f) result = freq * (1.0f + cent*h);
    else if (cent <         t) result = freq * pow2LUT[SharedLUT::getPow2_x_1200LUT_size()/2+(int32_t)cent];
    else                       result = freq * pow2LUT[SharedLUT::getPow2_x_1200LUT_size()-1];
    if (result > samplingRate*0.47f) result = samplingRate*0.47f; // 0.47 is upper limit.
    return result;
}

godot::Array Sequencer::getInstruments(void) {
    const auto& instruments = SharedInstruments::getInstance().getInstruments();
    godot::Array array;
    for (int32_t i = 0; i < 256; i++) {
        godot::Dictionary dic;

        dic["totalGain"]          = instruments[i].totalGain;

        dic["atackSlopeTime"]     = instruments[i].atackSlopeTime;
        dic["decayHalfLifeTime"]  = instruments[i].decayHalfLifeTime;
        dic["sustainRate"]        = instruments[i].sustainRate;
        dic["releaseSlopeTime"]   = instruments[i].releaseSlopeTime;

        dic["baseVsOthersRatio"]  = instruments[i].baseVsOthersRatio;
        dic["side1VsSide2Ratio"]  = instruments[i].side1VsSide2Ratio;
        dic["baseOffsetCent1"]    = instruments[i].baseOffsetCent1;
        dic["baseWave1"]          = static_cast<int32_t>(instruments[i].baseWave1);
        dic["baseOffsetCent2"]    = instruments[i].baseOffsetCent2;
        dic["baseWave2"]          = static_cast<int32_t>(instruments[i].baseWave2);
        dic["baseOffsetCent3"]    = instruments[i].baseOffsetCent3;
        dic["baseWave3"]          = static_cast<int32_t>(instruments[i].baseWave3);

        dic["noiseRatio"]         = instruments[i].noiseRatio;
        dic["noiseColorType"]     = static_cast<int32_t>(instruments[i].noiseColorType);

        dic["delay0Time"]         = instruments[i].delay0Time;
        dic["delay1Time"]         = instruments[i].delay1Time;
        dic["delay2Time"]         = instruments[i].delay2Time;
        dic["delay0Ratio"]        = instruments[i].delay0Ratio;
        dic["delay1Ratio"]        = instruments[i].delay1Ratio;
        dic["delay2Ratio"]        = instruments[i].delay2Ratio;

        dic["freqNoiseCentRange"] = instruments[i].freqNoiseCentRange;
        dic["freqNoiseType"]      = static_cast<int32_t>(instruments[i].freqNoiseType);

        dic["fmCentRange"]        = instruments[i].fmCentRange;
        dic["fmFreq"]             = instruments[i].fmFreq;
        dic["fmPhaseOffset"]      = instruments[i].fmPhaseOffset;
        dic["fmSync"]             = instruments[i].fmSync;
        dic["fmWave"]             = static_cast<int32_t>(instruments[i].fmWave);

        
        dic["amLevel"]            = instruments[i].amLevel;
        dic["amFreq"]             = instruments[i].amFreq;
        dic["amPhaseOffset"]      = instruments[i].amPhaseOffset;
        dic["amSync"]             = instruments[i].amSync;
        dic["amWave"]             = static_cast<int32_t>(instruments[i].amWave);

        array.push_back(dic);
    }
    return array;
}


void Sequencer::setInstruments(const godot::Array array) {
    if (array.size() != 256) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        godot::UtilityFunctions::print("Error in setInstruments(): array size error, ", array.size());
#endif // DEBUG_ENABLED
    }
    int32_t WAVE_TAIL = static_cast<int32_t>(BaseWave::WAVE_TAIL)-1;
    int32_t NOISECTYPE_TAIL = static_cast<int32_t>(NoiseColorType::NOISECTYPE_TAIL)-1;
    int32_t NOISEDTYPE_TAIL = static_cast<int32_t>(NoiseDistributType::NOISEDTYPE_TAIL)-1;
    auto instruments = SharedInstruments::getInstance().getInstruments();
    for (int32_t i = 0; i < 256; i++) {
        godot::Dictionary dic = array[i];

        instruments[i].totalGain          = (float)(godot::Math::clamp((double)(dic["totalGain"]), 0.0, 1.0));
        instruments[i].atackSlopeTime     = (float)(godot::Math::clamp((double)(dic["atackSlopeTime"]), 0.0, 5000.0));
        instruments[i].decayHalfLifeTime  = (float)(godot::Math::clamp((double)(dic["decayHalfLifeTime"]), 0.0, 5000.0));
        instruments[i].sustainRate        = (float)(godot::Math::clamp((double)(dic["sustainRate"]), 0.0, 1.0));
        instruments[i].releaseSlopeTime   = (float)(godot::Math::clamp((double)(dic["releaseSlopeTime"]), 0.0, 5000.0));

        instruments[i].baseVsOthersRatio  = (float)(godot::Math::clamp((double)(dic["baseVsOthersRatio"]), 0.0, 1.0));
        instruments[i].side1VsSide2Ratio  = (float)(godot::Math::clamp((double)(dic["side1VsSide2Ratio"]), 0.0, 1.0));
        instruments[i].baseOffsetCent1    = (float)(godot::Math::clamp((double)(dic["baseOffsetCent1"]), -8400.0, 8400.0));
        instruments[i].baseWave1          = static_cast<BaseWave>(std::clamp((int32_t)dic["baseWave1"], 0, WAVE_TAIL));
        instruments[i].baseOffsetCent2    = (float)(godot::Math::clamp((double)(dic["baseOffsetCent2"]), -8400.0, 8400.0));
        instruments[i].baseWave2          = static_cast<BaseWave>(std::clamp((int32_t)dic["baseWave2"], 0, WAVE_TAIL));
        instruments[i].baseOffsetCent3    = (float)(godot::Math::clamp((double)(dic["baseOffsetCent3"]), -8400.0, 8400.0));
        instruments[i].baseWave3          = static_cast<BaseWave>(std::clamp((int32_t)dic["baseWave3"], 0, WAVE_TAIL));

        instruments[i].noiseRatio         = (float)(godot::Math::clamp((double)(dic["noiseRatio"]), 0.0, 1.0));
        instruments[i].noiseColorType     = static_cast<NoiseColorType>(std::clamp((int32_t)dic["noiseColorType"], 0, NOISECTYPE_TAIL));

        instruments[i].delay0Time         = (float)(godot::Math::clamp((double)(dic["delay0Time"]), 0.0, 500.0));
        instruments[i].delay1Time         = (float)(godot::Math::clamp((double)(dic["delay1Time"]), 0.0, 500.0));
        instruments[i].delay2Time         = (float)(godot::Math::clamp((double)(dic["delay2Time"]), 0.0, 500.0));
        instruments[i].delay0Ratio        = (float)(godot::Math::clamp((double)(dic["delay0Ratio"]), 0.2, 0.2));
        instruments[i].delay1Ratio        = (float)(godot::Math::clamp((double)(dic["delay1Ratio"]), 0.2, 0.2));
        instruments[i].delay2Ratio        = (float)(godot::Math::clamp((double)(dic["delay2Ratio"]), 0.2, 0.2));

        instruments[i].freqNoiseCentRange = (float)(godot::Math::clamp((double)(dic["freqNoiseCentRange"]), -8400.0, 8400.0));
        instruments[i].freqNoiseType      = static_cast<NoiseDistributType>(std::clamp((int32_t)dic["freqNoiseType"], 0, NOISEDTYPE_TAIL));

        instruments[i].fmCentRange        = (float)(godot::Math::clamp((double)(dic["fmCentRange"]), -8400.0, 8400.0));
        instruments[i].fmFreq             = (float)(godot::Math::clamp((double)(dic["fmFreq"]), 0.0, 7040.0));
        instruments[i].fmPhaseOffset      = (float)(godot::Math::clamp((double)(dic["fmPhaseOffset"]), 0.0, 2.0));
        if (instruments[i].fmPhaseOffset == 2.0f) instruments[i].fmPhaseOffset = 0.0f;
        
        instruments[i].fmSync             = (int32_t)(std::clamp((int32_t)dic["fmSync"], 0, 1));
        instruments[i].fmWave             = static_cast<BaseWave>(std::clamp((int32_t)dic["fmWave"], 0, WAVE_TAIL));

        instruments[i].amLevel            = (float)(godot::Math::clamp((double)(dic["amLevel"]), 0.0, 1.0));
        instruments[i].amFreq             = (float)(godot::Math::clamp((double)(dic["amFreq"]), 0.0, 7040.0));
        instruments[i].amPhaseOffset      = (float)(godot::Math::clamp((double)(dic["amPhaseOffset"]), 0.0, 2.0));
        if (instruments[i].amPhaseOffset == 2.0f) instruments[i].amPhaseOffset = 0.0f;

        instruments[i].amSync             = (int32_t)(std::clamp((int32_t)dic["amSync"], 0, 1));
        instruments[i].amWave             = static_cast<BaseWave>(std::clamp((int32_t)dic["amWave"], 0, WAVE_TAIL));
    }
    SharedInstruments::getInstance().setInstruments(instruments);
}


godot::Array Sequencer::getPercussions(void) {
    godot::Array array;
    for (int32_t i = 0; i < 128; i++) {
        godot::Dictionary dic;

        dic["program"]  = percussions[i].program;
        dic["key"]      = percussions[i].key;
        array.push_back(dic);
    }
    return array;
}


void Sequencer::setPercussions(const godot::Array array) {
    if (array.size() != 128) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        godot::UtilityFunctions::print("Error in setPercussions(): array size error, ", array.size());
#endif // DEBUG_ENABLED
    }
    for (int32_t i = 0; i < 128; i++) {
        godot::Dictionary dic = array[i];

        percussions[i].program = (int32_t)(std::clamp((int32_t)dic["program"], 0, 255));
        percussions[i].key     = (int32_t)(std::clamp((int32_t)dic["key"], 0, 127));
    }
}


void Sequencer::setControlParams(const godot::Dictionary dic){
    asumedConcurrentTone = (float)(godot::Math::clamp((double)(dic["divisionNum"]), 0.1, 64.0));
    logLevel = (int32_t)(std::clamp((int32_t)dic["logLevel"], 0, 10));
    maxValue = 0.0;
}

godot::Dictionary Sequencer::getControlParams(void) {
    godot::Dictionary dic;
    dic["divisionNum"] = asumedConcurrentTone;
    dic["logLevel"] = logLevel;
    return dic;
}

bool Sequencer::initParam(double rate, double time, int32_t samples) {
    samplingRate = (float)rate;
    bufferingTime = (float)time;
    bufferSamples = samples;
    currentTime = 0;
    frameCount = 0;
    noiseBufSize = (int32_t)(rate/(double)bufferSamples);
    noiseBuffer = bufferSamples*noiseBufSize;
    freeToneIndices.clear();
    activeToneIndices.clear();

    // Initialize shared LUTs
    if (!SharedLUT::getInstance().initialize(samplingRate, noiseBuffer)) {
        return false;
    }

    // Clean up existing delay buffers before creating new ones
    for (int32_t i = 0; i < std::size(toneInstances); i++) {
        if (toneInstances[i].delayBuffer != nullptr) {
            delete [] toneInstances[i].delayBuffer;
            toneInstances[i].delayBuffer = nullptr;
        }
    }

    // make delay ring buffers
    delayBufferSize = (int32_t)((float)rate*(delayBufferDuration/1000.0f));

    for (int32_t i = 0; i < std::size(toneInstances); i++) {
        toneInstances[i].delayBuffer = new (std::nothrow) float[delayBufferSize];
        if (toneInstances[i].delayBuffer == nullptr) {
            // メモリ確保失敗時は既に確保したバッファをクリーンアップ
            for (int32_t j = 0; j < i; j++) {
                delete [] toneInstances[j].delayBuffer;
                toneInstances[j].delayBuffer = nullptr;
            }
            return false;
        }
        // reset SoA hot data
        phase1[i] = phase2[i] = phase3[i] = 0.0f;
        strength[i] = atackedStrength[i] = decayedStrength[i] = atackedStrengthfloor[i] = 0.0f;
        fmPhase[i] = fmIncrement[i] = amPhase[i] = amIncrement[i] = 0.0f;
        freqNoiseCentharfRange[i] = 0.0f;
        baseIncrement1[i] = baseIncrement2[i] = baseIncrement3[i] = 0.0f;
        delayBufferIndex[i] = delay0Index[i] = delay1Index[i] = delay2Index[i] = 0;
        delay0Ratio[i] = delay1Ratio[i] = delay2Ratio[i] = 0.0f;
        mainRatio[i] = 1.0f;
        velocity[i] = 0;
        velocity_f[i] = 0.0f;
        tempo[i] = 0;
        tempo_f[i] = 0.0f;
        restartVelocity[i] = 0;
        restartVelocity_f[i] = 0.0f;
        restartTempo[i] = 0;
        restartTempo_f[i] = 0.0f;
        atackSlopeRatio[i] = decaySlopeRatio[i] = releaseSlopeRatio[i] = 0.0f;
        base1ratio[i] = base2ratio[i] = base3ratio[i] = 0.0f;
        frequency[i] = 0.0f;
        passed[i] = 0;
        waitDuration[i] = 0.0f;
        restartWaitDuration[i] = 0.0f;
        mainteinDuration[i] = 0.0f;
        maxDelayTime[i] = 0.0f;
        program[i] = 0;
        key[i] = realKey1[i] = realKey2[i] = realKey3[i] = 0;
        useFM[i] = useAM[i] = useDelay[i] = useFreqNoise[i] = 0;
        freqNoiseMode[i] = 0;
        noiseColorMode[i] = 0;
        freeToneIndices.push_back(i);
    }

    SharedInstruments::getInstance().setInstruments(defaultInstruments);
    percussions = defaultPercussions;

    isSet = true;
    return true;
}


bool Sequencer::smfUnload(void) {
    unitOfTime = 60000.0;
    midi.setUnitOfTime(unitOfTime); // milliseconds
    freeToneIndices.clear();
    activeToneIndices.clear();
    for (int32_t i = 0; i < std::size(toneInstances); i++) {
        freeToneIndices.push_back(i);
    }

    midi.unload();
    
    return true;
}


bool Sequencer::smfLoad(const char *name, double givenUnitOfTime) {
    currentTime = 0;
    unitOfTime = (float)givenUnitOfTime;
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
    godot::UtilityFunctions::print("unitOfTime ", unitOfTime);
#endif // DEBUG_ENABLED

    midi.setUnitOfTime(unitOfTime); // milliseconds
    if (midi.load(name) == false) {
        return false;
    }
    
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        godot::UtilityFunctions::print("smf file size: ", midi.filesize);
#endif // DEBUG_ENABLED
    return true;
}


bool Sequencer::smfLoad(const godot::String &name, double givenUnitOfTime) {
    currentTime = 0;
    unitOfTime = (float)givenUnitOfTime;
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
    godot::UtilityFunctions::print("unitOfTime ", unitOfTime);
#endif // DEBUG_ENABLED

    midi.setUnitOfTime(unitOfTime); // milliseconds
    if (midi.load(name) == false) {
        return false;
    }
    
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        godot::UtilityFunctions::print("smf file size: ", midi.filesize);
#endif // DEBUG_ENABLED
    return true;
}


void Sequencer::incertNoteOn(const godot::Dictionary dic){
    Note oneNote;
    oneNote.state     = NState::NS_ON_FOREVER;
    oneNote.trackNum  = 0;
    oneNote.channel   = (int32_t)(std::clamp((int32_t)dic["channel"], 0, 31));
    oneNote.key       = (int32_t)(std::clamp((int32_t)dic["key"], 0, 127));
    oneNote.velocity  = (int32_t)(std::clamp((int32_t)dic["velocity"], 0, 127));
    oneNote.program   = (int32_t)(std::clamp((int32_t)dic["program"], 0, 255));
    oneNote.startTick = 0;
    oneNote.startTime = currentTime;
    oneNote.tempo     = (int32_t)(std::clamp((int32_t)dic["tempo"], 1, 999));

    checkNewNote(oneNote);
};


void Sequencer::incertNoteOff(const godot::Dictionary dic){
    Note oneNote;
    oneNote.state     = NState::NS_OFF;
    oneNote.trackNum  = 0;
    oneNote.channel   = (int32_t)(std::clamp((int32_t)dic["channel"], 0, 31));
    oneNote.key       = (int32_t)(std::clamp((int32_t)dic["key"], 0, 127));
    oneNote.velocity  = (int32_t)(std::clamp((int32_t)dic["velocity"], 0, 127));
    oneNote.program   = (int32_t)(std::clamp((int32_t)dic["program"], 0, 255));
    oneNote.startTick = 0;
    oneNote.startTime = currentTime;
    oneNote.tempo     = (int32_t)(std::clamp((int32_t)dic["tempo"], 1, 999));

    checkNewNote(oneNote);
};


godot::Ref<godot::Image> Sequencer::getMiniWavePicture(const godot::Dictionary dic){
    int32_t size_x = dic["size_x"];
    int32_t size_y = dic["size_y"];
    int32_t type = dic["type"];
    int32_t phase = dic["phase"];
    if (size_x > 400) size_x = 400;
    if (size_x < 16) size_x = 16;
    if (size_y > 400) size_y = 400;
    if (size_y < 16) size_y = 16;
    float invert = 1.0f;
    if (type == static_cast<int32_t>(BaseWave::WAVE_TAIL)){
        type = static_cast<int32_t>(BaseWave::WAVE_SAWTOOTH);
        invert = -1.0f;
    }
    if (type > static_cast<int32_t>(BaseWave::WAVE_TAIL)) type = 0;
    if (type < 0) type = 0;
    if (phase > 360) phase = 360;
    if (phase == 360) phase = 0;
    if (phase < 0) phase = 0;
    phase = (int32_t)((double)phase/360.0*(double)size_x);

    godot::Ref<godot::Image> miniWaveImage;

    miniWaveImage = godot::Image::create(size_x, size_y, false, godot::Image::FORMAT_RGBA8);
    miniWaveImage->fill(godot::Color(0.2, 0.2, 0.2, 1.0));
    auto& lut = SharedLUT::getInstance();
    const auto& waveLUT = lut.getWaveLUT();
    int32_t s = SharedLUT::getWaveLUTSize();
    
    int32_t pre_y;
    for (int32_t i = 0; i < size_x; i++){
        int32_t x = (int32_t)(double((i+phase)%size_x)/double(size_x)*double(s));
        float fy = (1.0f-waveLUT[type][x]*invert)/2.0f;
        if (fy >= 1.0f) fy = 0.99f;
        if (fy <= 0.0f) fy = 0.01f;
        int32_t y = (int32_t)(fy*(float)size_y);
        if (i == 0) pre_y = y;
        if (y < pre_y){
            for (int32_t j = y; j <= pre_y; j++){
                miniWaveImage->set_pixel(i, j, godot::Color(1.0, 0.5, 0.0, 1.0));
            }
        }
        else{
            for (int32_t j = pre_y; j <= y; j++){
                miniWaveImage->set_pixel(i, j, godot::Color(1.0, 0.5, 0.0 , 1.0));
            }
        }
        
        pre_y = y;
    }
    return miniWaveImage;
}


bool Sequencer::checkNewNote(Note oneNote){
    float durationTime = 0;
    if (oneNote.state == NState::NS_ON_FOREVER) durationTime = FLOAT_LONGTIME;
    int32_t ringingIdx = -1;
    for (int32_t idx : activeToneIndices) {
        const Tone& foundTone = toneInstances[idx];
        if (oneNote.key == foundTone.note.key
            && oneNote.channel == foundTone.note.channel
            && foundTone.note.state != NState::NS_OFF) {
            ringingIdx = idx;
            break;
        }
    }
    if (oneNote.state == NState::NS_OFF) {
        if (ringingIdx >= 0) {
            Tone& ringingTone = toneInstances[ringingIdx];
            mainteinDuration[ringingIdx] = (float)(oneNote.startTime - ringingTone.note.startTime);
            ringingTone.note.state = NState::NS_OFF;

            {
                godot::Dictionary dic;

                dic["msg"]                = (int32_t)0;
                dic["onOff"]              = (int32_t)0;
                dic["trackNum"]           = ringingTone.note.trackNum;
                dic["channel"]            = ringingTone.note.channel;
                dic["velocity"]           = ringingTone.note.velocity;
                dic["program"]            = ringingTone.note.program;
                dic["key"]                = ringingTone.note.key;
                dic["instrumentNum"]      = program[ringingIdx];
                dic["key2"]               = key[ringingIdx];
                emitSignal(dic);
            }

#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            if (logLevel > 1){
                godot::UtilityFunctions::print(
                        " state ", static_cast<int32_t>(ringingTone.note.state),
                        "  ch ", ringingTone.note.channel,
                        "  prog ", ringingTone.note.program,
                        "  key ", ringingTone.note.key,
                        "  scale ", scale[ringingTone.note.key%12],(uint16_t)(ringingTone.note.key / 12) - 1,
                        "  end(ms) ", ringingTone.note.startTime
                );
            }
#endif // DEBUG_ENABLED
        }
        else {
            return false;
        }
    }
    else if (!freeToneIndices.empty()){
        int32_t idx = freeToneIndices.back();
        freeToneIndices.pop_back();
        Tone& tone = toneInstances[idx];

        tone.note = oneNote;

        phase1[idx] = phase2[idx] = phase3[idx] = 0.0f;
        key[idx] = oneNote.key;
        frequency[idx] = noteFrequency(oneNote.key);
        passed[idx] = 0;
        waitDuration[idx] = (float)(oneNote.startTime - currentTime);

        mainteinDuration[idx] = durationTime;
        restartWaitDuration[idx] = FLOAT_LONGTIME;
        restartTempo[idx] = tempo[idx] = oneNote.tempo;
        restartTempo_f[idx] = tempo_f[idx] = (float)oneNote.tempo*1000.0f; // msec
        restartVelocity[idx] = velocity[idx] = oneNote.velocity;

        // select instrument
        const auto& instruments = SharedInstruments::getInstance().getInstruments();
        if (tone.note.channel > 127 || tone.note.channel < 0) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            godot::UtilityFunctions::print("invalid tone->note.channel ", tone.note.channel);
#endif // DEBUG_ENABLED
            program[idx] = 0;
            tone.instrument = &instruments[0];
            tone.note.velocity = 0;
        }
        else if (tone.note.channel == 9  || tone.note.channel == 25) { // 9 is ch10 that is reserved for Percussions.
            program[idx] = percussions[tone.note.key].program;
            tone.instrument = &instruments[percussions[tone.note.key].program];
            key[idx] = percussions[tone.note.key].key;
            frequency[idx] = noteFrequency(percussions[tone.note.key].key);
        }
        else {
            if (oneNote.program >= 0x70  && oneNote.program < 0x80){ // Percussives and Sound effects.
                program[idx] = percussions[oneNote.program].program;
                tone.instrument = &instruments[percussions[oneNote.program].program];
                key[idx] = percussions[oneNote.program].key;
                frequency[idx] = noteFrequency(percussions[oneNote.program].key);
            }
            else{
                program[idx] = oneNote.program;
                tone.instrument = &instruments[oneNote.program];
            }
        }
        useFM[idx] = (tone.instrument->fmFreq != 0.0f) ? 1 : 0;
        useAM[idx] = (tone.instrument->amFreq != 0.0f) ? 1 : 0;
        {
            realKey1[idx] = key[idx] + (int32_t)(tone.instrument->baseOffsetCent1/100.0f);
            realKey2[idx] = key[idx] + (int32_t)(tone.instrument->baseOffsetCent2/100.0f);
            realKey3[idx] = key[idx] + (int32_t)(tone.instrument->baseOffsetCent3/100.0f);
        }
        {
            godot::Dictionary dic;
            dic["msg"]                = (int32_t)0;
            dic["onOff"]              = (int32_t)1;
            dic["trackNum"]           = tone.note.trackNum;
            dic["channel"]            = tone.note.channel;
            dic["velocity"]           = tone.note.velocity;
            dic["program"]            = tone.note.program;
            dic["key"]                = tone.note.key;
            dic["instrumentNum"]      = program[idx];
            dic["key2"]               = key[idx];
            emitSignal(dic);
        }
        {
            auto& lut = SharedLUT::getInstance();
            restartVelocity_f[idx] = velocity_f[idx] = lut.getVelocity2powerLUT()[tone.note.velocity];
            atackedStrengthfloor[idx] = 0.0f;
            strength[idx] = 0.0f;
            atackedStrength[idx] = 0.0f;
            decayedStrength[idx] = 0.0f;
        }
        base1ratio[idx] = tone.instrument->baseVsOthersRatio;
        base2ratio[idx] = (1.0f-tone.instrument->baseVsOthersRatio)*tone.instrument->side1VsSide2Ratio;
        base3ratio[idx] = (1.0f-tone.instrument->baseVsOthersRatio)*(1.0f-tone.instrument->side1VsSide2Ratio);
                
        // fm moduration related.
        fmPhase[idx]= PI * tone.instrument->fmPhaseOffset;
        fmIncrement[idx] = 0.0f;
        if (tone.instrument->fmFreq != 0.0f) {
            if (tone.instrument->fmSync == 0){
                fmIncrement[idx] = (2.0f * PI * tone.instrument->fmFreq ) / samplingRate;
            }
            else{
                fmIncrement[idx] = (2.0f * PI * tone.instrument->fmFreq * tempo_f[idx] / unitOfTime) / samplingRate;
            }
        }

        // am moduration related.
        amPhase[idx]= PI * tone.instrument->amPhaseOffset;
        amIncrement[idx] = 0.0f;
        if (tone.instrument->amFreq != 0.0f) {
            if (tone.instrument->amSync == 0){
                amIncrement[idx] = (2.0f * PI * tone.instrument->amFreq ) / samplingRate;
            }
            else{
                amIncrement[idx] = (2.0f * PI * tone.instrument->amFreq * tempo_f[idx] / unitOfTime) / samplingRate;
            }
        }

        // variable freqNoise related. SIMD候補: baseIncrement計算とノイズ適用のペア演算
        {
            freqNoiseCentharfRange[idx] = tone.instrument->freqNoiseCentRange*0.5f;
            float c1 = centFrequency(frequency[idx], tone.instrument->baseOffsetCent1);
            float l1 = centFrequency(c1, -(freqNoiseCentharfRange[idx]));
            baseIncrement1[idx]  = (2.0f * PI * l1) / samplingRate;

            float c2 = centFrequency(frequency[idx], tone.instrument->baseOffsetCent2);
            float l2 = centFrequency(c2, -(freqNoiseCentharfRange[idx]));
            baseIncrement2[idx]  = (2.0f * PI * l2) / samplingRate;

            float c3 = centFrequency(frequency[idx], tone.instrument->baseOffsetCent3);
            float l3 = centFrequency(c3, -(freqNoiseCentharfRange[idx]));
            baseIncrement3[idx]  = (2.0f * PI * l3) / samplingRate;
        }

        // init delay ring buffer
        delayBufferIndex[idx] = 0;
        delay0Index[idx] = delay1Index[idx] = delay2Index[idx] = 0;
        delay0Ratio[idx] = delay1Ratio[idx] = delay2Ratio[idx] = 0.0f;
        maxDelayTime[idx] = 0.0f;
        if (   tone.instrument->delay0Time > 0.0f
            && tone.instrument->delay0Time < delayBufferDuration
            && tone.instrument->delay0Ratio < 1.00f
            && tone.instrument->delay0Ratio > 0.0f)
        {
            maxDelayTime[idx] = tone.instrument->delay0Time;
            delay0Index[idx] = (uint32_t)((float)delayBufferSize/delayBufferDuration * tone.instrument->delay0Time);
            delay0Ratio[idx] = tone.instrument->delay0Ratio;
        }
        if (   tone.instrument->delay1Time > 0.0f
            && tone.instrument->delay1Time < delayBufferDuration
            && tone.instrument->delay1Ratio < 1.00f
            && tone.instrument->delay1Ratio > 0.0f)
        {
            if (tone.instrument->delay1Time > maxDelayTime[idx]) maxDelayTime[idx] = tone.instrument->delay1Time;
            delay1Index[idx] = (uint32_t)((float)delayBufferSize/delayBufferDuration * tone.instrument->delay1Time);
            delay1Ratio[idx] = tone.instrument->delay1Ratio;
        }
        if (   tone.instrument->delay2Time > 0.0f
            && tone.instrument->delay2Time < delayBufferDuration
            && tone.instrument->delay2Ratio < 1.00f
            && tone.instrument->delay2Ratio > 0.0f)
        {
            if (tone.instrument->delay2Time > maxDelayTime[idx]) maxDelayTime[idx] = tone.instrument->delay2Time;
            delay2Index[idx] = (uint32_t)((float)delayBufferSize/delayBufferDuration * tone.instrument->delay2Time);
            delay2Ratio[idx] = tone.instrument->delay2Ratio;
        }
        maxDelayTime[idx] *= 3.0f;

        mainRatio[idx] = 1.0f - (delay0Ratio[idx]+delay1Ratio[idx]+delay2Ratio[idx]);
        for (int32_t i = 0; i < delayBufferSize; i++) {
            tone.delayBuffer[i] = 0.0f;
        }
        useDelay[idx] = (delay0Ratio[idx] != 0.0f) || (delay1Ratio[idx] != 0.0f) || (delay2Ratio[idx] != 0.0f);

        {
            auto& lut = SharedLUT::getInstance();
            atackSlopeRatio[idx] = lut.getAtackSlopeTime()/tone.instrument->atackSlopeTime;
            decaySlopeRatio[idx] = lut.getDecayHalfLifeTime()/tone.instrument->decayHalfLifeTime;
            releaseSlopeRatio[idx] = lut.getReleaseSlopeTime()/tone.instrument->releaseSlopeTime;
        }

        // noise mode flags
        useFreqNoise[idx] = (freqNoiseCentharfRange[idx] != 0.0f) ? 1 : 0;
        switch (tone.instrument->freqNoiseType) {
            case NoiseDistributType::NOISEDTYPE_TRIANGULAR: freqNoiseMode[idx] = 1; break;
            case NoiseDistributType::NOISEDTYPE_COS4ThPOW:  freqNoiseMode[idx] = 2; break;
            default: freqNoiseMode[idx] = 0; break;
        }
        noiseColorMode[idx] = (tone.instrument->noiseColorType == NoiseColorType::NOISECTYPE_PINK) ? 1 : 0;

        activeToneIndices.push_back(idx);

    }
    else {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        godot::UtilityFunctions::print("Error: no free tone.");
#endif // DEBUG_ENABLED
        return false;
    }
    return true;
}

bool Sequencer::feed(double *frame){
    for (int i=0; i < bufferSamples; i++) frame[i] = 0.0;

    int32_t frameTime = (int32_t)(bufferingTime*1000.0f);
    Note oneNote;
    while(isSet) {
        oneNote = midi.parse(currentTime + frameTime);
        if (oneNote.state == NState::NS_END || oneNote.state == NState::NS_EMPTY) {
            break;
        }
        if (checkNewNote(oneNote) == false) break ;
    }
    currentTime += frameTime;
    int32_t noiseBufIndex = frameCount*bufferSamples;
    auto& lut = SharedLUT::getInstance();
    const auto& waveLUT = lut.getWaveLUT();
    float period = (float)std::size(waveLUT[0])/(PI*2.0f);
    float delta = 1.0f/samplingRate*1000.0f;
    float div = 1.0f/asumedConcurrentTone; // to avoid saturation.

    // Hot-path LUT pointers (SIMD化に備えループ外へ)
    const float* whiteLUT = lut.getWhiteNoiseLUT();
    const float* triangularLUT = lut.getTriangularDistributionLUT();
    const float* cos4thPowLUT = lut.getCos4thPowDistributionLUT();
    const float* pinkLUT = lut.getPinkNoiseLUT();

    for (size_t tonePos = 0; tonePos < activeToneIndices.size();) {
        int32_t toneIndex = activeToneIndices[tonePos];
        Tone& toneRef = toneInstances[toneIndex];
        float current = (float)passed[toneIndex];
        float& ph1 = phase1[toneIndex];
        float& ph2 = phase2[toneIndex];
        float& ph3 = phase3[toneIndex];
        float& st = strength[toneIndex];
        float& atkSt = atackedStrength[toneIndex];
        float& decSt = decayedStrength[toneIndex];
        float& atkFloor = atackedStrengthfloor[toneIndex];
        float& fmPh = fmPhase[toneIndex];
        float& fmInc = fmIncrement[toneIndex];
        float& amPh = amPhase[toneIndex];
        float& amInc = amIncrement[toneIndex];
        float& wt = waitDuration[toneIndex];
        float& rw = restartWaitDuration[toneIndex];
        float& md = mainteinDuration[toneIndex];
        float& atkRatio = atackSlopeRatio[toneIndex];
        float& decRatio = decaySlopeRatio[toneIndex];
        float& relRatio = releaseSlopeRatio[toneIndex];
        float& b1ratio = base1ratio[toneIndex];
        float& b2ratio = base2ratio[toneIndex];
        float& b3ratio = base3ratio[toneIndex];
        float& velF = velocity_f[toneIndex];
        float& rVelF = restartVelocity_f[toneIndex];
        float& maxDelay = maxDelayTime[toneIndex];
        int32_t& rk1 = realKey1[toneIndex];
        int32_t& rk2 = realKey2[toneIndex];
        int32_t& rk3 = realKey3[toneIndex];
        const float* freqNoiseLUT = (freqNoiseMode[toneIndex] == 1) ? triangularLUT
                                     : (freqNoiseMode[toneIndex] == 2) ? cos4thPowLUT
                                     : whiteLUT;
        const float* noiseMixLUT = (noiseColorMode[toneIndex] == 1) ? pinkLUT : whiteLUT;
        bool doFM = (useFM[toneIndex] != 0);
        bool doAM = (useAM[toneIndex] != 0);
        bool doDelay = (useDelay[toneIndex] != 0);
        bool doFreqNoise = (useFreqNoise[toneIndex] != 0);
        const float noiseRatio = toneRef.instrument->noiseRatio;
        bool doNoiseMix = (noiseRatio != 0.0f);
        bool isEnd = false;
        int32_t sinWave   = static_cast<int32_t>(BaseWave::WAVE_SIN);
        int32_t baseWave1 = static_cast<int32_t>(toneRef.instrument->baseWave1);
        int32_t baseWave2 = static_cast<int32_t>(toneRef.instrument->baseWave2);
        int32_t baseWave3 = static_cast<int32_t>(toneRef.instrument->baseWave3);
        int32_t fmWave = static_cast<int32_t>(toneRef.instrument->fmWave);
        float fmWaveInvert = 1.0f;
        if (toneRef.instrument->fmWave == BaseWave::WAVE_SINSAWx2){
            fmWave = static_cast<int32_t>(BaseWave::WAVE_SAWTOOTH);
            fmWaveInvert = -1.0f;
        }
        int32_t amWave = static_cast<int32_t>(toneRef.instrument->amWave);
        float amWaveInvert = 1.0f;
        if (toneRef.instrument->amWave == BaseWave::WAVE_SINSAWx2){
            amWave = static_cast<int32_t>(BaseWave::WAVE_SAWTOOTH);
            amWaveInvert = -1.0f;
        }
        const float sustainRate = toneRef.instrument->sustainRate;
        const float fmCentRange = toneRef.instrument->fmCentRange;
        const float amLevel = toneRef.instrument->amLevel;
        const float totalGain = toneRef.instrument->totalGain;
        const float atackSlopeTime = toneRef.instrument->atackSlopeTime;
        const float releaseSlopeTime = toneRef.instrument->releaseSlopeTime;
        const float releaseStart = wt + md;
        const float releaseEnd = releaseStart + releaseSlopeTime + maxDelay;
        const float attackEnd = wt + atackSlopeTime;
        double maxFrameValue = 0.0;
#if defined(GDSYNTH_USE_X86_SIMD)
#pragma GCC ivdep
#pragma GCC unroll 4
#endif
        // SIMD hot path start: envelope + phase update + noise + AM/FM + mix + delay
        for (int32_t i = 0; i < bufferSamples; i++){
            bool isTone = false;
            if (current > rw) {
                toneRef.note.state = NState::NS_ON_FOREVER;
                md = rw = FLOAT_LONGTIME;
                atkFloor = st;
                wt = current;
                tempo_f[toneIndex] = restartTempo_f[toneIndex];
                velF = rVelF;
            }
            if (current > releaseEnd){
                isEnd = true;
                break;
            }
            else if (current > releaseStart){ // release
                int32_t d = (int32_t)(((current-(wt+md))*relRatio)/delta);
                const float* releaseLUT = lut.getReleaseSlopeLUT();
                if (d >= lut.getNumReleaseSlopeLUT()) d = lut.getNumReleaseSlopeLUT() - 1;
                atkFloor = st = decSt*releaseLUT[d];
                isTone = true;
            }
            else if (current > attackEnd){ // decay and sustain
                int32_t d = (int32_t)(((current-attackEnd)*decRatio)/delta);
                const float* decayLUT = lut.getDecaySlopeLUT();
                if (d >= lut.getNumDecaySlopeLUT()) d = lut.getNumDecaySlopeLUT() - 1;
                st = atkSt*((decayLUT[d]*(1.0f-sustainRate)+sustainRate));
                atkFloor = decSt = st;
                isTone = true;
            }
            else if (current > wt){ // atack
                int32_t d = (int32_t)((current-wt)*atkRatio/delta);
                const float* atackLUT = lut.getAtackSlopeLUT();
                if (d >= lut.getNumAtackSlopeLUT()) d = lut.getNumAtackSlopeLUT() - 1;
                st = atackLUT[d]*(1.0f-atkFloor)+atkFloor;
                decSt = atkSt = st;
                isTone = true;
            }
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            if (atkSt > 1.0f) godot::UtilityFunctions::print("atackedStrength saturated! ", atkSt);
            if (decSt > 1.0f) godot::UtilityFunctions::print("decayedStrength saturated! ", decSt);
            if (st > 1.0f)    godot::UtilityFunctions::print("strength saturated! ", st);
            if (atkSt < 0.0f) godot::UtilityFunctions::print("atackedStrength underflowed! ", atkSt);
            if (decSt < 0.0f) godot::UtilityFunctions::print("decayedStrength underflowed! ", decSt);
            if (st < 0.0f)    godot::UtilityFunctions::print("strength underflowed! ", st);
#endif // DEBUG_ENABLED
            if (isTone){
                float inc1, inc2, inc3;
                float cent = 0.0f;
                if (doFreqNoise) {
                    cent = freqNoiseCentharfRange[toneIndex]*freqNoiseLUT[noiseBufIndex+i];
                }
                if (doFM && current > wt){
                    fmPh += fmInc;
                    if (fmPh > PI*2.0f) fmPh -= PI*2.0f;
                    cent += fmCentRange*(waveLUT[fmWave][(int32_t)(fmPh*period)]*fmWaveInvert+1.0f)*0.5f;
                }
                
                inc1 = centFrequency(baseIncrement1[toneIndex], cent);
                inc2 = centFrequency(baseIncrement2[toneIndex], cent);
                inc3 = centFrequency(baseIncrement3[toneIndex], cent);
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                if (inc1 < 0.0f) godot::UtilityFunctions::print("inc1 is going backwards! ", inc1);
                if (inc2 < 0.0f) godot::UtilityFunctions::print("inc2 is going backwards! ", inc2);
                if (inc3 < 0.0f) godot::UtilityFunctions::print("inc3 is going backwards! ", inc3);
#endif // DEBUG_ENABLED
                
                ph1 += inc1;
                if (ph1 > PI*2.0f) ph1 -= PI*2.0f;
                ph2 += inc2;
                if (ph2 > PI*2.0f) ph2 -= PI*2.0f;
                ph3 += inc3;
                if (ph3 > PI*2.0f) ph3 -= PI*2.0f;
                
                float level = 1.0f;
                if (doAM && current > wt){
                    amPh += amInc;
                    if (amPh > PI*2.0f) amPh -= PI*2.0f;
                    level = (amLevel)*(waveLUT[amWave][(int32_t)(amPh*period)]*amWaveInvert+1.0f)*0.5f;
                    level += 1.0f - amLevel;
                }

#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                if (level > 1.0f) godot::UtilityFunctions::print("level saturated! ", level);
#endif // DEBUG_ENABLED
                
                float tone1, tone2, tone3;
                {
                    double c = 1.0/120.0; // key 120 may be 8372.0Hz
                    double f1 = (double)waveLUT[sinWave][(int32_t)(ph1*period)];
                    double f2 = (double)waveLUT[sinWave][(int32_t)(ph2*period)];
                    double f3 = (double)waveLUT[sinWave][(int32_t)(ph3*period)];

                    double g1 = (double)waveLUT[baseWave1][(int32_t)(ph1*period)];
                    double g2 = (double)waveLUT[baseWave2][(int32_t)(ph2*period)];
                    double g3 = (double)waveLUT[baseWave3][(int32_t)(ph3*period)];

                    double r1 = godot::Math::clamp((double)(rk1)*c, 0.0, 1.0);
                    double r2 = godot::Math::clamp((double)(rk2)*c, 0.0, 1.0);
                    double r3 = godot::Math::clamp((double)(rk3)*c, 0.0, 1.0);

                    tone1 = (float)godot::Math::lerp(g1, f1, r1)*b1ratio;
                    tone2 = (float)godot::Math::lerp(g2, f2, r2)*b2ratio;
                    tone3 = (float)godot::Math::lerp(g3, f3, r3)*b3ratio;
                }
                float data = tone1+tone2+tone3;
                
                if (doNoiseMix) {
                    data = data*(1.0f - noiseRatio)+noiseMixLUT[noiseBufIndex+i]*noiseRatio;
                }

#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                if (godot::Math::absf(data) > 1.0){
                    godot::UtilityFunctions::print("data 1 saturated! ", data);
                }
#endif // DEBUG_ENABLED
                data = godot::Math::clamp(data, -1.0f, 1.0f);

                data *= (velF*st*div*level)*totalGain;
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                if (godot::Math::absf(data) > 1.0){
                    godot::UtilityFunctions::print("data 2 saturated! ", data);
                }
#endif // DEBUG_ENABLED
                data = godot::Math::clamp(data, -1.0f, 1.0f);

                if (doDelay) {
                    data = data * mainRatio[toneIndex] + toneRef.delayBuffer[delayBufferIndex[toneIndex]];
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                    if (godot::Math::absf(data) > 1.0){
                        godot::UtilityFunctions::print("data 3 saturated! ", data);
                    }
#endif // DEBUG_ENABLED
                    data = godot::Math::clamp(data, -1.0f, 1.0f);
                
                    // delay
                    float delayData;
                    delayData = toneRef.delayBuffer[delay0Index[toneIndex]] + data * delay0Ratio[toneIndex];
                    if (delayData >  1.0) delayData =  1.0;
                    if (delayData < -1.0) delayData = -1.0;
                    toneRef.delayBuffer[delay0Index[toneIndex]] = delayData;
                    delayData = toneRef.delayBuffer[delay1Index[toneIndex]] + data * delay1Ratio[toneIndex];
                    if (delayData >  1.0) delayData =  1.0;
                    if (delayData < -1.0) delayData = -1.0;
                    toneRef.delayBuffer[delay1Index[toneIndex]] = delayData;
                    delayData = toneRef.delayBuffer[delay2Index[toneIndex]] + data * delay2Ratio[toneIndex];
                    if (delayData >  1.0) delayData =  1.0;
                    if (delayData < -1.0) delayData = -1.0;
                    toneRef.delayBuffer[delay2Index[toneIndex]] = delayData;

                    delay0Index[toneIndex] += 1;
                    if (delay0Index[toneIndex] == delayBufferSize) delay0Index[toneIndex] = 0;
                    delay1Index[toneIndex] +=1;
                    if (delay1Index[toneIndex] == delayBufferSize) delay1Index[toneIndex] = 0;
                    delay2Index[toneIndex] += 1;
                    if (delay2Index[toneIndex] == delayBufferSize) delay2Index[toneIndex] = 0;
                    toneRef.delayBuffer[delayBufferIndex[toneIndex]] = 0.0f;
                    delayBufferIndex[toneIndex] +=1;
                    if (delayBufferIndex[toneIndex] == delayBufferSize) delayBufferIndex[toneIndex] = 0;
                }

                frame[i] += (double)data;
                if (godot::Math::absf(frame[i]) > maxFrameValue) maxFrameValue = godot::Math::absf(frame[i]);
                frame[i] = godot::Math::clamp(frame[i], -1.0, 1.0);
            }
            current += delta;
        }
        // SIMD hot path end
        if (maxFrameValue > 1.0){
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            godot::UtilityFunctions::print("saturated! ", maxFrameValue);
#endif // DEBUG_ENABLED
        }
        if (maxFrameValue > maxValue) maxValue = maxFrameValue;
        {
            godot::Dictionary dic;
            dic["msg"]                = (int32_t)1;
            dic["max_level"]          = (int32_t)(maxValue*1000.0);
            dic["frame_level"]        = (int32_t)(maxFrameValue*1000.0);
            emitSignal(dic);
        }

        maxFrameValue = 0.0;
        if (isEnd && rw == FLOAT_LONGTIME){
            ph1 = ph2 = ph3  = 0.0f;
            st = 0.0f;
            atkSt = 0.0f;
            decSt = 0.0f;
            freeToneIndices.push_back(toneIndex);
            activeToneIndices[tonePos] = activeToneIndices.back();
            activeToneIndices.pop_back();
            continue;
        }
        passed[toneIndex] += (int32_t)(delta * (float)bufferSamples);
        tonePos++;
    }
    frameCount += 1;
    frameCount %= noiseBufSize;

    if (oneNote.state == NState::NS_END && activeToneIndices.empty()){
        midi.restart();
//        currentTime = -1000; // wait 1sec for repetition.
        currentTime = 0; // or executed immediately without waiting.
    }
    return true;
}
