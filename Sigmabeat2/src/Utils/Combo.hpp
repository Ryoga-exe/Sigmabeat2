#pragma once
#include "Common.hpp"

namespace Utils {
    struct Combo {
        Combo() : m_combo(0), m_maxCombo(0) {

        }
        void init() {
            m_combo = 0;
            m_maxCombo = 0;
        }
        void update() {

        }
        int32 getCombo() const {
            return m_combo;
        }
        int32 getMaxCombo() const {
            return m_maxCombo;
        }
        Combo& operator =(const int& r) {
            m_combo = r;
            m_maxCombo = Max(m_maxCombo, m_combo);
            return *this;
        }
        Combo& operator =(const Combo& r) {
            m_combo = r.m_combo;
            m_maxCombo = Max(m_maxCombo, m_combo);
            return *this;
        }
        Combo operator++() {
            ++m_combo;
            m_maxCombo = Max(m_maxCombo, m_combo);
            return *this;
        }
        const Combo operator++(int) {
            Combo tmp = *this;
            ++(*this);
            return tmp;
        }
        operator int32() const {
            return m_combo;
        }
    private:
        int32 m_combo;
        int32 m_maxCombo;
    };
}
