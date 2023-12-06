#include "state.h"


State::State(sf::RenderWindow *window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : window_(window), quit_(false), supportedKeys_(supportedKeys), states_(states) {}

State::~State() = default;

const bool& State::getQuit() const {
    return quit_;
}

void State::endState() {
    quit_ = true;
}

void State::updateMousePositions() {
    mousePosScreen_ = sf::Mouse::getPosition();
    mousePosWindow_ = sf::Mouse::getPosition(*window_);
    mousePosView_ = window_->mapPixelToCoords(mousePosWindow_);
}