#pragma once
#include "Common.hpp"

class Game : public MyApp::Scene {
public:
    Game(const InitData& init);
    void update() override;
    void draw() const override;

private:

};