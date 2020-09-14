
#pragma once

#include <cstdint>
#include "UIButton.h"
#include "pomi/Input.h"

#include "images/arrowbutton_1bpp.h"
#include "images/button_a_1bpp.h"

class ArrowButton final: public UIButton {
    
    public:
    
        constexpr ArrowButton(std::int32_t x, std::int32_t y): 
            UIButton(x, y), _redraw(true), _selected(false), _btn_flags(0), 
            _on_up_pressed_callback(nullptr), _on_up_released_callback(nullptr), 
            _on_mid_pressed_callback(nullptr), _on_mid_released_callback(nullptr), 
            _on_down_pressed_callback(nullptr), _on_down_released_callback(nullptr), 
            _up_enabled(true), _mid_enabled(false), _down_enabled(true)
        {}
        
        ~ArrowButton() = default;
        constexpr ArrowButton(const ArrowButton&) = default;
        ArrowButton& operator =(const ArrowButton&) = delete;
        
        inline void update() override {
            if(_selected) {
                if(Pomi::Input::pressed(Pomi::Input::Button::UP)) {
                    _redraw = true;
                    
                    if(_on_up_pressed_callback != nullptr) _on_up_pressed_callback(*this);
                }
                else if(Pomi::Input::released(Pomi::Input::Button::UP)) {
                    _redraw = true;
                    if(_on_up_released_callback != nullptr) _on_up_released_callback(*this);
                }
                
                if(Pomi::Input::pressed(Pomi::Input::Button::DOWN)) {
                    _redraw = true;
                    
                    if(_on_down_pressed_callback != nullptr) _on_down_pressed_callback(*this);
                }
                else if(Pomi::Input::released(Pomi::Input::Button::DOWN)) {
                    _redraw = true;
                    if(_on_down_released_callback != nullptr) _on_down_released_callback(*this);
                }
                
                if(Pomi::Input::pressed(Pomi::Input::Button::A)) {
                    _redraw = true;
                    
                    if(_on_mid_pressed_callback != nullptr) _on_mid_pressed_callback(*this);
                }
                else if(Pomi::Input::released(Pomi::Input::Button::A)) {
                    _redraw = true;
                    if(_on_mid_released_callback != nullptr) _on_mid_released_callback(*this);
                }
                
                _btn_flags = (Pomi::Input::held(Pomi::Input::Button::UP) ? ButtonFlags::UP : 0) | 
                             (Pomi::Input::held(Pomi::Input::Button::A) ? ButtonFlags::MID : 0) | 
                             (Pomi::Input::held(Pomi::Input::Button::DOWN) ? ButtonFlags::DOWN : 0);
            }
        }
        
        inline void draw(Pomi::Graphics& gfx, bool force_redraw=false) override {
            if(!_redraw && !force_redraw) return;
            _redraw = false;
            
            gfx.drawFilledRectangle(UIButton::x(), UIButton::y(), 22, 44, _BG_COLOR);
            
            if(_up_enabled) {
                std::uint8_t color = (_btn_flags & ButtonFlags::UP) ? _ACTIVE_COLOR : _FG_COLOR;
                gfx.drawImage1BPP(arrowbutton_1bpp, color, UIButton::x() + 5, UIButton::y() + 7);
            }
            
            if(_down_enabled) {
                std::uint8_t color = (_btn_flags & ButtonFlags::DOWN) ? _ACTIVE_COLOR : _FG_COLOR;
                gfx.drawImage1BPP(arrowbutton_1bpp, color, UIButton::x() + 5, UIButton::y() + 30, false, true);
            }
            
            if(_mid_enabled) {
                std::uint8_t color = (_btn_flags & ButtonFlags::MID) ? _ACTIVE_COLOR : _FG_COLOR;
                gfx.drawImage1BPP(button_a_1bpp, color, UIButton::x() + 7, UIButton::y() + 18);
            }
            
            if(_selected) {
                UIButton::drawHighlight(gfx, 22, 44);
            }
        }
        
        inline bool isSelected() const override { return _selected; }
        inline void select() override {
            _selected = true;
            _redraw = true;
        }
        inline void deselect() override {
            _selected = false;
            _btn_flags = 0;
            _redraw = true;
        }
        
        inline void attachOnUpPressedCallback(void (*fn)(ArrowButton&)) { _on_up_pressed_callback = fn; }
        inline void attachOnUpReleasedCallback(void (*fn)(ArrowButton&)) { _on_up_released_callback = fn; }
        inline void attachOnMidPressedCallback(void (*fn)(ArrowButton&)) { _on_mid_pressed_callback = fn; }
        inline void attachOnMidReleasedCallback(void (*fn)(ArrowButton&)) { _on_mid_released_callback = fn; }
        inline void attachOnDownPressedCallback(void (*fn)(ArrowButton&)) { _on_down_pressed_callback = fn; }
        inline void attachOnDownReleasedCallback(void (*fn)(ArrowButton&)) { _on_down_released_callback = fn; }
        
        inline void setInputMode(bool up, bool mid, bool down) {
            _up_enabled = up;
            _mid_enabled = mid;
            _down_enabled = down;
            _redraw = true;
        }
    
    private:
    
        enum ButtonFlags { UP = 1<<0, MID = 1<<1, DOWN = 1<<2 };
        
        static constexpr std::uint16_t _BG_COLOR = 1;
        static constexpr std::uint16_t _FG_COLOR = 6;
        static constexpr std::uint16_t _ACTIVE_COLOR = 5;
        
        bool _redraw;
        bool _selected;
        std::uint8_t _btn_flags;
        
        bool _up_enabled;
        bool _mid_enabled;
        bool _down_enabled;
        
        void (*_on_up_pressed_callback)(ArrowButton&);
        void (*_on_up_released_callback)(ArrowButton&);
        void (*_on_mid_pressed_callback)(ArrowButton&);
        void (*_on_mid_released_callback)(ArrowButton&);
        void (*_on_down_pressed_callback)(ArrowButton&);
        void (*_on_down_released_callback)(ArrowButton&);
};
