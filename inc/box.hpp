/* box.hpp
 * the box that contains the slugs
*/

#pragma once

#include <vector>
#include <memory>

#include "drawablennet.hpp"

namespace slug {

    class Simulation;
    class Entity;
    class Slug;

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
            std::vector<std::unique_ptr<slug::Entity>> mEntities;
            DrawableNeuralNet mDefaultBrain;
            Slug *mClosestSlug;
    };

} // slug