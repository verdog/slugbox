/* vectorutils
 * helper functions for vector math
*/

#include <cmath>

#include <SFML/Graphics.hpp>

#include "vectorutils.hpp"

namespace slug {
    namespace math {

        float magnitude(sf::Vector2f v) {
            return std::sqrt(v.x*v.x + v.y*v.y);
        }

        sf::Vector2f normalize(sf::Vector2f v) {
            return v/magnitude(v);
        }

        int randi(int low, int high) {
            return std::rand() % (high-low+1) + low;
        }

        float randf(float low, float high) {
            // https://stackoverflow.com/questions/686353/random-float-number-generation
            return low + static_cast <float> (rand())/( static_cast <float> (RAND_MAX/(high-low)));
        }
    }
}