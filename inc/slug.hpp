/* slug.hpp
 * he slither and slime
*/

#pragma once

#include "entity.hpp"
#include "nnet.hpp"

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

            // transforming
            void move(sf::Vector2f vec);
            void setPosition(sf::Vector2f vec);
            void rotate(float angle);
            void setRotation(float angle);

        private:
            SlugShape mSlugShape;
            sf::Time mLifeTime;

            void initBrain();
            NeuralNetwork mBrain;

            // physics
            sf::Vector2f mVelocity;
            float mRotVelocity;
    };

} // slug