/* entity.hpp
 * it's what things are
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace slug {

    class Entity : public sf::Drawable {
        public:
            Entity();
            ~Entity();
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        private:
    };

} // slug
