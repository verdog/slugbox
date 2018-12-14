/* box.cpp
 * the box that contains the slugs
*/

#include "box.hpp"

namespace slug {

    Box::Box() {
        spawn();
    }

    Box::~Box() {
        //
    }

    void Box::spawn() {
        //
    }

    void Box::update() {
        //
    }

    void Box::drawContents(sf::RenderWindow &window) {
        for (auto &e : mEntities) {
            window.draw(e);
        }
    }

} // slug