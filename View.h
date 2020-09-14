
#pragma once

#include <cstdint>
#include "pomi/Graphics.h"
#include "FMPatch.h"
#include "FMSynth.h"
#include "StringBuffer.h"

#include "images/button_b_1bpp.h"
#include "images/button_c_1bpp.h"
#include "images/note_1bpp.h"
#include "images/keyboard_1bpp.h"
#include "images/algo1_1bpp.h"
#include "images/algo2_1bpp.h"
#include "images/algo3_1bpp.h"
#include "images/algo4_1bpp.h"
#include "images/algo5_1bpp.h"
#include "images/algo6_1bpp.h"
#include "images/algo7_1bpp.h"
#include "images/algo8_1bpp.h"
#include "images/algo9_1bpp.h"
#include "images/algo10_1bpp.h"
#include "images/algo11_1bpp.h"

class View {
    
    public:
    
        enum InputFlags { UP = 1<<0, MID = 1<<1, DOWN = 1<<2 };
        
        constexpr View() {}
        virtual ~View() = default;
        constexpr View(const View&) = default;
        View& operator =(const View&) = delete;
        
        virtual void onUpPressed(std::int32_t button_id) = 0;
        virtual void onUpReleased(std::int32_t button_id) = 0;
        virtual void onMidPressed(std::int32_t button_id) = 0;
        virtual void onMidReleased(std::int32_t button_id) = 0;
        virtual void onDownPressed(std::int32_t button_id) = 0;
        virtual void onDownReleased(std::int32_t button_id) = 0;
        
        virtual void enter() = 0;
        virtual void leave() = 0;
        virtual void draw(Pomi::Graphics& gfx, bool force_redraw=false) = 0;
        
        virtual std::uint8_t getInputFlags(std::uint32_t button_idx) = 0;
        
        static void drawBackground(Pomi::Graphics& gfx, std::uint8_t num_tabs=0, std::uint8_t selected_tab=0) {
            gfx.drawFilledRectangle(2, 2, 216, 128, _BG_COLOR);
            
            gfx.drawFilledRectangle(21, 66, 2, 64, _TEXT_COLOR);
            gfx.drawFilledRectangle(65, 66, 2, 64, _TEXT_COLOR);
            gfx.drawFilledRectangle(109, 66, 2, 64, _TEXT_COLOR);
            gfx.drawFilledRectangle(153, 66, 2, 64, _TEXT_COLOR);
            gfx.drawFilledRectangle(197, 66, 2, 64, _TEXT_COLOR);
            
            // Draw tab icons
            for(std::uint32_t idx = 0; idx < num_tabs; ++idx) {
                std::int32_t y = 131 - 13 * num_tabs;
                gfx.drawHLine(9, y + 13 * idx, 4, _TEXT_COLOR);
                gfx.drawHLine(9, y + 13 * idx + 10, 4, _TEXT_COLOR);
                if(idx == static_cast<std::uint8_t>(selected_tab)) {
                    gfx.drawFilledRectangle(8, y + 13 * idx + 1, 6, 9, _TEXT_COLOR);
                }
                else {
                    gfx.drawVLine(8, y + 13 * idx + 1, 9, _TEXT_COLOR);
                    gfx.drawVLine(13, y + 13 * idx + 1, 9, _TEXT_COLOR);
                }
            }
            
            gfx.drawImage1BPP(button_b_1bpp, _TEXT_COLOR, 201, 107);
            gfx.drawImage1BPP(note_1bpp, _TEXT_COLOR, 210, 105);
            
            gfx.drawImage1BPP(button_c_1bpp, _TEXT_COLOR, 201, 120);
            gfx.drawImage1BPP(keyboard_1bpp, _TEXT_COLOR, 210, 118);
        }
        
        static inline void drawHeader(Pomi::Graphics& gfx, const char* label1, const char* label2="", const char* label3="", const char* label4="") {
            gfx.drawFilledRectangle(22, 53, 176, 11, _BG_COLOR);
            
            gfx.print(label1, (1 * 44) - 7 * strlen(label1) / 2, 53, _TEXT_COLOR);
            gfx.print(label2, (2 * 44) - 7 * strlen(label2) / 2, 53, _TEXT_COLOR);
            gfx.print(label3, (3 * 44) - 7 * strlen(label3) / 2, 53, _TEXT_COLOR);
            gfx.print(label4, (4 * 44) - 7 * strlen(label4) / 2, 53, _TEXT_COLOR);
        }
        
        static inline void drawValue(Pomi::Graphics& gfx, std::uint8_t column, std::int8_t value) {
            gfx.drawFilledRectangle(23 + column * 44, 92, 42, 11, _BG_COLOR);
            
            std::int32_t x0 = (1 + column) * 44;
            
            StringBuffer<5> text;
            text.append(value);
            gfx.print(text, x0 - (7 * text.length()) / 2, 92, _TEXT_COLOR);
        }
        
        static inline void drawSignedValue(Pomi::Graphics& gfx, std::uint8_t column, std::int8_t value) {
            gfx.drawFilledRectangle(23 + column * 44, 92, 42, 11, _BG_COLOR);
            
            std::int32_t x0 = (1 + column) * 44;
            
            StringBuffer<5> text(value > 0 ? "+" : "");
            text.append(value);
            gfx.print(text, x0 - (7 * text.length()) / 2, 92, _TEXT_COLOR);
        }
        
        static inline void drawLabel(Pomi::Graphics& gfx, std::uint8_t column, const char* label) {
            gfx.drawFilledRectangle(23 + column * 44, 92, 42, 11, _BG_COLOR);
            
            StringBuffer<5> text(label);
            gfx.print(text, (1 + column) * 44 - (7 * text.length()) / 2, 92, _TEXT_COLOR);
        }
        
        static inline void drawLevel(Pomi::Graphics& gfx, std::uint8_t column, std::uint8_t level) {
            gfx.drawFilledRectangle(23 + column * 44, 66, 42, 63, _BG_COLOR);
            
            std::int32_t h = level > 1 ? (level * 50 / 100) : 1;
            gfx.drawFilledRectangle(33 + column * 44, 116 - h, 22, h, _TEXT_COLOR);
            
            StringBuffer<5> text;
            text.append(level);
            gfx.print(text, (1 + column) * 44 - (7 * text.length()) / 2, 118, _TEXT_COLOR);
        }
        
        static inline void drawPitchRatio(Pomi::Graphics& gfx, std::uint8_t column, std::int8_t coarse, std::int8_t fine) {
            gfx.drawFilledRectangle(23 + column * 44, 92, 42, 11, _BG_COLOR);
            
            std::int32_t x0 = (1 + column) * 44;
            
            StringBuffer<5> text;
            text.append(coarse);
            
            std::uint32_t dot = text.length();
            text.append(100 + fine); // add 100 to add leading zeros; "1" is replaced with a "." below
            text.replace(dot, dot + 1, ".");
            gfx.print(text, x0 - (7 * text.length()) / 2, 92, _TEXT_COLOR);
        }
        
        static inline void drawPitchFixed(Pomi::Graphics& gfx, std::uint8_t column, std::int8_t coarse, std::int8_t fine) {
            gfx.drawFilledRectangle(23 + column * 44, 92, 42, 11, _BG_COLOR);
            
            std::int32_t freq_Q15 = FMSynth::toFixedFrequency(coarse, fine).internal();
            
            StringBuffer<5> text;
            text.append(freq_Q15 >> 15);
            
            std::uint32_t dot = text.length();
            text.append(1000 + ((1000 * (freq_Q15 & 0x7fff)) >> 15));
            text.replace(dot, dot + 1, "."); // replace the leading "1" (added above) with "."
            
            std::int32_t x0 = (1 + column) * 44;
            gfx.print(text, x0 - (7 * text.length()) / 2, 92, _TEXT_COLOR);
        }
        
        static inline void drawLFOFrequency(Pomi::Graphics& gfx, std::uint8_t column, std::uint8_t val) {
            gfx.drawFilledRectangle(23 + column * 44, 92, 42, 11, _BG_COLOR);
            
            std::int32_t freq = FMSynth::toLFOFrequency(val).internal();
            freq = (freq * 100) >> 15;
            StringBuffer<5> text;
            text.append(freq / 100);
            
            std::uint32_t dot = text.length();
            text.append(freq % 100 + 100);
            text.replace(dot, dot + 1, "."); // replace the leading "1" (added above) with "."
            
            std::int32_t x0 = (1 + column) * 44;
            gfx.print(text, x0 - (7 * text.length()) / 2, 92, _TEXT_COLOR);
        }
        
        static inline void drawAlgorithm(Pomi::Graphics& gfx, std::int32_t algorithm) {
            gfx.drawFilledRectangle(2, 2, 216, 50, _BG_COLOR);
            gfx.drawImage1BPP(_algo_images[algorithm <= 1 ? 0 : (algorithm >= 11 ? 10 : (algorithm - 1))], _TEXT_COLOR, 33, 8);
        }
        
        template<signed Max>
        static inline std::int32_t increaseLevel(std::int32_t level, std::int32_t incr) {
            return (level + (incr + 3) / 4) < Max ? (level + (incr + 3) / 4) : Max;
        }
        
        template<signed Min>
        static inline std::int32_t decreaseLevel(std::int32_t level, std::int32_t incr) {
            return (level + (incr - 3) / 4) > Min ? (level + (incr - 3) / 4) : Min;
        }
    
    private:
    
        static constexpr std::uint8_t _BG_COLOR = 3;
        static constexpr std::uint8_t _TEXT_COLOR = 6;
        
        static constexpr const std::uint8_t* _algo_images[11] = {
            algo1_1bpp, algo2_1bpp, algo3_1bpp, algo4_1bpp, algo5_1bpp, algo6_1bpp,
            algo7_1bpp, algo8_1bpp, algo9_1bpp, algo10_1bpp, algo11_1bpp
        };
};
