/* entity.cpp
 * it's what things are
*/

#include <iostream>

#include "entity.hpp"

namespace slug {

    Entity::Entity() {
        //
    }

    Entity::~Entity() {
        std::cout << "Entity::~Entity()\n";
    }
    
    void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        // by default, an entity doesn't draw anything
    }

    void Entity::handleInput(const sf::Time &dTime) {
        // empty
    }

    void Entity::update(const sf::Time &dTime) {
        // empty
    }

} // slug
