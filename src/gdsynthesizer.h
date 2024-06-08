/**************************************************************************/
/*  gdsynthesizer.h                                                       */
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


#ifndef GDSYNTHESIZER_H
#define GDSYNTHESIZER_H
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <functional>

#include "sequencer.hpp"

namespace godot {

class GDSynthesizer : public AudioStreamPlayer {
    GDCLASS(GDSynthesizer, AudioStreamPlayer);

    static constexpr double mix_rate = 44100.0; // Sampling frq. It's also the samples num par sec.
    static constexpr double buffer_length = 0.1; // Buffer length in seconds.

    int32_t buf_samples = int32_t(mix_rate*buffer_length);
    double time_passed;
    PackedVector2Array frames;
protected:
    static void _bind_methods();
public:
    double* pcmBuf = nullptr;
    Sequencer sequencer;
    GDSynthesizer();
    ~GDSynthesizer();
    void feedData(double delta);
    int initSynthe(const int32_t max_note);
    int loadMidi(const String &p_file);
    void unloadMidi(void);
    void setSyntheParams(const Array);
    Array getSyntheParams(void);

    void setPercussionParams(const Array);
    Array getPercussionParams(void);

    void setControlParams(const Dictionary);
    Dictionary getControlParams(void);

    void setNoteOn(const Dictionary);
    void setNoteOff(const Dictionary);
    Ref<Image> getMiniWavePicture(const Dictionary);
    
    void emitSignal(const godot::Dictionary dic);
};
}

#endif // GDSYNTHESIZER_H
