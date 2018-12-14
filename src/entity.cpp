/* entity.cpp
 * it's what things are
*/

#include "entity.hpp"

namespace slug {

    Entity::Entity() {
        //
    }

    Entity::~Entity() {
        //
    }
    
    void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        // by default, an entity doesn't draw anything
    }

} // slug
