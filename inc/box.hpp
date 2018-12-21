/* box.hpp
 * the box that contains the slugs
*/

#pragma once

#include <vector>
#include <memory>

namespace slug {

    class Simulation;
    class Entity;

    class Box {
        public:
            Box(Simulation &sim);
            ~Box();

            void spawn();
            void handleInput();
            void update(const sf::Time &dTime);
            void drawContents(sf::RenderWindow &window);
        private:
            Simulation &mSim;
            std::vector<std::shared_ptr<slug::Entity>> mEntities;
    };

} // slug