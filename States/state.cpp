#include "state.h"


State::State(sf::RenderWindow *window, std::map<std::string, int>* supportedKeys, std::stack<std::unique_ptr<State>>& states)
    : window_(window), supportedKeys_(supportedKeys), states_(states) {
    quit_ = false;
    paused_ = false;
    keyStates_.fill(false);
}

State::~State() = default;

const bool& State::getQuit() const {
    return quit_;
}

void State::endState() {
    quit_ = true;
}

void State::pauseState() {
    paused_ = true;
}

void State::unpauseState() {
    paused_ = false;
}

void State::updateMousePositions() {
    mousePosScreen_ = sf::Mouse::getPosition();
    mousePosWindow_ = sf::Mouse::getPosition(*window_);
    mousePosView_ = window_->mapPixelToCoords(mousePosWindow_);
}
