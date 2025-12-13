/**************************************************************************/
/*  gdsynthesizer.cpp                                                     */
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

#include "gdsynthesizer.h"
#include <godot_cpp/core/class_db.hpp>

#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
#include <godot_cpp/variant/utility_functions.hpp> // for "UtilityFunctions::print()".
#endif // DEBUG_ENABLED

#include <filesystem>

#include <cmath>
#include <new>

using namespace godot;

void GDSynthesizer::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init_synthe", "max_note"), &GDSynthesizer::initSynthe);
    ClassDB::bind_method(D_METHOD("load_midi", "file_path"), &GDSynthesizer::loadMidi);
    ClassDB::bind_method(D_METHOD("unload_midi"), &GDSynthesizer::unloadMidi);
    ClassDB::bind_method(D_METHOD("feed_data", "delta"), &GDSynthesizer::feedData);

    ClassDB::bind_method(D_METHOD("set_synthe_params", "p_array"), &GDSynthesizer::setSyntheParams);
    ClassDB::bind_method(D_METHOD("get_synthe_params"), &GDSynthesizer::getSyntheParams);

    ClassDB::bind_method(D_METHOD("set_percussion_params", "p_array"), &GDSynthesizer::setPercussionParams);
    ClassDB::bind_method(D_METHOD("get_percussion_params"), &GDSynthesizer::getPercussionParams);

    ClassDB::bind_method(D_METHOD("set_control_params", "p_dict"), &GDSynthesizer::setControlParams);
    ClassDB::bind_method(D_METHOD("get_control_params"), &GDSynthesizer::getControlParams);

    ClassDB::bind_method(D_METHOD("set_note_on", "p_dict"), &GDSynthesizer::setNoteOn);
    ClassDB::bind_method(D_METHOD("set_note_off", "p_dict"), &GDSynthesizer::setNoteOff);
    
    ClassDB::bind_method(D_METHOD("get_mini_wave_picture", "p_dict"), &GDSynthesizer::getMiniWavePicture);
    
    ADD_SIGNAL(MethodInfo("note_changed", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::DICTIONARY, "note")));
    ADD_SIGNAL(MethodInfo("pre_note_changed", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::DICTIONARY, "note")));
    ADD_SIGNAL(MethodInfo("level_info", PropertyInfo(Variant::DICTIONARY, "level")));
}

GDSynthesizer::GDSynthesizer()
{
	time_passed = 0;
    sequencer.emitSignal = std::bind(&GDSynthesizer::emitSignal, this, std::placeholders::_1);
}

GDSynthesizer::~GDSynthesizer()
{
    delete [] pcmBuf;
    pcmBuf = nullptr;
}

int GDSynthesizer::initSynthe(const int32_t max_note)
{
    if (!sequencer.initParam(mix_rate, buffer_length/2.0, buf_samples/2)) {
        return 0;
    }
    Ref<AudioStreamGenerator> stream;
    stream.instantiate(); // use Godot allocator (memnew equivalent)
    set_stream(stream);
    stream->set_mix_rate(mix_rate);
    stream->set_buffer_length(buffer_length);

    // Release existing buffer before allocating a new one
    if (pcmBuf != nullptr) {
        delete [] pcmBuf;
        pcmBuf = nullptr;
    }
    pcmBuf = new (std::nothrow) double[buf_samples/2];
    if (pcmBuf == nullptr) {
        return 0; // Allocation failed
    }
    frames = PackedVector2Array();
    frames.resize((int64_t)buf_samples/2);

    return 1;
}

void GDSynthesizer::unloadMidi(void)
{
    sequencer.smfUnload();
}

int GDSynthesizer::loadMidi(const String &file_path)
{
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
	UtilityFunctions::print("input strings: ", file_path.utf8().ptr());
	UtilityFunctions::print("input strings: ", file_path);
#endif // DEBUG_ENABLED

    if(FileAccess::file_exists(file_path)){
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        UtilityFunctions::print("file exist, type 1");
#endif // DEBUG_ENABLED
        sequencer.smfLoad(file_path, 60000.0);
    }
    else if (std::filesystem::is_regular_file(file_path.utf8().ptr())) {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        UtilityFunctions::print("file exist, type 2");
#endif // DEBUG_ENABLED
        sequencer.smfLoad(file_path.utf8().ptr(), 60000.0);
    }
    else {
#if defined(DEBUG_ENABLED) && defined(WINDOWS_ENABLED)
        UtilityFunctions::print(std::filesystem::current_path().c_str(), " is not file");
#endif // DEBUG_ENABLED
        return 0;
    }
    return 1;
}

void GDSynthesizer::feedData(double delta) {
    time_passed += delta;
    if (is_playing()) {
        int32_t size = (int32_t)frames.size();
        Ref<AudioStreamGeneratorPlayback> playback = get_stream_playback();
        if (playback->can_push_buffer(size)) {
            sequencer.feed(pcmBuf);
            for (int32_t i = 0; i < size; i++) {
                frames[i] = Vector2(pcmBuf[i], pcmBuf[i]);
            }
            playback->push_buffer(frames);
        }
    }
}


void GDSynthesizer::emitSignal(const godot::Dictionary dic) {
    if ((int32_t)dic["msg"] == 0){
        if ((int32_t)dic["onOff"] == 1){
            emit_signal("note_changed", "note_on", dic);
        }
        else{
            emit_signal("note_changed", "note_off", dic);
        }
    }
    else if ((int32_t)dic["msg"] == 1){
        emit_signal("level_info", dic);
    }
    else if ((int32_t)dic["msg"] == 2){
        // pre-on signal
        if ((int32_t)dic["onOff"] == 1){
            emit_signal("pre_note_changed", "pre_note_on", dic);
        }
        else{
            emit_signal("pre_note_changed", "pre_note_off", dic);
        }
    }
}


void GDSynthesizer::setSyntheParams(const Array p_array) {
    sequencer.setInstruments(p_array);
}

Array GDSynthesizer::getSyntheParams(void) {
    return sequencer.getInstruments();
}

void GDSynthesizer::setPercussionParams(const Array p_array) {
    sequencer.setPercussions(p_array);
}

Array GDSynthesizer::getPercussionParams(void) {
    return sequencer.getPercussions();
}

void GDSynthesizer::setNoteOn(const Dictionary p_dic) {
    sequencer.incertNoteOn(p_dic);
}

void GDSynthesizer::setNoteOff(const Dictionary p_dic) {
    sequencer.incertNoteOff(p_dic);
}

void GDSynthesizer::setControlParams(const Dictionary p_dic) {
    sequencer.setControlParams(p_dic);
}

Dictionary GDSynthesizer::getControlParams(void) {
    return sequencer.getControlParams();
}

Ref<Image> GDSynthesizer::getMiniWavePicture(const Dictionary p_dic) {
    return sequencer.getMiniWavePicture(p_dic);
}


