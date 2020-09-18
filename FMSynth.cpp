
#include <cstdint>
#include <Pokitto.h>
#include <USBMIDI.h>

#include "pomi/Input.h"
#include "pomi/Graphics.h"

#include "FMSynth.h"
#include "FMPatch.h"
#include "FMSource.h"
#include "FMSynthUI.h"

#include "fonts/lcdfont.h"
#include "images/usb_1bpp.h"

FMPatch FMSynth::_patch(_banks[0].patches[0]);
FMSource<FMSynth::SAMPLERATE> FMSynth::_voices[4] = {
    FMSource<FMSynth::SAMPLERATE>(0), FMSource<FMSynth::SAMPLERATE>(1), FMSource<FMSynth::SAMPLERATE>(2), FMSource<FMSynth::SAMPLERATE>(3)
};
std::uint32_t FMSynth::_voice_idx;

bool FMSynth::_sustain;
bool FMSynth::_sustained_voices[4];

USBMIDI* FMSynth::_midi(nullptr);
std::int8_t FMSynth::_midi_channel(0);

std::int8_t FMSynth::_prev_midikey(-1);

void FMSynth::run() {
    FMSynthUI::init();
    
    _connectUSB();
    
    Pomi::Mixer::init(SAMPLERATE);
    
    while(true) {
        std::uint32_t now = Pokitto::Core::getTime();
        
        Pomi::Input::update(now);
        
        FMSynthUI::update(now);
    }
}

void FMSynth::noteOn(std::int8_t midikey, std::int8_t velocity) {
    if(midikey < 0) {
        return;
    }
    else if(velocity <= 0) {
        noteOff(midikey, velocity);
        return;
    }
    
    if(_patch.glide > 0 && !_voices[_voice_idx].released()) {
        _prev_midikey = _sustain ? -1 : _voices[_voice_idx].midikey();
    }
    else {
        std::uint32_t orig_idx = _voice_idx;
        std::uint32_t idx = _voice_idx;
        do {
            if(_voices[idx].midikey() == midikey) {
                _voice_idx = idx;
                break;
            }
            else if(_voices[idx].released()) {
                _voice_idx = idx;
            }
            idx = idx > 0 ? (idx - 1) : 3;
        } while(idx != orig_idx);
        
        if(_voice_idx == orig_idx) {
            _voice_idx = _voice_idx < 3 ? (_voice_idx + 1) : 0;
        }
    }
    _voices[_voice_idx].noteOn(_patch, midikey, velocity);
    _sustained_voices[_voice_idx] = false;
}

void FMSynth::noteOff(std::int8_t midikey, std::int8_t velocity) {
    if(midikey < 0) {
        return;
    }
    
    if(_patch.glide > 0 && _prev_midikey >= 0) {
        if(midikey == _prev_midikey) {
            _prev_midikey = -1;
        }
        else if(midikey == _voices[_voice_idx].midikey()) {
            if(!_sustain) {
                _voices[_voice_idx].noteOn(_patch, _prev_midikey, velocity);
            }
            _prev_midikey = -1;
        }
        return;
    }
    
    for(std::uint32_t idx = 0; idx < 4; ++idx) {
        if(_voices[idx].midikey() == midikey) {
            if(!_sustain) {
                _voices[idx].noteOff();
            }
            else if(!_voices[idx].released()) {
                _sustained_voices[idx] = true;
            }
        }
    }
}

void FMSynth::setSustain(bool sustain) {
    _sustain = sustain;
    
    if(!sustain) {
        for(std::uint32_t idx = 0; idx < 4; ++idx) {
            if(_sustained_voices[idx]) {
                _voices[idx].noteOff();
                _sustained_voices[idx] = false;
            }
        }
    }
}

void FMSynth::setPitchBend(std::int32_t value) {
    // Pitchbend value converted from range -8192...8191 to a fixed point value -1.0...1.0
    FixedPoint::Fixed<15> ratio = FixedPoint::Fixed<15>::fromInternal(value << 2);
    for(std::uint32_t idx = 0; idx < 4; ++idx) {
        _voices[idx].setPitchBend(ratio);
    }
}

bool FMSynth::sendMIDINoteOff(std::int32_t midikey, std::int32_t velocity) {
    if(_midi) {
        _midi->write(MIDIMessage::NoteOff(midikey, velocity, _midi_channel));
        return true;
    }
    return false;
}

bool FMSynth::sendMIDINoteOn(std::int32_t midikey, std::int32_t velocity) {
    if(_midi) {
        _midi->write(MIDIMessage::NoteOn(midikey, velocity, _midi_channel));
        return true;
    }
    return false;
}

bool FMSynth::sendMIDIControlChange(std::int32_t controller, std::int32_t value) {
    if(_midi) {
        _midi->write(MIDIMessage::ControlChange(controller, value, _midi_channel));
        return true;
    }
    return false;
}

bool FMSynth::sendMIDIPitchWheel(std::int32_t pitch) {
    if(_midi) {
        _midi->write(MIDIMessage::PitchWheel(pitch <= -8192 ? -8192 : (pitch >= 8191 ? 8191 : pitch), _midi_channel));
        return true;
    }
    return false;
}

bool FMSynth::sendMIDIPatchDump() {
    if(_midi) {
        std::uint8_t* src = reinterpret_cast<std::uint8_t*>(&_patch);
        MIDIMessage msg = MIDIMessage::SysEx(src - 2, sizeof(FMPatch) + 2);
        msg.data[1] = 0xf0;  // SysEx message type
        msg.data[2] = 0x7d;  // Manufacturer ID for educational and development use
        _midi->write(msg);
        
        return true;
    }
    return false;
}

void FMSynth::onMIDIReceived(MIDIMessage msg) {
    // Handle SysEx message before anything else
    if(msg.data[1] == 0xf0) {
        std::uint8_t* dst = reinterpret_cast<std::uint8_t*>(&_patch);
        std::uint32_t len = msg.length - 2 < sizeof(FMPatch) ? msg.length - 2 : sizeof(FMPatch);
        for(std::uint32_t idx = 0; idx < len; ++idx) {
            dst[idx] = msg.data[3 + idx]; 
        }
        // Make sure the name string is terminated with null
        _patch.name[15] = '\0';
        return;
    }
    
    // Discard messages on channels other than 0
    if(msg.channel() != _midi_channel) {
        return;
    }
    
    constexpr std::uint32_t SUSTAIN_CONTROLLER_ID = 64;
    
    switch(msg.type()) {
        case MIDIMessage::NoteOnType:
        
            noteOn(msg.key(), msg.velocity());
            break;
        
        case MIDIMessage::NoteOffType:
        
            noteOff(msg.key(), msg.velocity());
            break;
        
        case MIDIMessage::ControlChangeType:
        
            if(msg.controller() == SUSTAIN_CONTROLLER_ID) {
                setSustain(msg.value() >= 64);
            }
        
        case MIDIMessage::PitchWheelType:
            
            setPitchBend(msg.pitch());
            break;
    }    
}

void FMSynth::_connectUSB() {
    do {
        Pomi::Input::update(0);
    } while(Pomi::Input::held(Pomi::Input::Button::A | Pomi::Input::Button::B | Pomi::Input::Button::C));
    
    Pomi::Graphics& gfx = FMSynthUI::graphics();
    gfx.setFont(lcdfont);
    
    gfx.drawFilledRectangle(2, 2, 216, 128, 3);
    
    gfx.drawImage1BPP(usb_1bpp, 6, 86, 28);
    gfx.print("Press   for USB MIDI", 40, 65, 6);
    gfx.print("C", 82, 65, 5);
    
    std::uint32_t exit_time = Pokitto::Core::getTime() + 5 * 1000;
    std::int32_t counter = -1;
    
    while(true) {
        Pomi::Input::update(0);
        
        if(Pomi::Input::pressed(Pomi::Input::Button::C)) {
            gfx.drawFilledRectangle(110 - 7*17/2, 89, 7 * 17, 11, 3);
            gfx.print("Connecting USB...", 110 - 7*17/2, 89, 6);
            
            _midi = new USBMIDI;
            _midi->attach(onMIDIReceived);
            break;
        }
        
        std::int32_t delta = exit_time - Pokitto::Core::getTime();
        std::int32_t counter_now = (delta + 999) / 1000;
        if(counter_now <= 0 || Pomi::Input::pressed(Pomi::Input::Button::A | Pomi::Input::Button::B)) {
            break;
        }
        else if(counter_now != counter) {
            counter = counter_now;
            
            StringBuffer<5> counter_text;
            counter_text.append(counter);
            
            std::int32_t x = 110 - 7 * counter_text.length() / 2;
            
            gfx.drawFilledRectangle(x, 89, 7 * counter_text.length(), 11, 3);
            gfx.print(counter_text, x, 89, 5);
        }
    }
    
    do {
        Pomi::Input::update(0);
    } while(Pomi::Input::held(Pomi::Input::Button::A | Pomi::Input::Button::B | Pomi::Input::Button::C));
}
