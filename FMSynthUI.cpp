
#include <cstdint>
#include "FMSynthUI.h"
#include "UIButton.h"
#include "View.h"
#include "PatchView.h"
#include "LevelView.h"
#include "BrowseView.h"
#include "FreqView.h"
#include "OperatorView.h"

#include "fonts/lcdfont.h"
#include "images/piano_marker_1bpp.h"

Pomi::Graphics FMSynthUI::_gfx(&_pal565[0], lcdfont);
std::int8_t FMSynthUI::_focus_col(-1);
std::int8_t FMSynthUI::_focus_row(-1);
PushButton* FMSynthUI::_active_btn(nullptr);
PushButton FMSynthUI::_lhs_buttons[4]{ PushButton(0, 132, "BROWSE"), PushButton(33, 132, "PATCH"), PushButton(0, 154, "LEVELS"), PushButton(33, 154, "FREQS") };
ArrowButton FMSynthUI::_arrow_buttons[4]{ ArrowButton(66, 132), ArrowButton(88, 132), ArrowButton(110, 132), ArrowButton(132, 132) };
PushButton FMSynthUI::_rhs_buttons[4]{ PushButton(154, 132, "OP1"), PushButton(187, 132, "OP2"), PushButton(154, 154, "OP3"), PushButton(187, 154, "OP4") };
View* FMSynthUI::_cur_view(nullptr);
PatchView FMSynthUI::_patch_view;
LevelView FMSynthUI::_level_view;
BrowseView FMSynthUI::_browse_view;
FreqView FMSynthUI::_freq_view;
OperatorView FMSynthUI::_op_view;
bool FMSynthUI::_show_piano(false);
std::int8_t FMSynthUI::_selected_pianokey(60);
std::int8_t FMSynthUI::_active_pianokey(-1);

std::int32_t FMSynthUI::_pitch_bend_delta_lvl;
std::int32_t FMSynthUI::_pitch_bend_lvl_b;
std::uint32_t FMSynthUI::_pitch_bend_start_millis;
std::uint32_t FMSynthUI::_pitch_bend_end_millis;

void FMSynthUI::init() {
    _lhs_buttons[0].attachOnPressedCallback([](PushButton& btn){
        setCurrentView(&_browse_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    _lhs_buttons[1].attachOnPressedCallback([](PushButton& btn){
        if(currentView() == &_patch_view) {
            _patch_view.nextTab();
        }
        setCurrentView(&_patch_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    _lhs_buttons[2].attachOnPressedCallback([](PushButton& btn){
        setCurrentView(&_level_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    _lhs_buttons[3].attachOnPressedCallback([](PushButton& btn){
        setCurrentView(&_freq_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    
    _arrow_buttons[0].attachOnUpPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpPressed(0);
    });
    _arrow_buttons[0].attachOnUpReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpReleased(0);
    });
    _arrow_buttons[0].attachOnMidPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidPressed(0);
    });
    _arrow_buttons[0].attachOnMidReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidReleased(0);
    });
    _arrow_buttons[0].attachOnDownPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownPressed(0);
    });
    _arrow_buttons[0].attachOnDownReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownReleased(0);
    });
    
    _arrow_buttons[1].attachOnUpPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpPressed(1);
    });
    _arrow_buttons[1].attachOnUpReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpReleased(1);
    });
    _arrow_buttons[1].attachOnMidPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidPressed(1);
    });
    _arrow_buttons[1].attachOnMidReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidReleased(1);
    });
    _arrow_buttons[1].attachOnDownPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownPressed(1);
    });
    _arrow_buttons[1].attachOnDownReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownReleased(1);
    });
    
    _arrow_buttons[2].attachOnUpPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpPressed(2);
    });
    _arrow_buttons[2].attachOnUpReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpReleased(2);
    });
    _arrow_buttons[2].attachOnMidPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidPressed(2);
    });
    _arrow_buttons[2].attachOnMidReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidReleased(2);
    });
    _arrow_buttons[2].attachOnDownPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownPressed(2);
    });
    _arrow_buttons[2].attachOnDownReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownReleased(2);
    });
    
    _arrow_buttons[3].attachOnUpPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpPressed(3);
    });
    _arrow_buttons[3].attachOnUpReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onUpReleased(3);
    });
    _arrow_buttons[3].attachOnMidPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidPressed(3);
    });
    _arrow_buttons[3].attachOnMidReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onMidReleased(3);
    });
    _arrow_buttons[3].attachOnDownPressedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownPressed(3);
    });
    _arrow_buttons[3].attachOnDownReleasedCallback([](ArrowButton&){
        if(currentView() != nullptr) currentView()->onDownReleased(3);
    });
    
    _rhs_buttons[0].attachOnPressedCallback([](PushButton& btn){
        if(currentView() == &_op_view && _op_view.operatorIndex() == 0) {
            _op_view.nextTab();
        }
        _op_view.setOperatorIndex(0);
        setCurrentView(&_op_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    _rhs_buttons[1].attachOnPressedCallback([](PushButton& btn){
        if(currentView() == &_op_view && _op_view.operatorIndex() == 1) {
            _op_view.nextTab();
        }
        _op_view.setOperatorIndex(1);
        setCurrentView(&_op_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    _rhs_buttons[2].attachOnPressedCallback([](PushButton& btn){
        if(currentView() == &_op_view && _op_view.operatorIndex() == 2) {
            _op_view.nextTab();
        }
        _op_view.setOperatorIndex(2);
        setCurrentView(&_op_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    _rhs_buttons[3].attachOnPressedCallback([](PushButton& btn){
        if(currentView() == &_op_view && _op_view.operatorIndex() == 3) {
            _op_view.nextTab();
        }
        _op_view.setOperatorIndex(3);
        setCurrentView(&_op_view);
        
        if(_active_btn != nullptr && _active_btn != &btn) {
            _active_btn->deactivate();
            _active_btn->draw(_gfx);
        }
        _active_btn = &btn;
        btn.activate();
    });
    
    _focus_col = 0;
    _focus_row = 0;
    _active_btn = &_lhs_buttons[0];
    _active_btn->activate();
    _lhs_buttons[0].select();
    
    _gfx.clear(1);
    
    setCurrentView(&_browse_view);
    _drawPushButtons();
    _drawArrowButtons();
}

void FMSynthUI::update(std::uint32_t now) {
    if(Pomi::Input::pressed(Pomi::Input::Button::C)) {
        FMSynth::noteOff(_show_piano ? _active_pianokey : _selected_pianokey, 0);
        _active_pianokey = -1;
        
        _show_piano = !_show_piano;
        if(_show_piano) {
            _gfx.drawFilledRectangle(0, 132, 220, 44, 1);
            _drawPianoKeys();
        }
        else {
            _drawPushButtons();
            _drawArrowButtons();
        }
    }
    
    if(_show_piano) {
        bool redraw = false;
        if(Pomi::Input::pressed(Pomi::Input::Button::LEFT)) {
            _selected_pianokey -= (_selected_pianokey > _LOWEST_PIANOKEY) ? 1 : 0;
            redraw = true;
        }
        else if(Pomi::Input::pressed(Pomi::Input::Button::RIGHT)) {
            _selected_pianokey += (_selected_pianokey < _HIGHEST_PIANOKEY) ? 1 : 0;
            redraw = true;
        }
        else if(Pomi::Input::released(Pomi::Input::Button::LEFT | Pomi::Input::Button::RIGHT)) {
            if(_active_pianokey >= 0) {
                FMSynth::noteOn(_selected_pianokey, 127);
                FMSynth::noteOff(_active_pianokey, 0);
                _active_pianokey = _selected_pianokey;
                redraw = true;
            }
        }
        
        if(Pomi::Input::pressed(Pomi::Input::Button::B)) {
            if(_selected_pianokey != _active_pianokey) {  // Ignore repeated presses
                _active_pianokey = _selected_pianokey;
                FMSynth::noteOn(_active_pianokey, 127);
                redraw = true;
            }
        }
        else if(Pomi::Input::released(Pomi::Input::Button::B)) {
            FMSynth::noteOff(_active_pianokey, 0);
            _active_pianokey = -1;
            redraw = true;
        }
        
        constexpr std::uint32_t SUSTAIN_CONTROLLER_ID = 64;
        if(Pomi::Input::pressed(Pomi::Input::Button::A)) {
            FMSynth::setSustain(true);
        }
        else if(Pomi::Input::released(Pomi::Input::Button::A)) {
            FMSynth::setSustain(false);
        }
        
        constexpr std::int32_t PITCH_BEND_MILLIS = 200;
        constexpr std::int32_t INV_BEND_MILLIS_Q15 = (1 << 15) * 1.0 / PITCH_BEND_MILLIS;
        
        std::int32_t pitch = _pitch_bend_lvl_b;
        if(Pomi::Input::pressed(Pomi::Input::Button::UP | Pomi::Input::Button::DOWN) || Pomi::Input::released(Pomi::Input::Button::UP | Pomi::Input::Button::DOWN)) {
            std::int32_t next_lvl = 8192 * (Pomi::Input::held(Pomi::Input::Button::UP) - Pomi::Input::held(Pomi::Input::Button::DOWN));
            if(next_lvl != _pitch_bend_lvl_b) {
                _pitch_bend_delta_lvl = next_lvl - pitch;
                _pitch_bend_lvl_b = next_lvl;
                _pitch_bend_end_millis = now + PITCH_BEND_MILLIS;
            }
        }
        
        if(_pitch_bend_delta_lvl != 0) {
            std::int32_t delta_millis = _pitch_bend_end_millis - now;
            if(delta_millis > 0) {
                pitch = _pitch_bend_lvl_b - ((_pitch_bend_delta_lvl * delta_millis * INV_BEND_MILLIS_Q15) >> 15);
            }
            else {
                _pitch_bend_delta_lvl = 0;
            }
            FMSynth::setPitchBend(pitch);
        }
        
        if(redraw) {
            _drawPianoKeys();
        }
    }
    else {
        std::int8_t prev_col = _focus_col;
        std::int8_t prev_row = _focus_row;
        
        if((_focus_col > 0) && Pomi::Input::pressed(Pomi::Input::Button::LEFT)) {
            --_focus_col;
        }
        if((_focus_col < 7) && Pomi::Input::pressed(Pomi::Input::Button::RIGHT)) {
            ++_focus_col;
        }
        
        if((_focus_col < 2) || (_focus_col >= 6)) {
            if(Pomi::Input::pressed(Pomi::Input::Button::UP) && (_focus_row > 0)) {
                --_focus_row;
            }
            if(Pomi::Input::pressed(Pomi::Input::Button::DOWN) && (_focus_row < 1)) {
                ++_focus_row;
            }
        }
        
        if(Pomi::Input::pressed(Pomi::Input::Button::B)) {
            if(_active_pianokey < 0) {  // only handle first button press, ignore repeated presses
                FMSynth::noteOn(_selected_pianokey, 127);
                _active_pianokey = _selected_pianokey;
            }
        }
        else if(Pomi::Input::released(Pomi::Input::Button::B)) {
            FMSynth::noteOff(_selected_pianokey, 0);
            _active_pianokey = -1;
        }
        
        UIButton* btn = _findButton(_focus_col, _focus_row);
        if((_focus_col != prev_col) || (_focus_row != prev_row)) {
            UIButton* prev_btn = _findButton(prev_col, prev_row);
            if(prev_btn != nullptr) prev_btn->deselect();
            prev_btn->draw(_gfx);
            
            if(btn != nullptr) btn->select();
        }
        else {
            btn->update();
        }
        btn->draw(_gfx);
        
        if(_cur_view != nullptr) _cur_view->draw(_gfx);
    }
}

View* FMSynthUI::currentView() {
    return _cur_view;
}

void FMSynthUI::setCurrentView(View* view) {
    if(view != nullptr && view != _cur_view) {
        if(_cur_view != nullptr) _cur_view->leave();
        _cur_view = view;
        _cur_view->enter();
    }
    
    for(std::uint32_t idx = 0; idx < 4; ++idx) {
        std::uint8_t flags = static_cast<std::uint8_t>(_cur_view->getInputFlags(idx));
        _arrow_buttons[idx].setInputMode(flags & View::InputFlags::UP, flags & View::InputFlags::MID, flags & View::InputFlags::DOWN);
    }
    
    _drawArrowButtons();
}

UIButton* FMSynthUI::_findButton(std::int32_t col, std::int32_t row) {
    if(col >= 0) {
        if(col < 2) {
            return &_lhs_buttons[2*row + col];
        }
        else if(col < 6) {
            return &_arrow_buttons[col-2];
        }
        else if(col < 8) {
            return &_rhs_buttons[2*row + (col-6)];
        }
    }
    return nullptr;
}

void FMSynthUI::_drawPianoKeys() {
    for(std::uint32_t octave = 0; octave < 3; ++octave) {
        for(std::uint32_t idx = 0; idx < 12; ++idx) {
            const PianoKeyDrawable& piano_key = _piano_keys[idx];
            std::uint8_t color = (_LOWEST_PIANOKEY + 12 * octave + idx == _active_pianokey) ? 5 : piano_key.color;
            _gfx.drawImage1BPP(piano_key.image, color, 2 + 72 * octave + piano_key.x, 134, piano_key.flip_x);
            if(_LOWEST_PIANOKEY + 12 * octave + idx == _selected_pianokey) {
                _gfx.drawImage1BPP(piano_marker_1bpp, 5, 2 + 72 * octave + piano_key.marker_x, 134 + piano_key.marker_y);
            }
        }
    }
}

void FMSynthUI::_drawPushButtons() {
    for(std::int32_t idx = 0; idx < 4; ++idx) {
        _lhs_buttons[idx].draw(_gfx, true);
    }
    
    for(std::int32_t idx = 0; idx < 4; ++idx) {
        _rhs_buttons[idx].draw(_gfx, true);
    }
}
 
void FMSynthUI::_drawArrowButtons() {   
    for(std::int32_t idx = 0; idx < 4; ++idx) {
        _arrow_buttons[idx].draw(_gfx, true);
    }
}
