/* mouseinterface
 * a class that makes getting mouse info a little nicer
*/

#include "mouseinterface.hpp"
#include "simulation.hpp"

namespace slug {

    MouseInterface::MouseInterface(Simulation &sim) 
    : mSim {sim}
    , mPressedReady {true}
    , mReleasedReady {false}
    {

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
        if (isButtonPressed(button) && mPressedReady) {
            mPressedReady = false;
            return true;
        } else if (isButtonReleased(button)) {
            mPressedReady = true;
            return false;
        } else {
            return false;
        }
    }

    bool MouseInterface::isButtonReleasedInstant(sf::Mouse::Button button) {
        if (isButtonReleased(button) && mReleasedReady) {
            mReleasedReady = false;
            return true;
        } else if (isButtonPressed(button)) {
            mReleasedReady = true;
            return false;
        } else {
            return false;
        }
    }

    sf::Vector2f MouseInterface::getLocalMousePosition() {
        return sf::Vector2f(sf::Mouse::getPosition(mSim.mRenderWindow).x, sf::Mouse::getPosition(mSim.mRenderWindow).y);
    }

}
