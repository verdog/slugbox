/* box.cpp
 * the box that contains the slugs
*/

#include <iostream>

#include "box.hpp"
#include "slug.hpp"

namespace slug {

    Box::Box() {
        spawn();
    }

    Box::~Box() {
        std::cout << "Box::~Box()\n";
    }

    void Box::spawn() {
        mEntities.push_back(std::unique_ptr<slug::Slug>(new Slug));
    }

    void Box::update(const sf::Time &dTime) {
        for (auto &e : mEntities) {
            e->update(dTime);
        }
    }

    void Box::drawContents(sf::RenderWindow &window) {
        for (auto &e : mEntities) {
            window.draw(*e);
        }
    }

} // slug