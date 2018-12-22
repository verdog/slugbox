/* mouseinterface
 * a class that makes getting mouse info a little nicer
*/

#include "mouseinterface.hpp"
#include "simulation.hpp"

namespace slug {

    MouseInterface::MouseInterface(Simulation &sim) 
    : mSim {sim}
    , mPressedReadyMap {
        {sf::Mouse::Left, true},
        {sf::Mouse::Right, true},
        {sf::Mouse::Middle, true},
        {sf::Mouse::XButton1, true},
        {sf::Mouse::XButton2, true}
    }
    , mReleasedReadyMap {
        {sf::Mouse::Left, false},
        {sf::Mouse::Right, false},
        {sf::Mouse::Middle, false},
        {sf::Mouse::XButton1, false},
        {sf::Mouse::XButton2, false}
    }
    {
        //
    }

    MouseInterface::~MouseInterface() {
        //
    }

    bool MouseInterface::isButtonPressed(sf::Mouse::Button button) {
        return sf::Mouse::isButtonPressed(button);
    }

    bool MouseInterface::isButtonReleased(sf::Mouse::Button button) {
        return !isButtonPressed(button);
    }

    bool MouseInterface::isButtonPressedInstant(sf::Mouse::Button button) {
        if (isButtonPressed(button) && mPressedReadyMap.at(button)) {
            mPressedReadyMap.at(button) = false;
            return true;
        } else if (isButtonReleased(button)) {
            mPressedReadyMap.at(button) = true;
            return false;
        } else {
            return false;
        }
    }

    bool MouseInterface::isButtonReleasedInstant(sf::Mouse::Button button) {
        if (isButtonReleased(button) && mReleasedReadyMap.at(button)) {
            mReleasedReadyMap.at(button) = false;
            return true;
        } else if (isButtonPressed(button)) {
            mReleasedReadyMap.at(button) = true;
            return false;
        } else {
            return false;
        }
    }

    sf::Vector2f MouseInterface::getLocalMousePosition() {
        return sf::Vector2f(sf::Mouse::getPosition(mSim.mRenderWindow).x, sf::Mouse::getPosition(mSim.mRenderWindow).y);
    }

}
