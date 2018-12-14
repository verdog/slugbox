/* box.hpp
 * the box that contains the slugs
*/

#pragma once

#include <vector>

#include "entity.hpp"

namespace slug {

    class Box {
        public:
            Box();
            ~Box();

            void spawn();
            void update();
            void drawContents(sf::RenderWindow &window);
        private:
            std::vector<slug::Entity> mEntities;
    };

} // slug