#ifndef __HEIGHTFIELD__
#define __HEIGHTFIELD__
#include "scalarfield.hpp"
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <vector>


class HeightField : public ScalarField {
    public:
        HeightField(const std::string& filename, const glm::vec3& min, const glm::vec3& max);
        HeightField(const Image& image, const glm::vec3& min, const glm::vec3& max);
        HeightField(HeightField&& heightField);
        void toObj(const std::string& filename) const;
        float height(const float x, const float y) const;
        float averageSlope(const int x, const int y) const;
        Image slope() const;
        float heightGrid(const int x, const int y) const;
    private:
        struct Triangle {
            int a, b, c;
            mutable int indexNorm;
            inline glm::vec3 normal(const std::vector<glm::vec3>& verticies, int index) const {
                glm::vec3 ab = verticies[b] - verticies[a];
                glm::vec3 ac = verticies[c] - verticies[a];
                indexNorm = index;
                return glm::normalize(glm::cross(ab, ac));
            }
        };
        
        std::vector<glm::vec3> makeVerticies() const;
        std::vector<glm::vec3> makeNormales(const std::vector<Triangle>&  triangles, const std::vector<glm::vec3>& verticies) const;
        std::vector<Triangle> makeTriangles(const std::vector<glm::vec3>& verticies) const;    
        int sizeX, sizeY;
        std::vector<float> heights;
        glm::vec3 minMap, maxMap;


};



#endif