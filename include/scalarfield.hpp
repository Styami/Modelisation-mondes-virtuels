#ifndef __SCALARFIELD__
#define __SCALARFIELD__
#include "image.hpp"
#include <string>

class ScalarField {
    public:
        ScalarField(const std::string& filename);
        void save(const std::string& filename);
    private:
        Image image;
};

#endif