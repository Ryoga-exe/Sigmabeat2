#pragma once
#include "../Common.hpp"

class Setup : public MyApp::Scene {
public:
	Setup(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Stopwatch m_stopwatch;
	const Texture m_authorLogo;
};