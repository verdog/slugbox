/* slug.hpp
 * he slither and slime
*/

#pragma once

#include "entity.hpp"

namespace slug {

    struct SlugShape : public sf::Drawable {
        SlugShape();
        ~SlugShape();
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        void generateShape(int points);

        unsigned int radius;
        float hairLength;
        float wiggleFactor;
        sf::ConvexShape convexShape;
        sf::VertexArray hairShape;
    };

    class Slug : public Entity {
        public:
            Slug();
            ~Slug();

            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void update(const sf::Time &dTime);

        private:
            SlugShape mSlugShape;
    };

} // slug