#pragma once
#include "Common.hpp"

class Game : public MyApp::Scene {
public:
    Game(const InitData& init);
    void update() override;
    void draw() const override;

    void drawLane() const;

private:
    struct Homography {
        Float4 m1;
        Float4 m2;
        Float4 m3;
    };

    const Size FieldSize = { 500, 1000 };
    MSRenderTexture m_fieldRT;

    const VertexShader VS;
    const PixelShader PS;
};
