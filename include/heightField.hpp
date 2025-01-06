#ifndef __HEIGHTFIELD__
#define __HEIGHTFIELD__
#include "scalarfield.hpp"
#include <algorithm>

class HeightField : public ScalarField {
    public:
        HeightField(const std::string& filename, const glm::vec3& min, const glm::vec3& max);
        HeightField(const Image& image, const glm::vec3& min, const glm::vec3& max);
        HeightField(HeightField&& heightField);
        float height(const float x, const float y) const;
        float averageSlope(const float x, const float y) const;
        Image slope() const;
        float heightGrid(const int x, const int y) const;
    private:
        int sizeX, sizeY;
        std::vector<float> heights;
        glm::vec3 minMap, maxMap;

};



#endif