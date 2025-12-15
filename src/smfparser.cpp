/**************************************************************************/
/*  smfparser.cpp                                                         */
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

#include "smfparser.hpp"

#include <godot_cpp/classes/file_access.hpp>
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
#endif // DEBUG_ENABLED

SMFParser::SMFParser() : unitOfTime(60000.0f), position(0), tempo(60) {
}

SMFParser::~SMFParser(){
    tracks.clear();
}


void SMFParser::unload(void) {
    
    position = 0;
    filesize = 0;
    formatType = 0;
    numOfTracks = 0;
    timeDivision = 0;
    binary_data.reset();
    tempos.clear();
    temposPreOnOff.clear(); // Clear temposPreOnOff as well
    tracks.clear();
    
}

bool SMFParser::load(const char *name) {
    // reset previous state in case caller skipped unload
    unload();

    std::ifstream in;
    position = 0;

    in.open(name, std::ios::in | std::ios::binary);
    if (!in.is_open()) return false;

    in.seekg(0, std::ifstream::end);
    filesize = static_cast<size_t>(in.tellg());
    in.seekg(0, std::ifstream::beg);

    { // check Mthd marker
        std::string str = "";
        for (int32_t i = 0; i < 4; ++i) str += in.get();
        if (str.compare("MThd") != 0) return false;
    }
    { // check data length that must be 6
        uint32_t chunkSize = 0;
        for (uint8_t i = 0; i < 4; ++i) chunkSize = (chunkSize << 8) | in.get();
        if (chunkSize != 6) return false;
    }
    { // check formatType that must be 0 or 1
        formatType = 0;
        for (uint8_t i = 0; i < 2; ++i) formatType = (formatType << 8) | in.get();
        if (formatType != 0 && formatType != 1) return false;
    }
    { // get numOfTracks
        numOfTracks = 0;
        for (uint8_t i = 0; i < 2; ++i) numOfTracks = (numOfTracks << 8) | in.get();
        if (formatType == 0 && numOfTracks != 1) return false;
        else if (formatType == 1 && numOfTracks < 1) return false;
        else if (formatType != 0 && formatType != 1) return false;
    }
    { // get timeDivision
        timeDivision = 0;
        for (uint8_t i = 0; i < 2; ++i) timeDivision = (timeDivision << 8) | in.get();
        if (timeDivision & 0x8000) return false; //currently, not supported SMPTE format
    }

    auto data_top = in.tellg();
    binary_data = std::make_unique<uint8_t[]>(filesize);
    in.seekg(0, std::ifstream::beg);
    for (int32_t i = 0; i < filesize; i++) binary_data[i] = in.get();
    in.close();

    position = (uint32_t)data_top;

    tempo = 60; // as default
    tempos.push_back({0, tempo, 0.0});
    temposPreOnOff.push_back({0, tempo, 0.0}); // Initialize temposPreOnOff as well
    for (int32_t i = 0; i < numOfTracks; ++i) {
        std::string str = getStr(4);
        if (str.compare("MTrk") != 0) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
#endif // DEBUG_ENABLED
            return false;
        }
        tracks.push_back(Track());

        tracks.back().length = getBytes(4);
        tracks.back().top = position;
        position += tracks.back().length;
        tracks.back().tail = position;
        tracks.back().tempo = tempo;
        tracks.back().position = tracks.back().top;
        tracks.back().previousEvent = 0;
        tracks.back().program = 0;
        tracks.back().tick = 0;
        tracks.back().state = TState::TS_EMPTY;
    }
    // Initialize tracksPreOnOff from tracks (independent context for preOnOff sequence)
    tracksPreOnOff = tracks;
    return true;
}



bool SMFParser::load(const godot::String &name) {
    // reset previous state in case caller skipped unload
    unload();

    position = 0;
    auto in = godot::FileAccess::open(name, godot::FileAccess::READ);

    if (!in->is_open()) return false;

    filesize = static_cast<size_t>(in->get_length());

    { // check Mthd marker
        std::string str = "";
        for (int32_t i = 0; i < 4; ++i) str += (unsigned char)in->get_8();
        if (str.compare("MThd") != 0) return false;
    }
    { // check data length that must be 6
        uint32_t chunkSize = 0;
        for (uint8_t i = 0; i < 4; ++i) chunkSize = (chunkSize << 8) | (uint32_t)in->get_8();
        if (chunkSize != 6) return false;
    }
    { // check formatType that must be 0 or 1
        formatType = 0;
        for (uint8_t i = 0; i < 2; ++i) formatType = (formatType << 8) | (uint32_t)in->get_8();
        if (formatType != 0 && formatType != 1) return false;
    }
    { // get numOfTracks
        numOfTracks = 0;
        for (uint8_t i = 0; i < 2; ++i) numOfTracks = (numOfTracks << 8) | (uint32_t)in->get_8();
        if (formatType == 0 && numOfTracks != 1) return false;
        else if (formatType == 1 && numOfTracks < 1) return false;
        else if (formatType != 0 && formatType != 1) return false;
    }
    { // get timeDivision
        timeDivision = 0;
        for (uint8_t i = 0; i < 2; ++i) timeDivision = (timeDivision << 8) | (uint32_t)in->get_8();
        if (timeDivision & 0x8000) return false; //currently, not supported SMPTE format
    }

    auto data_top = in->get_position();
    binary_data = std::make_unique<uint8_t[]>(filesize);
    in->seek(0);
    for (int32_t i = 0; i < filesize; i++) binary_data[i] = in->get_8();
    in->close();

    position = (uint32_t)data_top;

    tempo = 60; // as default
    tempos.push_back({0, tempo, 0.0});
    temposPreOnOff.push_back({0, tempo, 0.0}); // Initialize temposPreOnOff as well
    for (int32_t i = 0; i < numOfTracks; ++i) {
        std::string str = getStr(4);
        if (str.compare("MTrk") != 0) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
#endif // DEBUG_ENABLED
            return false;
        }
        tracks.push_back(Track());

        tracks.back().length = getBytes(4);
        tracks.back().top = position;
        position += tracks.back().length;
        tracks.back().tail = position;
        tracks.back().tempo = tempo;
        tracks.back().position = tracks.back().top;
        tracks.back().previousEvent = 0;
        tracks.back().program = 0;
        tracks.back().tick = 0;
        tracks.back().state = TState::TS_EMPTY;
    }
    // Initialize tracksPreOnOff from tracks (independent context for preOnOff sequence)
    tracksPreOnOff = tracks;
    return true;
}



void SMFParser::restart(void) {
    tempos.clear();
    tempos.push_back({0, tempo, 0.0});
    temposPreOnOff.clear();
    temposPreOnOff.push_back({0, tempo, 0.0}); // Reset temposPreOnOff as well
    for (int32_t i = 0; i < numOfTracks; ++i) {
        tracks[i].tempo = tempo;
        tracks[i].position = tracks[i].top;
        tracks[i].previousEvent = 0;
        tracks[i].program = 0;
        tracks[i].tick = 0;
        tracks[i].state = TState::TS_EMPTY;
        tracks[i].nextNote.startTick = 0;
    }
    // Restart tracksPreOnOff as well
    for (int32_t i = 0; i < numOfTracks; ++i) {
        tracksPreOnOff[i].tempo = tempo;
        tracksPreOnOff[i].position = tracksPreOnOff[i].top;
        tracksPreOnOff[i].previousEvent = 0;
        tracksPreOnOff[i].program = 0;
        tracksPreOnOff[i].tick = 0;
        tracksPreOnOff[i].state = TState::TS_EMPTY;
        tracksPreOnOff[i].nextNote.startTick = 0;
    }
}


Note SMFParser::parse(int32_t till, bool forPreOnOff) {
    Note retNote;
    retNote.state = NState::NS_EMPTY;
    
    // Early exit if no MIDI file is loaded
    if (numOfTracks == 0) {
        retNote.state = NState::NS_EMPTY;
        return retNote;
    }
    
    // Select tracks to use (preOnOff sequence or normal sequence)
    std::vector<Track>& activeTracks = forPreOnOff ? tracksPreOnOff : tracks;
    
    int32_t numServed = 0;
    for (uint32_t i = 0; i < numOfTracks; ++i) {
        if (activeTracks[i].position >= activeTracks[i].tail){
            activeTracks[i].state = TState::TS_SERVED;
            activeTracks[i].nextNote.startTick = (uint32_t)(-1);
        }
        while(activeTracks[i].state == TState::TS_EMPTY && activeTracks[i].position < activeTracks[i].tail) {
            activeTracks[i].tick += getVarLen(&(activeTracks[i].position));
            uint8_t event = getByte(&(activeTracks[i].position));

            if (event < 0x80) {
                event = activeTracks[i].previousEvent;
                skipByte(-1, &(activeTracks[i].position));
                if(event == 0) continue; // SysEx event
            } else 
                activeTracks[i].previousEvent = ((event & 0xf0) != 0xf0) ? event : 0;
            uint8_t channel = event & 0xf;

            switch(event & 0xf0) {
                case 0x80: // note off
                    {
                        int8_t key = getByte(&(activeTracks[i].position));
                        int8_t velocity = getByte(&(activeTracks[i].position));

                        activeTracks[i].nextNote = {
                            .state        = NState::NS_OFF,
                            .trackNum     = (int32_t)i,
                            .channel      = (int32_t)channel,
                            .key          = (int32_t)key,
                            .velocity     = (int32_t)velocity,
                            .program      = (int32_t)activeTracks[i].program,
                            .startTick    = activeTracks[i].tick,
                            .startTime    = 0,
                            .tempo        = 0
                        };
                        activeTracks[i].state = TState::TS_FOUND;
                    }
                    break;

                case 0x90: // note on
                    {
                        int8_t key = getByte(&(activeTracks[i].position));
                        int8_t velocity = getByte(&(activeTracks[i].position));

                        NState state = NState::NS_OFF;
                        if (velocity != 0) state = NState::NS_ON_FOREVER;

                        activeTracks[i].nextNote = {
                            .state        = state,
                            .trackNum     = (int32_t)i,
                            .channel      = (int32_t)channel,
                            .key          = (int32_t)key,
                            .velocity     = (int32_t)velocity,
                            .program      = (int32_t)activeTracks[i].program,
                            .startTick    = activeTracks[i].tick,
                            .startTime    = 0,
                            .tempo        = 0
                        };
                        activeTracks[i].state = TState::TS_FOUND;
                    }
                    break;

                case 0xa0: //Polyphonic Pressure (ignored)
                    {
                        int8_t key = getByte(&(activeTracks[i].position));
                        int8_t pressure = getByte(&(activeTracks[i].position));   
//                        skipByte(2, &(activeTracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                        // debug print removed
#endif // DEBUG_ENABLED
                        
                    }
                    break;

                case 0xb0: // Controller (ignored)
                    {
                        int8_t controller = getByte(&(activeTracks[i].position));
                        int8_t value = getByte(&(activeTracks[i].position));  
//#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
//                        debug print removed
//#endif // DEBUG_ENABLED
                    }
                    break;

                case 0xc0: // program change
                    {
                        activeTracks[i].program = getByte(&(activeTracks[i].position));
                    }
                    break;

                case 0xd0: // Channel Pressure (ignored)
                    {
                        int8_t pressure = getByte(&(activeTracks[i].position));
//                        skipByte(1, &(activeTracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                        // debug print removed
#endif // DEBUG_ENABLED
                    }
                    break;

                case 0xe0: // pitch bend (currently, ignored)
                    {
                        int8_t lsb = getByte(&(activeTracks[i].position)); // note that it is only little endian.
                        int8_t msb = getByte(&(activeTracks[i].position));
                        int16_t pitchBend = (int16_t)lsb + ((int16_t)msb)*256; // msb
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                        // debug print removed
#endif // DEBUG_ENABLED
                    }
                    break;

                case 0xf0: // SysEx event
                    {
                        if (event == 0xf0) {// System Exclusive Message Begin
                            skipByte(getVarLen(&(activeTracks[i].position)));
                        }
                        else if (event == 0xf7) { // System Exclusive Message End
                            skipByte(getVarLen(&(activeTracks[i].position)));
                        }
                        else if (event == 0xff) {
                            uint8_t type = getByte(&(activeTracks[i].position));
                            uint32_t value = getVarLen(&(activeTracks[i].position));

                            switch(type) {
                                case 0x00: // MetaSequence
                                    {
                                        skipByte(2, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x01: // MetaText
                                    {
                                        std::string str = getStr(value, &(activeTracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        // debug print removed
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x02: // MetaCopyright
                                    {
                                        std::string str = getStr(value, &(activeTracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        // debug print removed
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x03: // MetaTrackName
                                    {
                                        std::string str = getStr(value, &(activeTracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        // debug print removed
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x04: // MetaInstrumentName
                                    {
                                        std::string str = getStr(value, &(activeTracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        // debug print removed
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x05: // MetaLyrics
                                    {
                                        skipByte(value, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x06: // MetaMarker
                                    {
                                        skipByte(value, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x07: // MetaCuePoint
                                    {
                                        skipByte(value, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x20: // MetaChannelPrefix
                                    {
                                        skipByte(1, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x2f: // END OF TRACK
                                    {
                                        activeTracks[i].state = TState::TS_SERVED;
                                    }
                                    break;

                                case 0x51: // MetaSetTempo
                                    {
                                        uint32_t metaSetTempo = getBytes(3, &(activeTracks[i].position));
                                        const uint32_t BPM = 60000000 / metaSetTempo;
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        // debug print removed
#endif // DEBUG_ENABLED
                                        // Select appropriate tempos array based on forPreOnOff
                                        std::vector<Tempo>& activeTempos = forPreOnOff ? temposPreOnOff : tempos;
                                        if(activeTracks[i].tick == 0) {
                                            activeTempos[0].tempo = BPM;
                                        }
                                        else {
                                            activeTempos.push_back({activeTracks[i].tick, BPM});
                                            std::sort(activeTempos.begin(), activeTempos.end());
                                    
                                            float elapsedTicks = 0.0f;
                                            float time = 0.0f;
                                            for (int32_t j = 1; j < activeTempos.size(); ++j) {
                                                time += (unitOfTime/activeTempos[j-1].tempo)*((activeTempos[j].tick-elapsedTicks)/timeDivision);
                                                elapsedTicks = activeTempos[j].tick;
                                                activeTempos[j].time = time;
                                            }
                                        }
                                    }
                                    break;

                                case 0x54: // MetaSMPTEOffset
                                    {
                                        skipByte(5, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x58: // MetaTimeSignature
                                    {
                                        skipByte(4, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x59: // MetaKeySignature
                                    {
                                        skipByte(2, &(activeTracks[i].position));
                                    }
                                    break;

                                case 0x7f: // MetaSequencerSpecific
                                    {
                                        skipByte(value, &(activeTracks[i].position));
                                    }
                                    break;

                                default:
                                    break;
                            }
                        }
                    }
                    break;

                default:break;
            }
        }
        if (activeTracks[i].state == TState::TS_SERVED) numServed += 1;
    }

    if (numServed == numOfTracks){
        retNote.state = NState::NS_END;
    }
    else {
        int32_t minTick = 0x7fffffff; // set very large number
        int32_t j = 0;
        for (int32_t i = 0; i < numOfTracks; ++i) {
            if(activeTracks[i].nextNote.startTick < minTick) {
                minTick = activeTracks[i].nextNote.startTick;
                j = i;
            }
        }

        // Select appropriate tempos array based on forPreOnOff
        std::vector<Tempo>& activeTempos = forPreOnOff ? temposPreOnOff : tempos;
        auto it = std::find_if(activeTempos.begin(), activeTempos.end(), [&](const Tempo &bpm) {
            return (activeTracks[j].nextNote.startTick < bpm.tick);
        }) - 1;
        if (it != activeTempos.end()) {
            const float TEMPO = ((unitOfTime / it->tempo) /  timeDivision);
            activeTracks[j].nextNote.tempo = (int32_t)it->tempo;
            int32_t startTimeBeforeOffset = (int32_t)(it->time + ((activeTracks[j].nextNote.startTick - it->tick) * TEMPO));
            activeTracks[j].nextNote.startTime = startTimeBeforeOffset;
            // For normal sequence (not preOnOff), add preOnTime offset
            // This makes the note's startTime relative to the delayed playback start
            if (!forPreOnOff && preOnTime > 0.0f) {
                activeTracks[j].nextNote.startTime += (int32_t)preOnTime;
            }
        }
        // For normal sequence, the note's startTime already includes preOnTime offset
        // So we can use till directly (no need to subtract preOnTime)
        if(activeTracks[j].nextNote.startTime < till){
            activeTracks[j].state = TState::TS_EMPTY;
            retNote = activeTracks[j].nextNote;
        }
    }
    return retNote;
}


void SMFParser::setUnitOfTime(float unit) {
    unitOfTime = unit;
}

float SMFParser::getUnitOfTime() const {
    return unitOfTime;
}


uint32_t SMFParser::getBytes(uint16_t length) {
    uint32_t value = 0;
    for (uint8_t i = 0; i < length; ++i)
        value = (value << 8) | getByte();
    return value;
}


uint32_t SMFParser::getBytes(uint16_t length, uint32_t *pos) {
    uint32_t value = 0;
    for (uint8_t i = 0; i < length; ++i)
        value = (value << 8) | getByte(pos);
    return value;
}


uint8_t SMFParser::getByte() {
    uint8_t value = binary_data[position];
    ++position;
    return value;
}


uint8_t SMFParser::getByte(uint32_t *pos) {
    uint8_t value = binary_data[*pos];
    ++(*pos);
    return value;
}


void SMFParser::skipByte(int32_t length) {
    position += length;
}


void SMFParser::skipByte(int32_t length, uint32_t *pos) {
    (*pos) += length;
}


std::string SMFParser::getStr(uint16_t length) {
    std::string str = "";
    for (uint16_t i = 0; i < length; ++i)
        str += getByte();
    return str;
}


std::string SMFParser::getStr(uint16_t length, uint32_t *pos) {
    std::string str = "";
    for (uint16_t i = 0; i < length; ++i)
        str += getByte(pos);
    return str;
}


uint32_t SMFParser::getVarLen(uint32_t *pos) {
    uint32_t value = getByte(pos);
    if (value & 0x80) {
        value &= 0x7f;
         uint8_t byteRead = 0;
         do {
            byteRead = getByte(pos);
            value = (value << 7) | (byteRead & 0x7f);
         } while (byteRead & 0x80);
    }
    return value;
}

