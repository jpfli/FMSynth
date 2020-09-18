
#pragma once

#include <cstdint>
#include "View.h"
#include "pomi/Graphics.h"
#include "StringBuffer.h"
#include "FMSynth.h"

#include "fonts/lcdfont.h"

class FreqView final: public View {
    
    public:
    
        constexpr FreqView(): View(), _dirtyflags(~0), _incr(0), _finetune(false) {}
        ~FreqView() = default;
        constexpr FreqView(const FreqView&) = default;
        FreqView& operator =(const FreqView&) = delete;
        
        inline void onUpPressed(std::int32_t button_id) override {
            _incr = _incr < 20 ? (_incr + 1) : 20;
            FMPatch::Operator& op = FMSynth::patch().op[button_id];
            
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
            
            _dirtyflags |= 1 << button_id;
        }
        
        inline void onUpReleased(std::int32_t button_id) override {
            _incr = 0;
        }
        
        inline void onMidPressed(std::int32_t button_id) override {
            _finetune = true;
        }
        
        inline void onMidReleased(std::int32_t button_id) override {
            _finetune = false;
        }
        
        inline void onDownPressed(std::int32_t button_id) override {
            _incr = _incr > -20 ? (_incr - 1) : -20;
            FMPatch::Operator& op = FMSynth::patch().op[button_id];
            
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
            switch(button_idx) {
                case 0: return View::InputFlags::UP | View::InputFlags::MID | View::InputFlags::DOWN;
                case 1: return View::InputFlags::UP | View::InputFlags::MID | View::InputFlags::DOWN;
                case 2: return View::InputFlags::UP | View::InputFlags::MID | View::InputFlags::DOWN;
                case 3: return View::InputFlags::UP | View::InputFlags::MID | View::InputFlags::DOWN;
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
                View::drawBackground(gfx);
            }
            
            if(_dirtyflags & _TOPAREA_FLAG) {
                gfx.drawFilledRectangle(2, 2, 216, 64, _BG_COLOR);
                const FMPatch& patch = FMSynth::patch();
                
                View::drawAlgorithm(gfx, patch.algorithm);
                
                View::drawHeader(gfx, patch.op[0].pitch.fixed ? "FREQ" : "RATIO", 
                                      patch.op[1].pitch.fixed ? "FREQ" : "RATIO", 
                                      patch.op[2].pitch.fixed ? "FREQ" : "RATIO", 
                                      patch.op[3].pitch.fixed ? "FREQ" : "RATIO");
            }
            
            for(std::uint32_t idx = 0; idx < 4; ++idx) {
                if(_dirtyflags & (1 << idx)) {
                    const FMPatch::Operator& op = FMSynth::patch().op[idx];
                    if(op.pitch.fixed) {
                        View::drawPitchFixed(gfx, idx, op.pitch.coarse, op.pitch.fine);
                    }
                    else {
                        View::drawPitchRatio(gfx, idx, op.pitch.coarse, op.pitch.fine);
                    }
                }
            }
            
            _dirtyflags = 0;
        }
    
    private:
    
        static constexpr std::uint8_t _BG_COLOR = 3;
        static constexpr std::uint8_t _TEXT_COLOR = 6;
        
        static constexpr std::int32_t _COL1_FLAG = 1<<0;
        static constexpr std::int32_t _COL2_FLAG = 1<<1;
        static constexpr std::int32_t _COL3_FLAG = 1<<2;
        static constexpr std::int32_t _COL4_FLAG = 1<<3;
        static constexpr std::int32_t _TOPAREA_FLAG = 1<<4;
        static constexpr std::int32_t _BGND_FLAG = 1<<5;
        
        std::uint8_t _dirtyflags;
        
        std::int8_t _incr;
        bool _finetune;
};
