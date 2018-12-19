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

        void generateShape();

        unsigned int radius;
        float innerRatio;
        unsigned int points;
        float hairLength;
        float hairWidthAngle;
        float wiggleFactor;
        sf::ConvexShape outerShape;
        sf::ConvexShape innerShape;
    };

    class Slug : public Entity {
        public:
            Slug();
            ~Slug();

            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void update(const sf::Time &dTime);

        private:
            SlugShape mSlugShape;
            sf::Time mLifeTime;
    };

} // slug