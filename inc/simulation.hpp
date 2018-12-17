/* simulation.hpp
 * class that contains everything
*/

#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "box.hpp"

namespace slug {

    class Simulation {
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

            // boxes
            std::vector<std::unique_ptr<slug::Box>> mBoxes;
    };

} // slug