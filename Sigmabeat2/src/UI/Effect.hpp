#pragma once
#include "Common.hpp"

namespace UI {
    struct JudgeEffect : IEffect {
        Vec2 m_start;
        String m_judge;
        Font m_font;
        Color m_color;

        JudgeEffect(const Vec2& start, String judge, const Font& font, const Color& color = Palette::Whitesmoke)
            : m_start{ start }, m_judge{ judge }, m_font{ font }, m_color{ color } {}

        bool update(double t) override {
            const double e = EaseOutExpo(t * 2.0);

            m_font(m_judge).drawAt(m_start.movedBy(0, -40 * e).movedBy(4, 4), ColorF(0.0, 0.5 * (1.0 - t * 2.0)));
            m_font(m_judge).drawAt(m_start.movedBy(0, -40 * e), ColorF(m_color, 1.0 - Max(0.0, 4.0 * t - 1.0)));

            return (t < 0.5);
        }
    };
}
