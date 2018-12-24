/* slug.hpp
 * he slither and slime
*/

#pragma once

#include "entity.hpp"
#include "drawablennet.hpp"

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
            Slug(const DrawableNeuralNet &brain);
            ~Slug();

            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
            void update(const sf::Time &dTime);

            DrawableNeuralNet& getBrain();

            // transforming
            const sf::Transform& getTransform();
            void move(sf::Vector2f vec);
            void setPosition(sf::Vector2f vec);
            sf::Vector2f getPosition();
            void rotate(float angle);
            void setRotation(float angle);
            sf::FloatRect getGlobalBounds();
            sf::FloatRect getLocalBounds();
            float getRadius() const;

        private:
            SlugShape mSlugShape;
            sf::Time mLifeTime;

            void initBrain();
            DrawableNeuralNet mBrain;

            // physics
            sf::Vector2f mVelocity;
            float mRotVelocity;
    };

} // slug