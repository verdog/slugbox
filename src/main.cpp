/* slugbox
 * by spacedog
*/

#include <iostream>
#include <string>

#include "simulation.hpp"
#include "tests.hpp"

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == "-t") {
            std::cout << "Running tests: \n" << std::endl;
            slug::tests::test_all();
        }
    }

    slug::Simulation sim;
    sim.start();

    return 0;
}