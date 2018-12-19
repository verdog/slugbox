/* slug.cpp
 * he slither and slime
*/

#include <iostream>
#include <cmath>

#include "slug.hpp"
#include "vectorutils.hpp"

namespace slug {

    Slug::Slug() {
        mLifeTime = sf::Time::Zero;
    }

    Slug::~Slug() {
        std::cout << "Slug::~Slug()\n";
    }

    void Slug::update(const sf::Time &dTime) {
        mSlugShape.generateShape();
        mSlugShape.outerShape.rotate(-60 * dTime.asSeconds());

        mLifeTime += dTime;
    }

    void Slug::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(mSlugShape, states);
    }

    SlugShape::SlugShape() {
        radius = slug::math::randi(48, 128);
        innerRatio = slug::math::randf(0.2, 0.95);
        wiggleFactor = slug::math::randf(0, 8);
        hairLength = slug::math::randf(0, radius/4);
        hairWidthAngle = M_PI/slug::math::randi(32, 64);
        points = slug::math::randi(3, 32);

        outerShape.setPointCount(points*3);
        innerShape.setPointCount(points*3);
        outerShape.setOutlineColor(sf::Color::Black);
        innerShape.setOutlineColor(sf::Color::Black);
        outerShape.setOutlineThickness(2);
        innerShape.setOutlineThickness(2);
        outerShape.setFillColor(sf::Color::Transparent);
        innerShape.setFillColor(sf::Color::Black);
        outerShape.setPosition(460, 360);
    }

    SlugShape::~SlugShape() {
        //        
    }

    void SlugShape::generateShape() {
        float angle = 0;
        float angleDelta = 2*M_PI/points;
        
        for (int i = 0; i < points*3; i+=3, angle += angleDelta) {
            sf::Vector2f anchorA    = sf::Vector2f((radius+hairLength) * std::cos(angle), (radius+hairLength) * std::sin(angle));
            sf::Vector2f anchorB    = sf::Vector2f(radius * std::cos(angle+hairWidthAngle), radius * std::sin(angle+hairWidthAngle));
            sf::Vector2f anchorC    = sf::Vector2f(radius * std::cos(angle+angleDelta-hairWidthAngle), radius * std::sin(angle+angleDelta-hairWidthAngle));
            sf::Vector2f directionA = slug::math::normalize(anchorA);
            sf::Vector2f directionB = slug::math::normalize(anchorB);
            sf::Vector2f directionC = slug::math::normalize(anchorC);

            // wiggle
            anchorA += directionA * wiggleFactor * slug::math::randf(-1.0, 1.0);
            anchorB += directionB * wiggleFactor/2.0f * slug::math::randf(-1.0, 1.0);
            anchorC += directionC * wiggleFactor/2.0f * slug::math::randf(-1.0, 1.0);

            // set point on polygon
            outerShape.setPoint(i  , anchorA);
            outerShape.setPoint(i+1, anchorB);
            outerShape.setPoint(i+2, anchorC);

            innerShape.setPoint(i  , anchorA * innerRatio);
            innerShape.setPoint(i+1, anchorB * innerRatio);
            innerShape.setPoint(i+2, anchorC * innerRatio);
        }
    }

    void SlugShape::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        // draw the outer shell
        target.draw(outerShape, states);

        // take transform of outer shape
        states.transform *= outerShape.getTransform();
        states.transform *= innerShape.getInverseTransform();
        target.draw(innerShape, states);
    }

} // slug