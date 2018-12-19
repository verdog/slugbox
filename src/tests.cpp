/* tests
 * various debug testing
*/

#include <iostream>
#include <string>

#include "tests.hpp"
#include "vectorutils.hpp"

namespace slug {
    namespace tests {

        void test_all() {
            srand(std::time(nullptr));
            test_randi(-4, 4);
            test_randf(-4, 4);
        }

        void test_randf(float low, float high) {
            // tests slug::math::randf
            std::cout << "*** Testing slug::math::randf ***\n";
            std::cout << "    randf(" + std::to_string(low) + ", " + std::to_string(high) + "):\n";
            std::string res = "";

            for (int i = 0; i < 16; i++) {
                if (i > 0) {
                    res += ", ";
                }

                res += std::to_string(slug::math::randf(low, high));
            }

            std::cout << "    result: " + res << std::endl;
        }

        void test_randi(int low, int high) {
            // tests slug::math::randi
            std::cout << "*** Testing slug::math::randi ***\n";
            std::cout << "    randi(" + std::to_string(low) + ", " + std::to_string(high) + "):\n";
            std::string res = "";

            for (int i = 0; i < 16; i++) {
                if (i > 0) {
                    res += ", ";
                }

                res += std::to_string(slug::math::randi(low, high));
            }

            std::cout << "    result: " + res << std::endl;
        }

    }
}