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
#include <godot_cpp/variant/utility_functions.hpp> // for "UtilityFunctions::print()".
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
    tracks.clear();
    
}

bool SMFParser::load(const char *name) {
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
    for (int32_t i = 0; i < numOfTracks; ++i) {
        std::string str = getStr(4);
        if (str.compare("MTrk") != 0) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            godot::UtilityFunctions::print("Error: MTrk mark not found.");
            godot::UtilityFunctions::print("    Track", i, " MTrk header: ",  str.c_str());
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
    return true;
}



bool SMFParser::load(const godot::String &name) {
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
    for (int32_t i = 0; i < numOfTracks; ++i) {
        std::string str = getStr(4);
        if (str.compare("MTrk") != 0) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
            godot::UtilityFunctions::print("Error: MTrk mark not found.");
            godot::UtilityFunctions::print("    Track", i, " MTrk header: ",  str.c_str());
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
    return true;
}



void SMFParser::restart(void) {
    tempos.clear();
    tempos.push_back({0, tempo, 0.0});
    for (int32_t i = 0; i < numOfTracks; ++i) {
        tracks[i].tempo = tempo;
        tracks[i].position = tracks[i].top;
        tracks[i].previousEvent = 0;
        tracks[i].program = 0;
        tracks[i].tick = 0;
        tracks[i].state = TState::TS_EMPTY;
        tracks[i].nextNote.startTick = 0;
    }
}


Note SMFParser::parse(int32_t till) {
    Note retNote;
    retNote.state = NState::NS_EMPTY;
    int32_t numServed = 0;
    for (uint32_t i = 0; i < numOfTracks; ++i) {
        if (tracks[i].position >= tracks[i].tail){
            tracks[i].state = TState::TS_SERVED;
            tracks[i].nextNote.startTick = (uint32_t)(-1);
        }
        while(tracks[i].state == TState::TS_EMPTY && tracks[i].position < tracks[i].tail) {
            tracks[i].tick += getVarLen(&(tracks[i].position));
            uint8_t event = getByte(&(tracks[i].position));

            if (event < 0x80) {
                event = tracks[i].previousEvent;
                skipByte(-1, &(tracks[i].position));
                if(event == 0) continue; // SysEx event
            } else 
                tracks[i].previousEvent = ((event & 0xf0) != 0xf0) ? event : 0;
            uint8_t channel = event & 0xf;

            switch(event & 0xf0) {
                case 0x80: // note off
                    {
                        int8_t key = getByte(&(tracks[i].position));
                        int8_t velocity = getByte(&(tracks[i].position));

                        tracks[i].nextNote = {
                            .state        = NState::NS_OFF,
                            .trackNum     = (int32_t)i,
                            .channel      = (int32_t)channel,
                            .key          = (int32_t)key,
                            .velocity     = (int32_t)velocity,
                            .program      = (int32_t)tracks[i].program,
                            .startTick    = tracks[i].tick,
                            .startTime    = 0,
                            .tempo        = 0
                        };
                        tracks[i].state = TState::TS_FOUND;
                    }
                    break;

                case 0x90: // note on
                    {
                        int8_t key = getByte(&(tracks[i].position));
                        int8_t velocity = getByte(&(tracks[i].position));

                        NState state = NState::NS_OFF;
                        if (velocity != 0) state = NState::NS_ON_FOREVER;

                        tracks[i].nextNote = {
                            .state        = state,
                            .trackNum     = (int32_t)i,
                            .channel      = (int32_t)channel,
                            .key          = (int32_t)key,
                            .velocity     = (int32_t)velocity,
                            .program      = (int32_t)tracks[i].program,
                            .startTick    = tracks[i].tick,
                            .startTime    = 0,
                            .tempo        = 0
                        };
                        tracks[i].state = TState::TS_FOUND;
                    }
                    break;

                case 0xa0: //Polyphonic Pressure (ignored)
                    {
                        int8_t key = getByte(&(tracks[i].position));
                        int8_t pressure = getByte(&(tracks[i].position));   
//                        skipByte(2, &(tracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                        godot::UtilityFunctions::print("Polyphonic Pressure: ", key, " ", pressure, " ch=", channel);
#endif // DEBUG_ENABLED
                        
                    }
                    break;

                case 0xb0: // Controller (ignored)
                    {
                        int8_t controller = getByte(&(tracks[i].position));
                        int8_t value = getByte(&(tracks[i].position));  
//#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
//                        godot::UtilityFunctions::print("Controller: ",controller, " ", value, " ch=", channel);
//#endif // DEBUG_ENABLED
                    }
                    break;

                case 0xc0: // program change
                    {
                        tracks[i].program = getByte(&(tracks[i].position));
                    }
                    break;

                case 0xd0: // Channel Pressure (ignored)
                    {
                        int8_t pressure = getByte(&(tracks[i].position));
//                        skipByte(1, &(tracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                        godot::UtilityFunctions::print("Channel Pressure: ", pressure, " ch=", channel);
#endif // DEBUG_ENABLED
                    }
                    break;

                case 0xe0: // pitch bend (currently, ignored)
                    {
                        int8_t lsb = getByte(&(tracks[i].position)); // note that it is only little endian.
                        int8_t msb = getByte(&(tracks[i].position));
                        int16_t pitchBend = (int16_t)lsb + ((int16_t)msb)*256; // msb
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                        godot::UtilityFunctions::print("Pitch bend: ",pitchBend, " ch=", channel);
#endif // DEBUG_ENABLED
                    }
                    break;

                case 0xf0: // SysEx event
                    {
                        if (event == 0xf0) {// System Exclusive Message Begin
                            skipByte(getVarLen(&(tracks[i].position)));
                        }
                        else if (event == 0xf7) { // System Exclusive Message End
                            skipByte(getVarLen(&(tracks[i].position)));
                        }
                        else if (event == 0xff) {
                            uint8_t type = getByte(&(tracks[i].position));
                            uint32_t value = getVarLen(&(tracks[i].position));

                            switch(type) {
                                case 0x00: // MetaSequence
                                    {
                                        skipByte(2, &(tracks[i].position));
                                    }
                                    break;

                                case 0x01: // MetaText
                                    {
                                        std::string str = getStr(value, &(tracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        godot::UtilityFunctions::print("Track", i, " MetaText: ", str.c_str());
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x02: // MetaCopyright
                                    {
                                        std::string str = getStr(value, &(tracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        godot::UtilityFunctions::print("Track", i, " MetaCopyright: ", str.c_str());
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x03: // MetaTrackName
                                    {
                                        std::string str = getStr(value, &(tracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        godot::UtilityFunctions::print("Track", i, " MetaTrackName: ", str.c_str());
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x04: // MetaInstrumentName
                                    {
                                        std::string str = getStr(value, &(tracks[i].position));
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        godot::UtilityFunctions::print("Track", i, " MetaInstrumentName: ", str.c_str());
#endif // DEBUG_ENABLED
                                    }
                                    break;

                                case 0x05: // MetaLyrics
                                    {
                                        skipByte(value, &(tracks[i].position));
                                    }
                                    break;

                                case 0x06: // MetaMarker
                                    {
                                        skipByte(value, &(tracks[i].position));
                                    }
                                    break;

                                case 0x07: // MetaCuePoint
                                    {
                                        skipByte(value, &(tracks[i].position));
                                    }
                                    break;

                                case 0x20: // MetaChannelPrefix
                                    {
                                        skipByte(1, &(tracks[i].position));
                                    }
                                    break;

                                case 0x2f: // END OF TRACK
                                    {
                                        tracks[i].state = TState::TS_SERVED;
                                    }
                                    break;

                                case 0x51: // MetaSetTempo
                                    {
                                        uint32_t metaSetTempo = getBytes(3, &(tracks[i].position));
                                        const uint32_t BPM = 60000000 / metaSetTempo;
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
                                        godot::UtilityFunctions::print("MetaSetTempo: ", metaSetTempo);
                                        godot::UtilityFunctions::print("       Track: ", i);
                                        godot::UtilityFunctions::print("        tick: ", tracks[i].tick);
                                        godot::UtilityFunctions::print("         BPM: ", BPM);
#endif // DEBUG_ENABLED
                                        if(tracks[i].tick == 0) tempos[0].tempo = BPM;
                                        else {
                                            tempos.push_back({tracks[i].tick, BPM});
                                            std::sort(tempos.begin(), tempos.end());
                                    
                                            float elapsedTicks = 0.0f;
                                            float time = 0.0f;
                                            for (int32_t j = 1; j < tempos.size(); ++j) {
                                                time += (unitOfTime/tempos[j-1].tempo)*((tempos[j].tick-elapsedTicks)/timeDivision);
                                                elapsedTicks = tempos[j].tick;
                                                tempos[j].time = time;
                                            }
                                        }
                                    }
                                    break;

                                case 0x54: // MetaSMPTEOffset
                                    {
                                        skipByte(5, &(tracks[i].position));
                                    }
                                    break;

                                case 0x58: // MetaTimeSignature
                                    {
                                        skipByte(4, &(tracks[i].position));
                                    }
                                    break;

                                case 0x59: // MetaKeySignature
                                    {
                                        skipByte(2, &(tracks[i].position));
                                    }
                                    break;

                                case 0x7f: // MetaSequencerSpecific
                                    {
                                        skipByte(value, &(tracks[i].position));
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
        if (tracks[i].state == TState::TS_SERVED) numServed += 1;
    }

    if (numServed == numOfTracks){
        retNote.state = NState::NS_END;
    }
    else {
        int32_t minTick = 0x7fffffff; // set very large number
        int32_t j = 0;
        for (int32_t i = 0; i < numOfTracks; ++i) {
            if(tracks[i].nextNote.startTick < minTick) {
                minTick = tracks[i].nextNote.startTick;
                j = i;
            }
        }

        auto it = std::find_if(tempos.begin(), tempos.end(), [&](const Tempo &bpm) {
            return (tracks[j].nextNote.startTick < bpm.tick);
        }) - 1;
        if (it != tempos.end()) {
            const float TEMPO = ((unitOfTime / it->tempo) /  timeDivision);
            tracks[j].nextNote.tempo = (int32_t)it->tempo;
            tracks[j].nextNote.startTime = (int32_t)(it->time + ((tracks[j].nextNote.startTick - it->tick) * TEMPO));
        }
        if(tracks[j].nextNote.startTime < till){
            tracks[j].state = TState::TS_EMPTY;
            retNote = tracks[j].nextNote;
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

