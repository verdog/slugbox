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
        mSlugShape.convexShape.rotate(120 * dTime.asSeconds());
    }

    void Slug::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(mSlugShape);
    }

    SlugShape::SlugShape() {
        radius = 16;
        wiggleFactor = 3;
        hairLength = 6.0;
        points = 5;

        convexShape.setOutlineColor(sf::Color::Black);
        convexShape.setOutlineThickness(2);
        convexShape.setFillColor(sf::Color::Transparent);
        convexShape.setPosition(256,256);
    }

    SlugShape::~SlugShape() {
        //        
    }

    void SlugShape::generateShape() {
        // generate basic polygon
        float angle = 0;
        float angleDelta = 2*M_PI/points;
        
        convexShape.setPointCount(points);
        hairShape.setPrimitiveType(sf::PrimitiveType::Lines);
        hairShape.clear();

        for (int i = 0; i < points; i++, angle += angleDelta) {
            sf::Vector2f newPoint = sf::Vector2f( radius * std::cos(angle), radius * std::sin(angle) );
            sf::Vector2f direction = slug::math::normalize(newPoint);

            // wiggle
            newPoint = newPoint + direction * slug::math::randf(-wiggleFactor, wiggleFactor);

            // set point on polygon
            convexShape.setPoint(i, newPoint);
            // set hair line
            sf::Vertex hairVertA = sf::Vertex(newPoint, sf::Color::Black);
            sf::Vertex hairVertB = sf::Vertex(newPoint + hairLength * slug::math::normalize(newPoint), sf::Color::Black);

            hairShape.append(hairVertA);
            hairShape.append(hairVertB);
        }
    }

    void SlugShape::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(convexShape);
        states.transform = convexShape.getTransform();
        target.draw(hairShape, states);
    }

} // slug