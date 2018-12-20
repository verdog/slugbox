/* mouseinterface
 * a class that makes getting mouse info a little nicer
*/

#pragma once

#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace slug {

    class Simulation;

    class MouseInterface {
        public:
            MouseInterface(Simulation &sim);
            ~MouseInterface();

            bool isButtonPressed(sf::Mouse::Button button);
            bool isButtonReleased(sf::Mouse::Button button);
            bool isButtonPressedInstant(sf::Mouse::Button button);
            bool isButtonReleasedInstant(sf::Mouse::Button button);
            sf::Vector2f getLocalMousePosition();

        private:
            Simulation &mSim;
            std::unordered_map<sf::Mouse::Button, bool> mPressedReadyMap;
            std::unordered_map<sf::Mouse::Button, bool> mReleasedReadyMap;
    };

}