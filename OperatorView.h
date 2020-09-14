
#pragma once

#include <cstdint>
#include "View.h"
#include "pomi/Graphics.h"
#include "StringBuffer.h"
#include "FMSynth.h"

#include "fonts/lcdfont.h"

class OperatorView final: public View {
    
    public:
    
        constexpr OperatorView(): View(), _dirtyflags(~0), _op_idx(0), _tab(Tab::Oscillator), _incr(0), _finetune(false) {}
        ~OperatorView() = default;
        constexpr OperatorView(const OperatorView&) = default;
        OperatorView& operator =(const OperatorView&) = delete;
        
        inline std::uint8_t operatorIndex() const { return _op_idx; }
        
        inline void setOperatorIndex(std::uint32_t idx) {
            if(idx != _op_idx) {
                _op_idx = idx;
                _dirtyflags = ~0;
            }
        }
        
        inline void nextTab() {
            _tab = (static_cast<std::uint8_t>(_tab) < 1) ? static_cast<Tab>(static_cast<std::uint8_t>(_tab) + 1) : Tab::Oscillator;
            _dirtyflags = ~0;
        }
        
        inline void onUpPressed(std::int32_t button_id) override {
            _incr = _incr < 20 ? (_incr + 1) : 20;
            FMPatch::Operator& op = FMSynth::patch().op[_op_idx];
            
            switch(_tab) {
                case Tab::Oscillator:
                
                    switch(button_id) {
                        case 0:
                        
                            op.level = View::increaseLevel<100>(op.level, _incr);
                            break;
                         
                        case 2:
                        
                            if(_finetune) {
                                std::int32_t fine = View::increaseLevel<199>(op.pitch.fine, _incr);
                                if(fine > 99) {
                                    if(op.pitch.coarse < 15) {
                                        fine -= 100;
                                        op.pitch.coarse = View::increaseLevel<15>(op.pitch.coarse, 1);
                                    }
                                    else {
                                        fine = 99;
                                    }
                                }
                                op.pitch.fine = fine;
                            }
                            else {
                                op.pitch.coarse = View::increaseLevel<15>(op.pitch.coarse, _incr);
                            }
                            break;
                            
                        case 3:
                        
                            op.detune = View::increaseLevel<100>(op.detune, _incr);
                            break;
                    }
                    break;
                
                case Tab::Envelope:
                
                    switch(button_id) {
                        case 0:
                        
                            op.attack = View::increaseLevel<100>(op.attack, _incr);
                            break;
                        
                        case 1:
                        
                            op.decay = View::increaseLevel<100>(op.decay, _incr);
                            break;
                        
                        case 2:
                        
                            op.sustain = View::increaseLevel<100>(op.sustain, _incr);
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
            _finetune = true;
            
            FMPatch::Operator& op = FMSynth::patch().op[_op_idx];
            
            switch(_tab) {
                case Tab::Oscillator:
                
                    if(button_id == 1) {
                        op.pitch.fixed = !op.pitch.fixed;
                        _dirtyflags |= (1 << button_id) | _COL3_FLAG | _TOPAREA_FLAG;
                    }
                
                case Tab::Envelope:
                
                    if(button_id == 3) {
                        op.loop = !op.loop;
                        _dirtyflags |= 1 << button_id;
                    }
                    break;
            }
        }
        
        inline void onMidReleased(std::int32_t button_id) override {
            _finetune = false;
        }
        
        inline void onDownPressed(std::int32_t button_id) override {
            _incr = _incr > -20 ? (_incr - 1) : -20;
            FMPatch::Operator& op = FMSynth::patch().op[_op_idx];
            
            switch(_tab) {
                case Tab::Oscillator:
                
                    switch(button_id) {
                        case 0:
                        
                            op.level = View::decreaseLevel<0>(op.level, _incr);
                            break;
                        
                        case 2:
                        
                            if(_finetune) {
                                std::int32_t fine = View::decreaseLevel<-100>(op.pitch.fine, _incr);
                                if(fine < 0) {
                                    if(op.pitch.coarse > 0) {
                                        fine += 100;
                                        op.pitch.coarse = View::decreaseLevel<0>(op.pitch.coarse, -1);
                                    }
                                    else {
                                        fine = 0;
                                    }
                                }
                                op.pitch.fine = fine;
                            }
                            else {
                                op.pitch.coarse = View::decreaseLevel<0>(op.pitch.coarse, _incr);
                            }
                            break;
                        
                        case 3:
                        
                            op.detune = View::decreaseLevel<0>(op.detune, _incr);
                            break;
                    }
                    break;
                
                case Tab::Envelope:
                
                    switch(button_id) {
                        case 0:
                        
                            op.attack = View::decreaseLevel<0>(op.attack, _incr);
                            break;
                        
                        case 1:
                        
                            op.decay = View::decreaseLevel<0>(op.decay, _incr);
                            break;
                        
                        case 2:
                        
                            op.sustain = View::decreaseLevel<0>(op.sustain, _incr);
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
                
                case Tab::Oscillator:
                
                    switch(button_idx) {
                        case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 1: return View::InputFlags::MID;
                        case 2: return View::InputFlags::UP | View::InputFlags::MID | View::InputFlags::DOWN;
                        case 3: return View::InputFlags::UP | View::InputFlags::DOWN;
                    }
                    break;
                
                case Tab::Envelope:
                
                    switch(button_idx) {
                        case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 1: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 2: return View::InputFlags::UP | View::InputFlags::DOWN;
                        case 3: return View::InputFlags::MID;
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
                View::drawBackground(gfx, 2, static_cast<std::uint8_t>(_tab));
            }
            
            FMPatch::Operator& op = FMSynth::patch().op[_op_idx];
            
            switch(_tab) {
                case Tab::Oscillator:
                
                    if(_dirtyflags & _TOPAREA_FLAG) {
                        gfx.drawFilledRectangle(2, 2, 216, 64, _BG_COLOR);
                        
                        gfx.print("OSCILLATOR", 110 - 7 * 10 / 2, 21, _TEXT_COLOR);
                        
                        View::drawHeader(gfx, "LEVEL", "MODE", (op.pitch.fixed) ? "FREQ" : "RATIO", "DTUNE");
                    }
                    
                    if(_dirtyflags & _COL1_FLAG) {
                        View::drawLevel(gfx, 0, op.level);
                    }
                    
                    if(_dirtyflags & _COL2_FLAG) {
                        View::drawLabel(gfx, 1, (op.pitch.fixed) ? "FIXED" : "RATIO");
                    }
                    
                    if(_dirtyflags & _COL3_FLAG) {
                        if(op.pitch.fixed) {
                            View::drawPitchFixed(gfx, 2, op.pitch.coarse, op.pitch.fine);
                        }
                        else {
                            View::drawPitchRatio(gfx, 2, op.pitch.coarse, op.pitch.fine);
                        }
                    }
                    
                    if(_dirtyflags & _COL4_FLAG) {
                        View::drawSignedValue(gfx, 3, op.detune - 50);
                    }
                    break;
                
                case Tab::Envelope:
                
                    if(_dirtyflags & _TOPAREA_FLAG) {
                        gfx.drawFilledRectangle(2, 2, 216, 64, _BG_COLOR);
                        
                        gfx.print("ENVELOPE", 110 - 7 * 8 / 2, 21, _TEXT_COLOR);
                        
                        View::drawHeader(gfx, "ATT", "DEC", "SUS", "LOOP");
                    }
                    
                    if(_dirtyflags & _COL1_FLAG) {
                        View::drawLevel(gfx, 0, op.attack);
                    }
                    if(_dirtyflags & _COL2_FLAG) {
                        View::drawLevel(gfx, 1, op.decay);
                    }
                    if(_dirtyflags & _COL3_FLAG) {
                        View::drawLevel(gfx, 2, op.sustain);
                    }
                    if(_dirtyflags & _COL4_FLAG) {
                        View::drawLabel(gfx, 3, (op.loop) ? "ON" : "OFF");
                    }
                    break;
            }
            
            _dirtyflags = 0;
        }
    
    private:
    
        enum class Tab: std::uint8_t { Oscillator, Envelope };
    
        static constexpr std::uint8_t _BG_COLOR = 3;
        static constexpr std::uint8_t _TEXT_COLOR = 6;
        
        static constexpr std::int32_t _COL1_FLAG = 1<<0;
        static constexpr std::int32_t _COL2_FLAG = 1<<1;
        static constexpr std::int32_t _COL3_FLAG = 1<<2;
        static constexpr std::int32_t _COL4_FLAG = 1<<3;
        static constexpr std::int32_t _TOPAREA_FLAG = 1<<4;
        static constexpr std::int32_t _BGND_FLAG = 1<<5;
        
        std::uint8_t _dirtyflags;
        std::uint8_t _op_idx;
        Tab _tab;
        std::int8_t _incr;
        bool _finetune;
};
