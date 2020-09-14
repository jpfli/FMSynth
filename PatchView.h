
#pragma once

#include <cstdint>
#include <array>
#include "View.h"
#include "pomi/Graphics.h"
#include "StringBuffer.h"
#include "FMSynth.h"

#include "fonts/lcdfont.h"

class PatchView final: public View {
    
    public:
    
        constexpr PatchView(): View(), _dirtyflags(~0), _tab(Tab::General), _incr(0) {}
        ~PatchView() = default;
        constexpr PatchView(const PatchView&) = default;
        PatchView& operator =(const PatchView&) = delete;
        
        inline void nextTab() {
            _tab = (static_cast<std::uint8_t>(_tab) < 2) ? static_cast<Tab>(static_cast<std::uint8_t>(_tab) + 1) : Tab::General;
            _dirtyflags = ~0;
        }
        
        inline void onUpPressed(std::int32_t button_id) override {
            _incr = _incr < 20 ? (_incr + 1) : 20;
            
            switch(_tab) {
                case Tab::General:
                
                    switch(button_id) {
                        case 0:
                        
                            if(FMSynth::patch().algorithm < 11) {
                                FMSynth::patch().algorithm += 1;
                                _dirtyflags |= _TOPAREA_FLAG;
                            }
                            break;
                        
                        case 1:
                        
                            FMSynth::patch().volume = View::increaseLevel<100>(FMSynth::patch().volume, _incr);
                            break;
                        
                        case 2:
                        
                            FMSynth::patch().feedback = View::increaseLevel<100>(FMSynth::patch().feedback, _incr);
                            break;
                        
                        case 3:
                        
                            FMSynth::patch().glide = View::increaseLevel<100>(FMSynth::patch().glide, _incr);
                            break;
                    }
                    break;
                
                case Tab::Envelope:
                
                    switch(button_id) {
                        case 0:
                        
                            FMSynth::patch().attack = View::increaseLevel<100>(FMSynth::patch().attack, _incr);
                            break;
                        
                        case 1:
                        
                            FMSynth::patch().decay = View::increaseLevel<100>(FMSynth::patch().decay, _incr);
                            break;
                        
                        case 2:
                        
                            FMSynth::patch().sustain = View::increaseLevel<100>(FMSynth::patch().sustain, _incr);
                            break;
                        
                        case 3:
                        
                            FMSynth::patch().release = View::increaseLevel<100>(FMSynth::patch().release, _incr);
                            break;
                    }
                    break;
                
                case Tab::PitchLFO:
                
                    switch(button_id) {
                        case 0:
                        
                            FMSynth::patch().lfo.pmd = View::increaseLevel<100>(FMSynth::patch().lfo.pmd, _incr);
                            break;
                        
                        case 1:
                        
                            FMSynth::patch().lfo.speed = View::increaseLevel<100>(FMSynth::patch().lfo.speed, _incr);
                            break;
                        
                        case 2:
                        
                            FMSynth::patch().lfo.attack = View::increaseLevel<100>(FMSynth::patch().lfo.attack, _incr);
                            break;
                    }
                    break;
            }
            
            _dirtyflags |= 1 << button_id;
        }
        
        inline void onUpReleased(std::int32_t button_id) override {
            _incr = 0;
        }
        
        inline void onMidPressed(std::int32_t button_id) override {
            
        }
        
        inline void onMidReleased(std::int32_t button_id) override {
            
        }
        
        inline void onDownPressed(std::int32_t button_id) override {
            _incr = _incr > -20 ? (_incr - 1) : -20;
            
            switch(_tab) {
                case Tab::General: // General settings
                
                    switch(button_id) {
                        case 0:
                        
                            if(FMSynth::patch().algorithm > 1) {
                                FMSynth::patch().algorithm -= 1;
                                _dirtyflags |= _TOPAREA_FLAG;
                            }
                            break;
                        
                        case 1:
                        
                            FMSynth::patch().volume = View::decreaseLevel<0>(FMSynth::patch().volume, _incr);
                            break;
                        
                        case 2:
                        
                            FMSynth::patch().feedback = View::decreaseLevel<0>(FMSynth::patch().feedback, _incr);
                            break;
                        
                        case 3:
                        
                            FMSynth::patch().glide = View::decreaseLevel<0>(FMSynth::patch().glide, _incr);
                            break;
                    }
                    break;
                
                case Tab::Envelope: // ADSR envelope
                
                    switch(button_id) {
                        case 0:
                        
                            FMSynth::patch().attack = View::decreaseLevel<0>(FMSynth::patch().attack, _incr);
                            break;
                        
                        case 1:
                        
                            FMSynth::patch().decay = View::decreaseLevel<0>(FMSynth::patch().decay, _incr);
                            break;
                        
                        case 2:
                        
                            FMSynth::patch().sustain = View::decreaseLevel<0>(FMSynth::patch().sustain, _incr);
                            break;
                        
                        case 3:
                        
                            FMSynth::patch().release = View::decreaseLevel<0>(FMSynth::patch().release, _incr);
                            break;
                    }
                    break;
                
                case Tab::PitchLFO:
                
                    switch(button_id) {
                        case 0:
                        
                            FMSynth::patch().lfo.pmd = View::decreaseLevel<0>(FMSynth::patch().lfo.pmd, _incr);
                            break;
                        
                        case 1:
                        
                            FMSynth::patch().lfo.speed = View::decreaseLevel<0>(FMSynth::patch().lfo.speed, _incr);
                            break;
                        
                        case 2:
                        
                            FMSynth::patch().lfo.attack = View::decreaseLevel<0>(FMSynth::patch().lfo.attack, _incr);
                            break;
                    }
                    break;
            }
            
            _dirtyflags |= 1 << button_id;
        }
        
        inline void onDownReleased(std::int32_t button_id) override {
            _incr = 0;
        }
        
        inline void enter() override {
            _dirtyflags = ~0;
        }
        
        inline void leave() override {
            
        }
        
        inline std::uint8_t getInputFlags(std::uint32_t button_idx) override {
            switch(_tab) {
                
                case Tab::General:
                
                    switch(button_idx) {
                        case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 1: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 2: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 3: return View::InputFlags::UP | View::InputFlags::DOWN;
                    }
                    break;
                
                case Tab::Envelope:
                
                    switch(button_idx) {
                        case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 1: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 2: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 3: return View::InputFlags::UP | View::InputFlags::DOWN;
                    }
                    break;
                
                case Tab::PitchLFO:
                
                    switch(button_idx) {
                        case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 1: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 2: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 3: return 0;
                    }
                    break;
            }
            return 0;
        }
        
        inline void draw(Pomi::Graphics& gfx, bool force_redraw=false) override {
            if(force_redraw) {
                _dirtyflags = ~0;
            }
            else if(_dirtyflags == 0) {
                return;
            }
            
            gfx.setFont(lcdfont);
            
            if(_dirtyflags & _BGND_FLAG) {
                View::drawBackground(gfx, 3, static_cast<std::uint8_t>(_tab));
            }
            
            switch(_tab) {
                
                case Tab::General: // General settings
                
                    if(_dirtyflags & _TOPAREA_FLAG) {
                        View::drawAlgorithm(gfx, FMSynth::patch().algorithm);
                        View::drawHeader(gfx, "ALGO", "VOL", "FB", "GLIDE");
                    }
                    
                    if(_dirtyflags & _COL1_FLAG) {
                        View::drawValue(gfx, 0, FMSynth::patch().algorithm);
                    }
                    
                    if(_dirtyflags & _COL2_FLAG) {
                        View::drawLevel(gfx, 1, FMSynth::patch().volume);
                    }
                    
                    if(_dirtyflags & _COL3_FLAG) {
                        View::drawSignedValue(gfx, 2, FMSynth::patch().feedback - 50);
                    }
                    
                    if(_dirtyflags & _COL4_FLAG) {
                        View::drawLevel(gfx, 3, FMSynth::patch().glide);
                    }
                    
                    break;
                
                case Tab::Envelope: // ADSR envelope
                
                    if(_dirtyflags & _TOPAREA_FLAG) {
                        gfx.drawFilledRectangle(2, 2, 216, 50, _BG_COLOR);
                        gfx.print("ENVELOPE", 110 - 7 * 8 / 2, 21, _TEXT_COLOR);
                        
                        View::drawHeader(gfx, "ATT", "DEC", "SUS", "REL");
                    }
                    
                    if(_dirtyflags & _COL1_FLAG) {
                        View::drawLevel(gfx, 0, FMSynth::patch().attack);
                    }
                    if(_dirtyflags & _COL2_FLAG) {
                        View::drawLevel(gfx, 1, FMSynth::patch().decay);
                    }
                    if(_dirtyflags & _COL3_FLAG) {
                        View::drawLevel(gfx, 2, FMSynth::patch().sustain);
                    }
                    if(_dirtyflags & _COL4_FLAG) {
                        View::drawLevel(gfx, 3, FMSynth::patch().release);
                    }
                    
                    break;
                
                case Tab::PitchLFO: // Pitch LFO
                    if(_dirtyflags & _TOPAREA_FLAG) {
                        gfx.drawFilledRectangle(2, 2, 216, 50, _BG_COLOR);
                        gfx.print("PITCH LFO", 110 - 7 * 9 / 2, 21, _TEXT_COLOR);
                        
                        View::drawHeader(gfx, "DEPTH", "FREQ", "ATT");
                    }
                    
                    if(_dirtyflags & _COL1_FLAG) {
                        View::drawLevel(gfx, 0, FMSynth::patch().lfo.pmd);
                    }
                    if(_dirtyflags & _COL2_FLAG) {
                        View::drawLFOFrequency(gfx, 1, FMSynth::patch().lfo.speed);
                    }
                    if(_dirtyflags & _COL3_FLAG) {
                        View::drawLevel(gfx, 2, FMSynth::patch().lfo.attack);
                    }
                    
                    break;
            }
            
            _dirtyflags = 0;
        }
    
    private:
    
        enum class Tab: std::uint8_t { General, Envelope, PitchLFO };
    
        static constexpr std::uint8_t _BG_COLOR = 3;
        static constexpr std::uint8_t _TEXT_COLOR = 6;
        
        static constexpr std::int32_t _COL1_FLAG = 1<<0;
        static constexpr std::int32_t _COL2_FLAG = 1<<1;
        static constexpr std::int32_t _COL3_FLAG = 1<<2;
        static constexpr std::int32_t _COL4_FLAG = 1<<3;
        static constexpr std::int32_t _TOPAREA_FLAG = 1<<4;
        static constexpr std::int32_t _BGND_FLAG = 1<<5;
        
        std::uint8_t _dirtyflags;
        
        Tab _tab;
        std::int8_t _incr;
};
