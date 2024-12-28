#include "scalarfield.hpp"

ScalarField::ScalarField(const std::string& filename) :
    image(filename)
{}

void ScalarField::save(const std::string& filename) {
    image.save(filename);
}

