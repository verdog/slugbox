/* slug.hpp
 * he slither and slime
*/

#pragma once

#include "entity.hpp"

namespace slug {

    class Slug : public Entity {
        public:
            Slug();
            ~Slug();

            void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            sf::CircleShape mShape;
    };

} // slug