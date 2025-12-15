/**************************************************************************/
/*  smfparser.hpp                                                         */
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


#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
#include <memory>
#include <godot_cpp/classes/file_access.hpp>

enum class NState {
    NS_OFF,          //  0
    NS_ON_FOREVER,   //  1

    NS_EMPTY,        //  2
    NS_END,          //  3

    NS_TAIL
};

enum class TState {
    TS_EMPTY,     //  0
    TS_FOUND,     //  1
    TS_SERVED,    //  2

    TS_TAIL
};

struct Note {
    NState state;
    int32_t trackNum;
    int32_t channel;
    int32_t key;
    int32_t velocity;
    int32_t program;
    uint32_t startTick;
    int32_t startTime;
    int32_t tempo;
    bool operator<(const Note& another) const {
        return startTick < another.startTick;
    }
};

class SMFParser {
private:

    // methods for binary data access.
    uint32_t getBytes(uint16_t);
    uint8_t getByte();
    void skipByte(int32_t);
    uint32_t getVarLen();
    std::string getStr(uint16_t);
    
    uint32_t getBytes(uint16_t, uint32_t *);
    uint8_t getByte(uint32_t *);
    void skipByte(int32_t, uint32_t *);
    uint32_t getVarLen(uint32_t *);
    std::string getStr(uint16_t, uint32_t *);
    
    
    // context
    uint32_t numOfTracks = 0;
    uint32_t timeDivision = 0;
    uint32_t formatType = 0;
    
    uint32_t position;
    float unitOfTime;
    uint32_t tempo;

    struct Track {
        TState state;
        std::vector<Note> notes;
        uint32_t tick;
        uint32_t top;
        uint32_t length;
        uint32_t tail;
        uint32_t tempo;
        uint32_t position;
        uint8_t previousEvent = 0;
        int8_t program = 0;
        Note nextNote;
    };
    std::vector<Track> tracks;
    std::vector<Track> tracksPreOnOff; // For preOnOff sequence (independent context)

    struct Tempo {
        uint32_t tick, tempo;
        float time;
        bool operator<(const Tempo& another) const {
            return tick < another.tick;
        }
    };
    std::vector<Tempo> tempos; // For normal sequence
    std::vector<Tempo> temposPreOnOff; // For preOnOff sequence (independent context)
    std::unique_ptr<uint8_t []> binary_data;
    float preOnTime = 0.0f; // Pre-on signal time in milliseconds (0 = disabled)
public:
    size_t filesize = 0;
    SMFParser();
    ~SMFParser();
    bool load(const char*);
    bool load(const godot::String &);
    void unload(void);
    void restart(void);
    Note parse(int32_t, bool forPreOnOff = false);
    void setUnitOfTime(float);
    float getUnitOfTime() const;
    void setPreOnTime(float pTime) { preOnTime = pTime; }
    float getPreOnTime() const { return preOnTime; }
    uint32_t getNumOfTracks() const { return numOfTracks; }
};
