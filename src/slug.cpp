/* slug.cpp
 * he slither and slime
*/

#include <iostream>
#include <cmath>

#include "slug.hpp"
#include "vectorutils.hpp"

namespace slug {

    Slug::Slug() {
        
    }

    Slug::~Slug() {
        std::cout << "Slug::~Slug()\n";
    }

    void Slug::update(const sf::Time &dTime) {
        mSlugShape.generateShape();
        mSlugShape.convexShape.rotate(60 * dTime.asSeconds());
    }

    void Slug::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(mSlugShape);
    }

    SlugShape::SlugShape() {
        radius = 64;
        wiggleFactor = 10;
        hairLength = 24;
        hairWidthAngle = M_PI/64;
        points = 17;

        convexShape.setPointCount(points*3);
        convexShape.setOutlineColor(sf::Color::Black);
        convexShape.setOutlineThickness(2);
        convexShape.setFillColor(sf::Color::Transparent);
        convexShape.setPosition(256,256);
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
            convexShape.setPoint(i, anchorA);
            convexShape.setPoint(i+1, anchorB);
            convexShape.setPoint(i+2, anchorC);
        }
    }

    void SlugShape::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(convexShape);
    }

} // slug