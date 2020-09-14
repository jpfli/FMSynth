
#pragma once

#include <cstdint>
#include "pomi/Graphics.h"

class UIButton {
    
    public:
    
        constexpr UIButton(std::int32_t x, std::int32_t y): _x(x), _y(y) {}
        virtual ~UIButton() = default;
        constexpr UIButton(const UIButton&) = default;
        UIButton& operator =(const UIButton&) = delete;
        
        virtual void update() = 0;
        virtual void draw(Pomi::Graphics& gfx, bool force_redraw=false) = 0;
        
        virtual bool isSelected() const = 0;
        virtual void select() = 0;
        virtual void deselect() = 0;
        
        inline std::int32_t x() const { return _x; }
        inline std::int32_t y() const { return _y; }
        
        inline void drawHighlight(Pomi::Graphics& gfx, std::int32_t w, std::int32_t h) const {
            gfx.drawHLine(_x+1, _y, w-2, _HIGHLIGHT_COLOR);
            gfx.drawHLine(_x+1, _y + h - 1, w-2, _HIGHLIGHT_COLOR);
            gfx.drawVLine(_x, _y + 1, h - 2, _HIGHLIGHT_COLOR);
            gfx.drawVLine(_x + w - 1, _y + 1, h - 2, _HIGHLIGHT_COLOR);
        }
    
    private:
    
        static constexpr std::uint16_t _HIGHLIGHT_COLOR = 5;
        
        const std::int32_t _x;
        const std::int32_t _y;
};
