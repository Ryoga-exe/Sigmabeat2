#pragma once
#include "../Common.hpp"

class Menu : public MyApp::Scene {
public:
    Menu(const InitData& init);
    void update() override;
    void draw() const override;

private:
    int32 m_musicNum;
    int32 m_activeIndex;
    double m_state;
};