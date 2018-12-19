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
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            virtual void handleInput(const sf::Time &dTime);
            virtual void update(const sf::Time &dTime);
        private:
    };

} // slug
