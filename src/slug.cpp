/* slug.cpp
 * he slither and slime
*/

#include <iostream>

#include "slug.hpp"

namespace slug {

    Slug::Slug() {
        mShape.setRadius(16);
        mShape.setPointCount(7);
        mShape.setOutlineColor(sf::Color::Black);
        mShape.setOutlineThickness(2);
        mShape.setPosition(0, 0);
    }

    Slug::~Slug() {
        std::cout << "Slug::~Slug()\n";
    }

    void Slug::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(mShape);
    }

} // slug