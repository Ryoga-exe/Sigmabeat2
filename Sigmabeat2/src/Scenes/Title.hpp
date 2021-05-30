#pragma once
#include "Common.hpp"

class Title : public MyApp::Scene {
public:
    Title(const InitData& init);
    ~Title();
    void update() override;
    void draw() const override;

private:
    double m_delta = 0.0;
    const Texture m_titleLogo;
};