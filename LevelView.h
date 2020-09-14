
#pragma once

#include <cstdint>
#include "View.h"
#include "pomi/Graphics.h"
#include "FMSynth.h"

#include "fonts/lcdfont.h"

class LevelView final: public View {
    
    public:
    
        constexpr LevelView(): View(), _dirtyflags(~0), _incr(0) {}
        ~LevelView() = default;
        constexpr LevelView(const LevelView&) = default;
        LevelView& operator =(const LevelView&) = delete;
        
        inline void onUpPressed(std::int32_t button_id) override {
            _incr = _incr < 20 ? (_incr + 1) : 20;
            
            if(button_id >= 0 && button_id < 4) {
                FMPatch::Operator& op = FMSynth::patch().op[button_id];
                op.level = View::increaseLevel<100>(op.level, _incr);
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
            
            if(button_id >= 0 && button_id < 4) {
                FMPatch::Operator& op = FMSynth::patch().op[button_id];
                op.level = View::decreaseLevel<0>(op.level, _incr);
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
                case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                case 1: return View::InputFlags::UP | View::InputFlags::DOWN;
                case 2: return View::InputFlags::UP | View::InputFlags::DOWN;
                case 3: return View::InputFlags::UP | View::InputFlags::DOWN;
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
                View::drawAlgorithm(gfx, FMSynth::patch().algorithm);
            }
            
            for(std::uint32_t idx = 0; idx < 4; ++idx) {
                if(_dirtyflags & (1 << idx)) {
                    View::drawLevel(gfx, idx, FMSynth::patch().op[idx].level);
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
};
