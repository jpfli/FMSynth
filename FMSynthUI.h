
#pragma once

#include <cstdint>
#include <array>
#include <USBMIDI.h>
#include "pomi/Graphics.h"
#include "UIButton.h"
#include "PushButton.h"
#include "ArrowButton.h"
#include "View.h"
#include "PatchView.h"
#include "LevelView.h"
#include "BrowseView.h"
#include "FreqView.h"
#include "OperatorView.h"

#include "palette.h"
#include "images/piano_c_1bpp.h"
#include "images/piano_d_1bpp.h"
#include "images/piano_g_1bpp.h"
#include "images/piano_black_1bpp.h"

class FMSynthUI {
    
    public:
    
        FMSynthUI() = delete;
        ~FMSynthUI() = delete;
        FMSynthUI(const FMSynthUI&) = delete;
        FMSynthUI& operator =(const FMSynthUI&) = delete;
        
        static void init();
        static void update(std::uint32_t now);
        
        static View* currentView();
        static void setCurrentView(View* view);
        
        static inline Pomi::Graphics& graphics() { return _gfx; }
    
    private:
    
        static constexpr std::int8_t _LOWEST_PIANOKEY = 48;
        static constexpr std::int8_t _HIGHEST_PIANOKEY = 48 + 12 * 3 - 1;
        
        struct PianoKeyDrawable {
            const std::uint8_t* image;
            std::uint8_t color;
            
            std::int8_t x;
            bool flip_x;
            
            std::int8_t marker_x;
            std::int8_t marker_y;
        };
        
        static constexpr auto _pal565 = []{
            std::array<std::uint16_t, 16> arr{};
            for(std::size_t idx = 0; idx < 16; ++idx) {
                std::uint8_t red = palette[3 * idx + 0];
                std::uint8_t green = palette[3 * idx + 1];
                std::uint8_t blue = palette[3 * idx + 2];
                arr[idx] = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            }
            return arr;
        }();
        
        static UIButton* _findButton(std::int32_t col, std::int32_t row);
        
        static void _drawPianoKeys();
        static void _drawPushButtons();
        static void _drawArrowButtons();
        
        static Pomi::Graphics _gfx;
        
        static std::int8_t _focus_col;
        static std::int8_t _focus_row;
        static PushButton* _active_btn;
        
        static PushButton _lhs_buttons[4];
        static ArrowButton _arrow_buttons[4];
        static PushButton _rhs_buttons[4];
        
        static View* _cur_view;
        static PatchView _patch_view;
        static LevelView _level_view;
        static BrowseView _browse_view;
        static FreqView _freq_view;
        static OperatorView _op_view;
        
        static bool _show_piano;
        static std::int8_t _selected_pianokey;
        static std::int8_t _active_pianokey;
        
        static std::int32_t _pitch_bend_delta_lvl;
        static std::int32_t _pitch_bend_lvl_b;
        static std::uint32_t _pitch_bend_start_millis;
        static std::uint32_t _pitch_bend_end_millis;
        
        static constexpr PianoKeyDrawable _piano_keys[12] = {
            {piano_c_1bpp, 6, 1, false, 1, 27}, 
            {piano_black_1bpp, 0, 6, false, 5, 7}, 
            {piano_d_1bpp, 6, 11, false, 11, 27}, 
            {piano_black_1bpp, 0, 18, false, 17, 7}, 
            {piano_c_1bpp, 6, 21, true, 21, 27}, 
            {piano_c_1bpp, 6, 31, false, 31, 27}, 
            {piano_black_1bpp, 0, 36, false, 35, 7}, 
            {piano_g_1bpp, 6, 41, false, 42, 27}, 
            {piano_black_1bpp, 0, 48, false, 47, 7}, 
            {piano_g_1bpp, 6, 52, true, 52, 27}, 
            {piano_black_1bpp, 0, 60, false, 59, 7}, 
            {piano_c_1bpp, 6, 63, true, 63, 27}, 
        };
};
