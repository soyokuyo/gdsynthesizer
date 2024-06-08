/**************************************************************************/
/*  instrument.hpp                                                        */
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


#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#define BW_SIN         BaseWave::WAVE_SIN
#define BW_SQUARE      BaseWave::WAVE_SQUARE
#define BW_TRIANGLE    BaseWave::WAVE_TRIANGLE
#define BW_SAWTOOTH    BaseWave::WAVE_SAWTOOTH
#define BW_SINSAWx2    BaseWave::WAVE_SINSAWx2 // in base wave OC, it's used as doubled sawtooth on sin wave.
#define BW_SAWT_INV    BaseWave::WAVE_SINSAWx2 // in LFO, it's used as inverted sawtooth wave.

#define NDT_FLAT       NoiseDistributType::NOISEDTYPE_FLAT     
#define NDT_TRIANGULAR NoiseDistributType::NOISEDTYPE_TRIANGULAR
#define NDT_COS4ThPOW  NoiseDistributType::NOISEDTYPE_COS4ThPOW

#define NCT_WHITE      NoiseColorType::NOISECTYPE_WHITE
#define NCT_PINK       NoiseColorType::NOISECTYPE_PINK

const std::array<Instrument, Sequencer::numinstruments> defaultInstruments = {
    {
        //
        //                                          atackSlopeTime                                                                                                                                                                                                                                                             
        //                                            |     decayHalfLifeTime                                                                                                                                                                                                                                                  
        //                                            |       |   sustainRate                                                                                                                                                                                                                                                  
        //                                            |       |     |     releaseSlopeTime                                                                                                                                                                                                                                     
        //                                            |       |     |       |                                                                                                   delay0Time                                                                                                                                     
        //                                            |       |     |       |    baseVsOthersRatio                                                                                 |   delay1Time                          freqNoizeCentRange      fmCentRange                            amLevel                              
        //                                            |       |     |       |      | side1VsSide2Ratio                                                                             |      |   delay2Time                       | freqNoiseType         |  fmFreq                              |  amFreq                        
        //                                            |       |     |       |      |    |                                                                                          |      |      |  delay0Ratio                |    |                  |     |  fmPhaseOffset                 |     |  amPhaseOffset           
        //                                totalGain   |       |     |       |      |    |      baseOffsetCent1      baseOffsetCent2       baseOffsetCent2     noiseRatio           |      |      |     |   delay1Ratio         |    |                  |     |     | fmSync                   |     |     | amSync             
        //                                   |        |       |     |       |      |    |        |  baseWave1          | baseWave2           | baseWave2         | noiseColorType  |      |      |     |     |   delay2Ratio   |    |                  |     |     |    |  fmWave             |     |     |    |  amWave       
        //                                   |        |       |     |       |      |    |        |    |                |    |                |    |              |    |            |      |      |     |     |     |           |    |                  |     |     |    |   |                 |     |     |    |   |           
        /* Piano                       */ 
        /*00    Acoustic Piano         */ { 1.00,   15.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*01    Bright Piano           */ { 1.00,   20.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*02    Electric Grand Piano   */ { 1.00,   20.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*03    Honky-tonk Piano       */ { 1.00,   20.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*04    Electric Piano         */ { 1.00,   20.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*05    Electric Piano 2       */ { 1.00,   20.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*06    Harpsichord            */ { 1.00,    5.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*07    Clavi                  */ { 1.00,   20.0,  110.0,  0.10, 120.0,   0.4, 0.75,    0.0, BW_SIN     ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  0.01, NCT_PINK ,   5.0,  11.0,  17.0,  0.20, 0.10, 0.05,      30.0, NDT_TRIANGULAR,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /* Chromatic Percussion        */ 
        /*08    Celesta                */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*09    Glockenspiel           */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*0A    Music box              */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*0B    Vibraphone             */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*0C    Marimba                */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*0D    Xylophone              */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*0E    Tubular Bell           */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*0F    Dulcimer               */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     240.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Organ                      */ 
        /*10    Drawbar Organ          */ { 1.00,   20.0,  150.0,  1.00,  20.0,   0.6, 0.6,     0.0, BW_SAWTOOTH,  1200.0, BW_SAWTOOTH,  2400.0, BW_TRIANGLE,  0.01, NCT_PINK ,  99.0, 127.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*11    Percussive Organ       */ { 1.00,   20.0,  120.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,  1200.0, BW_SINSAWx2,  2400.0, BW_SINSAWx2,  0.01, NCT_PINK ,  99.0, 127.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*12    Rock Organ             */ { 1.00,   20.0,  150.0,  1.00,  20.0,   0.6, 0.6,     0.0, BW_SAWTOOTH,  1200.0, BW_SAWTOOTH,  2400.0, BW_TRIANGLE,  0.01, NCT_PINK ,  99.0, 127.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*13    Church organ           */ { 0.80,   20.0,  150.0,  1.00,  20.0,   0.6, 0.6,     0.0, BW_SINSAWx2,  1200.0, BW_SAWTOOTH,  2400.0, BW_TRIANGLE,  0.01, NCT_PINK ,  99.0, 127.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*14    Reed organ             */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*15    Accordion              */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*16    Harmonica              */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*17    Tango Accordion        */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Guitar                     */ 
        /*18    Acoustic Guitar (nylon)*/ { 1.00,   20.0,  120.0,  0.05, 160.0,  0.33, 0.5,     0.0, BW_SQUARE  ,  1200.0, BW_SAWTOOTH,  2400.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  27.0, 167.0, 197.0,  0.15, 0.20, 0.05,     400.0, NDT_TRIANGULAR,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*19    Acoustic Guitar (steel)*/ { 1.00,   20.0,  150.0,  0.05, 160.0,  0.33, 0.5,     0.0, BW_SQUARE  ,  1200.0, BW_SAWTOOTH,  2400.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  27.0, 167.0, 197.0,  0.15, 0.20, 0.05,      40.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*1A    Electric Guitar (jazz) */ { 1.00,   20.0,  150.0,  0.10,  20.0,   0.8, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*1B    Electric Guitar (clean)*/ { 1.00,   20.0,  150.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*1C    Electric Guitar (muted)*/ { 1.00,   20.0,   30.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*1D    Overdriven Guitar      */ { 1.00,   20.0,   50.0,  0.70,  20.0,   0.5, 0.7,     0.0, BW_SQUARE  ,  1200.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*1E    Distortion Guitar      */ { 1.00,   20.0,   50.0,  0.70,  20.0,   0.5, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*1F    Guitar harmonics       */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SIN     ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Bass                       */ 
        /*20    Acoustic Bass          */ { 0.50,   40.0,  120.0,  0.05,  50.0,   0.7, 0.7,     0.0, BW_SIN     ,     0.0, BW_SAWTOOTH,  1200.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*21    Electric Bass (finger) */ { 0.50,   40.0,  120.0,  0.05,  50.0,   0.7, 0.7,     0.0, BW_SIN     ,     0.0, BW_SAWTOOTH,  1200.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*22    Electric Bass (pick)   */ { 0.50,   20.0,  100.0,  0.10,  50.0,   0.7, 0.7,     0.0, BW_SIN     ,     0.0, BW_SAWTOOTH,  1200.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*23    Fretless Bass          */ { 1.00,   60.0,  150.0,  0.10,  50.0,   0.9, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*24    Slap Bass 1            */ { 1.00,   30.0,  100.0,  0.10,  50.0,   0.9, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*25    Slap Bass 2            */ { 1.00,   30.0,  100.0,  0.10,  50.0,   0.9, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*26    Synth Bass 1           */ { 1.00,   20.0,  100.0,  0.10,  50.0,   0.9, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*27    Synth Bass 2           */ { 1.00,   20.0,  100.0,  0.10,  50.0,   0.9, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SAWTOOTH,     0.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Strings                    */ 
        /*28    Violin                 */ { 1.00,   40.0,  500.0,  0.80,  20.0,   0.3, 0.5,     0.0, BW_SQUARE  ,  2400.0, BW_SAWTOOTH,  4800.0, BW_TRIANGLE,  0.01, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     100.0, NDT_TRIANGULAR,   20.0, 4.00 , 1.5 , 0, BW_SIN       ,    0.1, 17.0 , 1.5 , 0, BW_SAWT_INV  },
        /*29    Viola                  */ { 1.80,   60.0,  500.0,  0.80,  40.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,  0.01, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     100.0, NDT_TRIANGULAR,   20.0, 4.00 , 1.5 , 0, BW_SIN       ,    0.2, 13.0 , 1.5 , 0, BW_SAWT_INV  },
        /*2A    Cello                  */ { 1.00,   80.0,  500.0,  0.80,  60.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,  0.01, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     100.0, NDT_TRIANGULAR,   20.0, 4.00 , 1.5 , 0, BW_SIN       ,    0.2, 13.0 , 1.5 , 0, BW_SAWT_INV  },
        /*2B    Double bass            */ { 1.80,  100.0,  500.0,  0.80, 100.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,  0.01, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     100.0, NDT_TRIANGULAR,   20.0, 4.00 , 1.5 , 0, BW_SIN       ,    0.3, 13.0 , 1.5 , 0, BW_SAWT_INV  },
        /*2C    Tremolo Strings        */ { 1.80,   20.0,  500.0,  0.80,  20.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,  0.01, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     100.0, NDT_TRIANGULAR,  100.0, 4.00 , 1.0 , 1, BW_SQUARE    ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*2D    Pizzicato Strings      */ { 1.00,   20.0,   70.0,  0.10,  20.0,   0.8, 1.0,     0.0, BW_TRIANGLE,  1200.0, BW_SIN     ,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,       5.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*2E    Orchestral Harp        */ { 1.00,   50.0,  500.0,  0.50,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      60.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*2F    Timpani                */ { 1.00,   50.0,  500.0,  0.50,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      60.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Ensemble                   */ 
        /*30    String Ensemble 1      */ { 0.80,   40.0,  500.0,  0.80,  40.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,   0.1, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     200.0, NDT_TRIANGULAR,   20.0, 8.00 , 1.5 , 0, BW_SIN       ,    0.3, 12.0 , 1.5 , 0, BW_SIN       },
        /*31    String Ensemble 2      */ { 0.80,   40.0,  500.0,  0.80,  40.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,   0.1, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     200.0, NDT_TRIANGULAR,   20.0, 8.00 , 1.5 , 0, BW_SIN       ,    0.3, 12.0 , 1.5 , 0, BW_SIN       },
        /*32    Synth Strings 1        */ { 0.80,   40.0,  500.0,  0.80,  40.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,   0.1, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     200.0, NDT_TRIANGULAR,   20.0, 8.00 , 1.5 , 0, BW_SIN       ,    0.3, 12.0 , 1.5 , 0, BW_SIN       },
        /*33    Synth Strings 2        */ { 0.80,   40.0,  500.0,  0.80,  40.0,   0.5, 0.6,     0.0, BW_SINSAWx2,  2400.0, BW_SAWTOOTH,  4800.0, BW_SAWTOOTH,   0.1, NCT_PINK , 117.0, 167.0, 197.0,  0.20, 0.10, 0.05,     200.0, NDT_TRIANGULAR,   20.0, 8.00 , 1.5 , 0, BW_SIN       ,    0.3, 12.0 , 1.5 , 0, BW_SIN       },
        /*34    Voice Aahs             */ { 1.00,   50.0,  500.0,  0.50,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      60.0, NDT_COS4ThPOW ,   20.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*35    Voice Oohs             */ { 1.00,   50.0,  500.0,  0.50,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      60.0, NDT_COS4ThPOW ,   20.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*36    Synth Voice            */ { 1.00,   50.0,  500.0,  0.50,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      60.0, NDT_COS4ThPOW ,   20.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*37    Orchestra Hit          */ { 1.00,   50.0,  500.0,  0.50,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      60.0, NDT_COS4ThPOW ,   20.0, 4.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Brass                      */ 
        /*38    Trumpet                */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*39    Trombone               */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*3A    Tuba                   */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*3B    Muted Trumpet          */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*3C    French horn            */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*3D    Brass Section          */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*3E    Synth Brass 1          */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*3F    Synth Brass 2          */ { 0.35,  100.0,   50.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Reed                       */ 
        /*40    Soprano Sax            */ { 0.35,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*41    Alto Sax               */ { 0.35,   50.0,  500.0,  1.00,  50.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*42    Tenor Sax              */ { 0.35,   50.0,  500.0,  1.00,  50.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*43    Baritone Sax           */ { 0.35,  100.0,  500.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*44    Oboe                   */ { 0.35,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*45    English Horn           */ { 0.35,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*46    Bassoon                */ { 0.35,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*47    Clarinet               */ { 0.35,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      30.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Pipe                       */ 
        /*48    Piccolo                */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.1, NCT_WHITE, 113.0, 147.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*49    Flute                  */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.1, NCT_PINK , 113.0, 147.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*4A    Recorder               */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.1, NCT_WHITE, 113.0, 147.0, 177.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*4B    Pan Flute              */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.1, NCT_PINK , 113.0, 147.0, 177.0,  0.20, 0.10, 0.05,     100.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*4C    Blown Bottle           */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_PINK ,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*4D    Shakuhachi             */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_PINK ,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*4E    Whistle                */ { 1.00,   20.0,  500.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_PINK ,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*4F    Ocarina                */ { 1.00,  200.0,  100.0,  0.30, 200.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Synth Lead                 */ 
        /*50    Lead 1 (square)        */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*51    Lead 2 (sawtooth)      */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*52    Lead 3 (calliope)      */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*53    Lead 4 (chiff)         */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*54    Lead 5 (charang)       */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*55    Lead 6 (voice)         */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*56    Lead 7 (fifths)        */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*57    Lead 8 (bass + lead)   */ { 1.00,   20.0,   50.0,  1.00,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Synth Pad                  */ 
        /*58    Pad 1 (Fantasia)       */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*59    Pad 2 (warm)           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*5A    Pad 3 (polysynth)      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*5B    Pad 4 (choir)          */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*5C    Pad 5 (bowed)          */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*5D    Pad 6 (metallic)       */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*5E    Pad 7 (halo)           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*5F    Pad 8 (sweep)          */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Synth Effects              */ 
        /*60    FX 1 (rain)            */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*61    FX 2 (soundtrack)      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*62    FX 3 (crystal)         */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*63    FX 4 (atmosphere)      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*64    FX 5 (brightness)      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*65    FX 6 (goblins)         */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*66    FX 7 (echoes)          */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*67    FX 8 (sci-fi)          */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Ethnic                     */ 
        /*68    Sitar                  */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*69    Banjo                  */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*6A    Shamisen               */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*6B    Koto                   */ { 1.00,   20.0,  120.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*6C    Kalimba                */ { 1.00,   20.0,   50.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*6D    Bagpipe                */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*6E    Fiddle                 */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*6F    Shanai                 */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*70    Tinkle Bell            */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*71    Agogo                  */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*72    Steel Drums            */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*73    Woodblock              */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*74    Taiko Drum             */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,      20.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*75    Melodic Tom            */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*76    Synth Drum             */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*77    Reverse Cymbal         */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Sound effects              */ 
        /*78    Guitar Fret Noise      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*79    Breath Noise           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*7A    Seashore               */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*7B    Bird Tweet             */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*7C    Telephone Ring         */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*7D    Helicopter             */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*7E    Applause               */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*7F    Gunshot                */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SAWTOOTH,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },

        /* --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

        /*  Percussive                 */ 
        /*80    note35 Bass Drum 2     */ { 0.80,    5.0,  120.0,  0.00,  20.0,   0.6, 0.5, -1400.0, BW_SIN     , -1500.0, BW_SIN     , -1600.0, BW_SIN     ,  0.01, NCT_PINK , 107.0, 127.0, 147.0,  0.20, 0.10, 0.05,     900.0, NDT_COS4ThPOW ,    1.0, 50.0 , 1.5 , 0, BW_SAWTOOTH  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*81    note36 Bass Drum 1     */ { 0.80,    5.0,  120.0,  0.00,  20.0,   0.6, 0.5, -1400.0, BW_SIN     , -1500.0, BW_SIN     , -1600.0, BW_SIN     ,  0.01, NCT_PINK , 107.0, 127.0, 147.0,  0.20, 0.10, 0.05,     900.0, NDT_COS4ThPOW ,    1.0, 50.0 , 1.5 , 0, BW_SAWTOOTH  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*82    note37 Side Stick      */ { 0.80,    1.0,   40.0,  0.00,  20.0,  0.55,0.60,     0.0, BW_SINSAWx2,   200.0, BW_SAWTOOTH,   400.0, BW_TRIANGLE,  0.05, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    3600.0, NDT_FLAT      ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*83    note38 Snare Drum 1    */ { 0.80,    5.0,   40.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,   700.0, BW_SQUARE  ,   0.1, NCT_WHITE,  47.0,  77.0,  97.0,  0.20, 0.15, 0.10,    2400.0, NDT_COS4ThPOW ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*84    note39 Hand Clap       */ { 0.80,    1.0,   30.0,  0.00,  20.0,   0.5, 0.5,     0.0, BW_TRIANGLE,   200.0, BW_SINSAWx2,   400.0, BW_SINSAWx2,  0.10, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    7200.0, NDT_FLAT      ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*85    note40 Snare Drum 2    */ { 0.80,    5.0,   40.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,   700.0, BW_SQUARE  ,   0.1, NCT_WHITE,  47.0,  77.0,  97.0,  0.20, 0.15, 0.10,    2400.0, NDT_COS4ThPOW ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*86    note41 Low Tom 2       */ { 1.00,    5.0,   50.0,  0.01, 100.0,   0.4, 0.5,     0.0, BW_SQUARE  ,  1200.0, BW_SAWTOOTH, -1200.0, BW_SAWTOOTH,   0.2, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     800.0, NDT_FLAT      ,  120.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*87    note42 Closed Hi-hat   */ { 1.00,    3.0,   50.0,  0.00,  20.0,   0.4, 0.5,   120.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   150.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    7200.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*88    note43 Low Tom 1       */ { 1.00,    5.0,   50.0,  0.01, 100.0,   0.4, 0.5,     0.0, BW_SAWTOOTH,   190.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   0.2, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    2400.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*89    note44 Pedal Hi-hat    */ { 1.00,    3.0,   40.0,  0.00,  20.0,   0.4, 0.5,   120.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   150.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    7200.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*8A    note45 Mid Tom 2       */ { 1.00,    5.0,   50.0,  0.00, 100.0,   0.4, 0.5,     0.0, BW_SAWTOOTH,   190.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   0.2, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    2400.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*8B    note46 Open Hi-hat     */ { 1.00,    3.0,   20.0,  0.05, 100.0,   0.4, 0.5,   120.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   150.0, BW_SAWTOOTH,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    7200.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*8C    note47 Mid Tom 1       */ { 1.00,    5.0,   50.0,  0.00, 100.0,   0.4, 0.5,     0.0, BW_SAWTOOTH,   190.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   0.2, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    2400.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*8D    note48 High Tom 2      */ { 1.00,    5.0,   50.0,  0.00, 100.0,   0.4, 0.5,     0.0, BW_SAWTOOTH,   190.0, BW_SAWTOOTH,    90.0, BW_SAWTOOTH,   0.2, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    2400.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*8E    note49 Crash Cymbal 1  */ { 0.50,    1.0,   50.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SINSAWx2,  4800.0, BW_SAWTOOTH,  6000.0, BW_SAWTOOTH,   0.3, NCT_WHITE, 137.0, 157.0, 177.0,  0.20, 0.15, 0.10,    7200.0, NDT_FLAT      ,    1.0,200.0 , 1.5 , 0, BW_SAWT_INV  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*8F    note50 High Tom 1      */ { 1.00,    5.0,   50.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SQUARE  ,   190.0, BW_SINSAWx2,    90.0, BW_SAWTOOTH,   0.2, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    2400.0, NDT_FLAT      ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*90    note51 Ride Cymbal 1   */ { 0.50,    1.0,   40.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SINSAWx2,  4800.0, BW_SAWTOOTH,  6000.0, BW_SAWTOOTH,   0.3, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    7200.0, NDT_FLAT      ,    0.0,  0.0 , 1.5 , 0, BW_SAWT_INV  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*91    note52 Chinese Cymbal  */ { 0.50,    1.0,   50.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SINSAWx2,  4800.0, BW_SAWTOOTH,  6000.0, BW_SAWTOOTH,   0.3, NCT_WHITE, 137.0, 157.0, 177.0,  0.20, 0.15, 0.10,    7200.0, NDT_FLAT      ,    1.0,200.0 , 1.5 , 0, BW_SAWT_INV  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*92    note53 Ride Bell       */ { 1.00,    5.0,   90.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*93    note54 Tambourine      */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*94    note55 Splash Cymbal   */ { 1.00,    3.0,   40.0,  0.00, 100.0,   0.4, 0.5,     0.0, BW_SAWTOOTH,  1200.0, BW_SAWTOOTH,  2400.0, BW_SAWTOOTH,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 20.0 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*95    note56 Cowbell         */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*96    note57 Crash Cymbal 2  */ { 0.50,    1.0,   50.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SINSAWx2,  4800.0, BW_SAWTOOTH,  6000.0, BW_SAWTOOTH,   0.3, NCT_WHITE, 137.0, 157.0, 177.0,  0.20, 0.15, 0.10,    7200.0, NDT_FLAT      ,    1.0,200.0 , 1.5 , 0, BW_SAWT_INV  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*97    note58 Vibra Slap      */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*98    note59 Ride Cymbal 2   */ { 0.50,    1.0,   40.0,  0.00,  20.0,   0.4, 0.5,     0.0, BW_SINSAWx2,  4800.0, BW_SAWTOOTH,  6000.0, BW_SAWTOOTH,   0.3, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,    7200.0, NDT_FLAT      ,    0.0,  0.0 , 1.5 , 0, BW_SAWT_INV  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*99    note60 High Bongo      */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*9A    note61 Low Bongo       */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*9B    note62 Mute High Conga */ { 1.00,    5.0,   30.0,  0.00,  20.0,   0.5, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*9C    note63 Open High Conga */ { 1.00,    5.0,   80.0,  0.00,  20.0,   0.5, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,   20.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*9D    note64 Low Conga       */ { 1.00,    5.0,   50.0,  0.00,  20.0,   0.5, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*9E    note65 High Timbale    */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*9F    note66 Low Timbale     */ { 1.00,    5.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*A0    note67 High Agogo      */ { 1.00,   20.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A1    note68 Low Agogo       */ { 1.00,   20.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A2    note69 Cabasa          */ { 1.00,   20.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A3    note70 Maracas         */ { 1.00,   20.0,   50.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A4    note71 Short Whistle   */ { 1.00,   50.0,  100.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A5    note72 Long Whistle    */ { 1.00,   90.0,  100.0,  1.00, 100.0,   1.0, 1.0,     0.0, BW_SIN     ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A6    note73 Short Guiro     */ { 1.00,   20.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A7    note74 Long Guiro      */ { 1.00,   20.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*A8    note75 Claves          */ { 1.00,   20.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*A9    note76 High Wood Block */ { 1.00,    5.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*AA    note77 Low Wood Block  */ { 1.00,    5.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*AB    note78 Mute Cuica      */ { 1.00,    5.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*AC    note79 Open Cuica      */ { 1.00,    5.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_TRIANGLE,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,    1200.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*AD    note80 Mute Triangle   */ { 1.00,    5.0,   80.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_SIN     ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,     600.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*AE    note81 Open Triangle   */ { 1.00,    5.0,  200.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_SIN     ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,  19.0,  37.0,  47.0,  0.20, 0.10, 0.05,     600.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*AF                           */ { 1.00,   20.0,  100.0,  0.00,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*B0                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B1                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B2                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B3                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B4                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B5                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B6                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B7                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*B8                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*B9                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*BA                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*BB                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*BC                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*BD                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*BE                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*BF                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*C0                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C1                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C2                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C3                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C4                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C5                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C6                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C7                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*C8                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*C9                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*CA                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*CB                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*CC                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*CD                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*CE                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*CF                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*D0                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D1                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D2                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D3                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D4                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D5                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D6                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D7                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*D8                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*D9                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*DA                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*DB                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*DC                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*DD                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*DE                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*DF                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*E0                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E1                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E2                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E3                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E4                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E5                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E6                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E7                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*E8                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*E9                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*EA                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*EB                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*EC                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*ED                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*EE                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*EF                           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*F0    prog70 Tinkle Bell     */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F1    prog71 Agogo           */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F2    prog72 Steel Drums     */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F3    prog73 Woodblock       */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F4    prog74 Taiko Drum      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F5    prog75 Melodic Tom     */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F6    prog76 Synth Drum      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F7    prog77 Reverse Cymbal  */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*  Percussive                 */ 
        /*F8    prog78 Gu. Fret Noise  */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*F9    prog79 Breath Noise    */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*FA    prog7A Seashore        */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*FB    prog7B Bird Tweet      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*FC    prog7C Telephone Ring  */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*FD    prog7D Helicopter      */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*FE    prog7E Applause        */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  },
        /*FF    prog7F Gunshot         */ { 1.00,   20.0,  500.0,  0.10,  20.0,   1.0, 1.0,     0.0, BW_SQUARE  ,     0.0, BW_SINSAWx2,     0.0, BW_SINSAWx2,   0.0, NCT_WHITE,   0.0,   0.0,   0.0,  0.00, 0.00, 0.00,       0.0, NDT_COS4ThPOW ,    0.0, 0.00 , 1.5 , 0, BW_TRIANGLE  ,    0.0,  0.0 , 1.5 , 0, BW_TRIANGLE  } 
    }
};


const std::array<Percussion, Sequencer::numPercussions> defaultPercussions = {
    {
        // Percussive
        {128,    24/* C1*/}, // note00(0x00)
        {128,    25/*C#1*/}, // note01(0x01)
        {128,    26/* D1*/}, // note02(0x02)
        {128,    27/*D#1*/}, // note03(0x03)
        {128,    28/* E1*/}, // note04(0x04)
        {128,    29/* F1*/}, // note05(0x05)
        {128,    30/*F#1*/}, // note06(0x06)
        {128,    31/* G1*/}, // note07(0x07)
        // Percussive
        {128,    32/*G#1*/}, // note08(0x08)
        {128,    33/* A1*/}, // note09(0x09)
        {128,    34/*A#1*/}, // note10(0x0A)
        {128,    35/* B1*/}, // note11(0x0B)
        {128,    36/* C2*/}, // note12(0x0C)
        {128,    37/*C#2*/}, // note13(0x0D)
        {128,    38/* D2*/}, // note14(0x0E)
        {128,    39/*D#2*/}, // note15(0x0F)
        // Percussive
        {128,    40/* E2*/}, // note16(0x10)
        {128,    41/* F2*/}, // note17(0x11)
        {128,    42/*F#2*/}, // note18(0x12)
        {128,    43/* G2*/}, // note19(0x13)
        {128,    44/*G#2*/}, // note20(0x14)
        {128,    45/* A2*/}, // note21(0x15)
        {128,    46/*A#2*/}, // note22(0x16)
        {128,    47/* B2*/}, // note23(0x17)
        // Percussive
        {128,    48/* C3*/}, // note24(0x18)
        {128,    49/*C#3*/}, // note25(0x19)
        {128,    50/* D3*/}, // note26(0x1A)
        {128,    51/*D#3*/}, // note27(0x1B)
        {128,    52/* E3*/}, // note28(0x1C)
        {128,    53/* F3*/}, // note29(0x1D)
        {128,    54/*F#3*/}, // note30(0x1E)
        {128,    55/* G3*/}, // note31(0x1F)
        // Percussive
        {128,    32/*G#3*/}, // note32(0x20)
        {128,    33/* A3*/}, // note33(0x21)
        {128,    34/*A#3*/}, // note34(0x22) 
        {128,    60/* C4*/}, // note35(0x23) Bass Drum 2        
        {129,    60/* C4*/}, // note36(0x24) Bass Drum 1        
        {130,   101/* F7*/}, // note37(0x25) Side Stick         
        {131,    69/* A4*/}, // note38(0x26) Snare Drum 1       
        {132,    48/* C3*/}, // note39(0x27) Hand Clap          
        // Percussive
        {133,    69/* A4*/}, // note40(0x28) Snare Drum 2       
        {134,    60/* C4*/}, // note41(0x29) Low Tom 2          
        {135,   105/* A7*/}, // note42(0x2A) Closed Hi-hat      
        {136,    60/* C4*/}, // note43(0x2B) Low Tom 1          
        {137,   105/* A7*/}, // note44(0x2C) Pedal Hi-hat       
        {138,    67/* G4*/}, // note45(0x2D) Mid Tom 2          
        {139,   105/* A7*/}, // note46(0x2E) Open Hi-hat        
        {140,    67/* G4*/}, // note47(0x2F) Mid Tom 1          
        // Percussive
        {141,    74/* D5*/}, // note48(0x30) High Tom 2         
        {142,    69/* A4*/}, // note49(0x31) Crash Cymbal 1     
        {143,    74/* D5*/}, // note50(0x32) High Tom 1         
        {144,    72/* C5*/}, // note51(0x33) Ride Cymbal 1      
        {145,   105/* A7*/}, // note52(0x34) Chinese Cymbal     
        {146,    31       }, // note53(0x35) Ride Bell          
        {147,    31       }, // note54(0x36) Tambourine         
        {148,   105/* A7*/}, // note55(0x37) Splash Cymbal      
        // Percussive
        {149,    31       }, // note56(0x38) Cowbell            
        {150,    69/* A4*/}, // note57(0x39) Crash Cymbal 2     
        {151,    31       }, // note58(0x3A) Vibra Slap         
        {152,    72/* C5*/}, // note59(0x3B) Ride Cymbal 2      
        {153,    74/* D5*/}, // note60(0x3C) High Bongo         
        {154,    60/* C4*/}, // note61(0x3D) Low Bongo          
        {155,    74/* D5*/}, // note62(0x3E) Mute High Conga    
        {156,    74/* D5*/}, // note63(0x3F) Open High Conga    
        // Percussive
        {157,    60/* C4*/}, // note64(0x40) Low Conga          
        {158,    31       }, // note65(0x41) High Timbale       
        {159,    31       }, // note66(0x42) Low Timbale        
        {160,    31       }, // note67(0x43) High Agogo         
        {161,    31       }, // note68(0x44) Low Agogo          
        {162,    31       }, // note69(0x45) Cabasa             
        {163,    31       }, // note70(0x46) Maracas            
        {164,    31       }, // note71(0x47) Short Whistle      
        // Percussive
        {165,    81/* A5*/}, // note72(0x48) Long Whistle       
        {166,    31       }, // note73(0x49) Short Guiro        
        {167,    81       }, // note74(0x4A) Long Guiro         
        {168,    31       }, // note75(0x4B) Claves             
        {169,    31       }, // note76(0x4C) High Wood Block    
        {170,    31       }, // note77(0x4D) Low Wood Block     
        {171,    31       }, // note78(0x4E) Mute Cuica         
        {172,    31       }, // note79(0x4F) Open Cuica         
        // Percussive
        {173,    96/* C7*/}, // note80(0x50) Mute Triangle      
        {174,    96/* C7*/}, // note81(0x51) Open Triangle      
        {128,    31       }, // note82(0x52)
        {128,    31       }, // note83(0x53)
        {128,    31       }, // note84(0x54)
        {128,    31       }, // note85(0x55)
        {128,    31       }, // note86(0x56)
        {128,    31       }, // note87(0x57)
        // Percussive
        {128,    31       }, // note88(0x58)
        {128,    31       }, // note89(0x59)
        {128,    31       }, // note90(0x5A)
        {128,    31       }, // note91(0x5B)
        {128,    31       }, // note92(0x5C)
        {128,    31       }, // note93(0x5D)
        {128,    31       }, // note94(0x5E)
        {128,    31       }, // note95(0x5F)
        // Percussive
        {128,    31       }, // note96(0x60)
        {128,    31       }, // note97(0x61)
        {128,    31       }, // note98(0x62)
        {128,    31       }, // note99(0x63)
        {128,    31       }, // note100(0x64)
        {128,    31       }, // note101(0x65)
        {128,    31       }, // note102(0x66)
        {128,    31       }, // note103(0x67)
        // Percussive
        {128,    31       }, // note104(0x68)
        {128,    31       }, // note105(0x69)
        {128,    31       }, // note106(0x6A)
        {128,    31       }, // note107(0x6B)
        {128,    31       }, // note108(0x6C)
        {128,    31       }, // note109(0x6D)
        {128,    31       }, // note110(0x6E)
        {128,    31       }, // note111(0x6F)
        // Percussive
        {240,    31       }, // note112(0x70)F0    70 Tinkle Bell      
        {241,    31       }, // note113(0x71)F1    71 Agogo            
        {242,    31       }, // note114(0x72)F2    72 Steel Drums      
        {243,    31       }, // note115(0x73)F3    73 Woodblock        
        {244,    31       }, // note116(0x74)F4    74 Taiko Drum       
        {245,    31       }, // note117(0x75)F5    75 Melodic Tom      
        {246,    31       }, // note118(0x76)F6    76 Synth Drum       
        {247,    31       }, // note119(0x77)F7    77 Reverse Cymbal   
        // Percussive
        {248,    31       }, // note120(0x78)F8    78 Guitar Fret Noise
        {249,    31       }, // note121(0x79)F9    79 Breath Noise     
        {250,    31       }, // note122(0x7A)FA    7A Seashore         
        {251,    31       }, // note123(0x7B)FB    7B Bird Tweet       
        {252,    31       }, // note124(0x7C)FC    7C Telephone Ring   
        {253,    31       }, // note125(0x7D)FD    7D Helicopter       
        {254,    31       }, // note126(0x7E)FE    7E Applause         
        {255,    31       }  // note127(0x7F)FF    7F Gunshot          
    }
};

#endif

