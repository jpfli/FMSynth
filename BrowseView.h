
#pragma once

#include <cstdint>
#include "View.h"
#include "pomi/Graphics.h"
#include "StringBuffer.h"
#include "FMSynth.h"

#include "fonts/lcdfont.h"

class BrowseView final: public View {
    
    public:
    
        constexpr BrowseView(): View(), _dirtyflags(~0), _bank_num(1), _patch_num(1), _export_done(false) {}
        ~BrowseView() = default;
        constexpr BrowseView(const BrowseView&) = default;
        BrowseView& operator =(const BrowseView&) = delete;
        
        inline void onUpPressed(std::int32_t button_id) override {
            switch(button_id) {
                case 0:
                
                    if(_bank_num < FMSynth::numBanks()) {
                        ++_bank_num;
                        _patch_num = 1;
                        FMSynth::selectPatch(_bank_num - 1, _patch_num - 1);
                        _export_done = false;
                        _dirtyflags |= _TOPAREA_FLAG | _COL2_FLAG | _COL3_FLAG;
                    }
                    break;
                
                case 1:
                
                    if(_patch_num < FMSynth::numPatches(_bank_num - 1)) {
                        ++_patch_num;
                        FMSynth::selectPatch(_bank_num - 1, _patch_num - 1);
                        _export_done = false;
                        _dirtyflags |= _TOPAREA_FLAG | _COL2_FLAG | _COL3_FLAG;
                    }
                    break;
                
                case 3:
                
                    FMSynth::setMIDIChannel(FMSynth::MIDIChannel() + 1);
                    break;
            }
            _dirtyflags |= (1 << button_id);
        }
        
        inline void onUpReleased(std::int32_t button_id) override {
            
        }
        
        inline void onMidPressed(std::int32_t button_id) override {
            switch(button_id) {
                case 2:
                
                    if(!_export_done) {
                        _export_done = FMSynth::sendMIDIPatchDump();
                        
                        _dirtyflags |= (1 << button_id);
                    }
                    break;
            }
        }
        
        inline void onMidReleased(std::int32_t button_id) override {
            
        }
        
        inline void onDownPressed(std::int32_t button_id) override {
            switch(button_id) {
                case 0:
                
                    if(_bank_num > 1) {
                        --_bank_num;
                        _patch_num = 1;
                        FMSynth::selectPatch(_bank_num - 1, _patch_num - 1);
                        _export_done = false;
                        _dirtyflags |= _TOPAREA_FLAG | _COL2_FLAG | _COL3_FLAG;
                    }
                    break;
                
                case 1:
                
                    if(_patch_num > 1) {
                        --_patch_num;
                        FMSynth::selectPatch(_bank_num - 1, _patch_num - 1);
                        _export_done = false;
                        _dirtyflags |= _TOPAREA_FLAG | _COL2_FLAG | _COL3_FLAG;
                    }
                    break;
                
                case 3:
                
                    FMSynth::setMIDIChannel(FMSynth::MIDIChannel() - 1);
                    break;
            }
            _dirtyflags |= (1 << button_id);
        }
        
        inline void onDownReleased(std::int32_t button_id) override {
            
        }
        
        inline void enter() override {
            _export_done = false;
            _dirtyflags = ~0;
        }
        
        inline void leave() override {
            
        }
        
        inline std::uint8_t getInputFlags(std::uint32_t button_idx) override {
            switch(button_idx) {
                case 0: return View::InputFlags::UP | View::InputFlags::DOWN;
                case 1: return View::InputFlags::UP | View::InputFlags::DOWN;
                case 2: return View::InputFlags::MID;
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
                gfx.drawFilledRectangle(2, 2, 216, 50, _BG_COLOR);
                
                StringBuffer<5> text;
                text.append(_patch_num);
                text.append(".");
                std::int32_t x = 110 - 7*16/2;
                gfx.print(text, x - 7*text.length(), 21, _TEXT_COLOR);
                gfx.print(FMSynth::patch().name, x, 21, _TEXT_COLOR);
                
                View::drawHeader(gfx, "BANK", "PATCH", "DUMP", "CHN");
            }
            
            if(_dirtyflags & _COL1_FLAG) {
                View::drawValue(gfx, 0, _bank_num);
            }
            if(_dirtyflags & _COL2_FLAG) {
                View::drawValue(gfx, 1, _patch_num);
            }
            if(_dirtyflags & _COL3_FLAG) {
                View::drawLabel(gfx, 2, _export_done ? "DONE" : "SEND");
            }
            if(_dirtyflags & _COL4_FLAG) {
                View::drawValue(gfx, 3, FMSynth::MIDIChannel() + 1);
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
        
        std::int8_t _bank_num;
        std::int8_t _patch_num;
        
        bool _export_done;
};
