/* box.hpp
 * the box that contains the slugs
*/

#pragma once

#include <vector>
#include <memory>

#include "entity.hpp"

namespace slug {

    class Box {
        public:
            Box();
            ~Box();

            void spawn();
            void update(const sf::Time &dTime);
            void drawContents(sf::RenderWindow &window);
        private:
            std::vector<std::unique_ptr<slug::Entity>> mEntities;
    };

} // slug