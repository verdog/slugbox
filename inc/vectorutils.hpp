/* vectorutils
 * helper functions for vector math
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace slug {
    namespace math {
        sf::Vector2f normalize(sf::Vector2f v);
        float magnitude(sf::Vector2f v);
        float randf(float low, float high);
    }
}