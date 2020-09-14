
#pragma once

// #include <Pokitto.h>
#include <cstdint>
#include "UIButton.h"
#include "pomi/Input.h"

#include "fonts/btnfont.h"
#include "images/pushbutton.h"
#include "images/pushbutton_press.h"

class PushButton final: public UIButton {
    
    public:
    
        constexpr PushButton(std::int32_t x, std::int32_t y, const char label[]): 
            UIButton(x, y), _redraw(true), _selected(false), _label(label), _pressed(false), _activated(false), _on_pressed_callback(nullptr)
        {}
        
        ~PushButton() = default;
        constexpr PushButton(const PushButton&) = default;
        PushButton& operator =(const PushButton&) = delete;
        
        inline void update() override {
            if(_selected) {
                if(Pomi::Input::pressed(Pomi::Input::Button::A)) {
                    _pressed = true;
                    _redraw = true;
                    
                    if(_on_pressed_callback != nullptr) _on_pressed_callback(*this);
                }
                else if(Pomi::Input::released(Pomi::Input::Button::A)) {
                    _pressed = false;
                    _redraw = true;
                }
            }
        }
        
        inline void draw(Pomi::Graphics& gfx, bool force_redraw=false) override {
            if(!_redraw && !force_redraw) return;
            
            _redraw = false;
            
            gfx.drawImage((_pressed ? pushbutton_press : pushbutton), UIButton::x(), UIButton::y());
            
            gfx.setFont(btnfont);
            std::int32_t x = UIButton::x() + (33 - 5 * std::strlen(_label) + 1) / 2;
            std::int32_t y = UIButton::y() + (_pressed ? 9 : 8);
            gfx.print(_label, x, y, _activated ? _ACT_TEXT_COLOR : _TEXT_COLOR);
            
            if(_selected) {
                UIButton::drawHighlight(gfx, 33, 22);
            }
        }
        
        inline bool isSelected() const override { return _selected; }
        inline void select() override {
            _selected = true;
            _redraw = true;
        }
        inline void deselect() override {
            _selected = false;
            _pressed = false;
            _redraw = true;
        }
        
        inline bool isActivated() const { return _activated; }
        inline void activate() {
            _activated = true;
            _redraw = true;
        }
        inline void deactivate() {
            _activated = false;
            _redraw = true;
        }
        
        inline void attachOnPressedCallback(void (*fn)(PushButton&)) { _on_pressed_callback = fn; }
    
    private:
    
        static constexpr std::uint16_t _TEXT_COLOR = 6;
        static constexpr std::uint16_t _ACT_TEXT_COLOR = 5;
        
        bool _redraw;
        bool _selected;
        
        const char* _label;
        
        bool _pressed;
        bool _activated;
        
        void (*_on_pressed_callback)(PushButton&);
};
