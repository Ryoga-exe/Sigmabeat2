#include "Result.hpp"

Result::Result(const InitData& init)
    : IScene(init), m_delta(0.0) {


    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

}

Result::~Result() {
    
}

void Result::update() {
    
}

void Result::draw() const {

}
