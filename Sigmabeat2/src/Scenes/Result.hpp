#pragma once
#include "Common.hpp"

class Result : public MyApp::Scene {
public:
    Result(const InitData& init);
    ~Result();
    void update() override;
    void draw() const override;

private:
    double m_delta = 0.0;
    const Texture m_titleLogo;
};
