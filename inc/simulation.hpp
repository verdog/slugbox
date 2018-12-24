/* simulation.hpp
 * class that contains everything
*/

#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "mouseinterface.hpp"

namespace slug {

    class Box;

    class Simulation {
        friend class Box;
        friend class MouseInterface;
        public:
            Simulation();
            ~Simulation();

            void start();
            void stop();
        private:
            void run();

            // window
            sf::Vector2i mResolution;
            sf::RenderWindow mRenderWindow;
            bool mWindowHasFocus;

            // interfaces
            MouseInterface mMouseInterface;

            // boxes
            std::vector<std::unique_ptr<slug::Box>> mBoxes;
    };

} // slug