#pragma once
#include "../Common.hpp"

class Title : public MyApp::Scene {
public:
	Title(const InitData& init);
	void update() override;
	void draw() const override;

private:
	
};