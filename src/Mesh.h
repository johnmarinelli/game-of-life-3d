#ifndef Mesh_h
#define Mesh_h

#include <map>
#include <vector>
#include <numeric>

namespace john {
  namespace mesh {
    
    struct triangle {
      glm::vec3 v0, v1, v2;
      triangle(const glm::vec3& v_0, const glm::vec3& v_1, const glm::vec3& v_2) : v0(v_0), v1(v_1), v2(v_2) {}
    };
    
    namespace cube {
      // TODO
      // if there's a way to calculate these at compile time that'd be nice
      static GLfloat rawNormals[24];
      static std::vector<glm::vec3> triangleNormals;
      static std::vector<glm::vec3> normals;
      
      static const GLushort indexOrder[] =
      {
        0, 1, 2, // 0
        2, 1, 3, // 1
        2, 3, 4, // 2
        4, 3, 5, // 3
        4, 5, 6, // 4
        6, 5, 7, // 5
        6, 7, 0, // 6
        0, 7, 1, // 7
        6, 0, 2, // 8
        2, 4, 6, // 9
        7, 5, 3, // 10
        7, 3, 1  // 11
      };
      
      static const GLfloat vertices[] =
      {
        -1.0f, -1.0f, -1.0f, // 0
        -1.0f,  1.0f, -1.0f, // 1
        1.0f, -1.0f, -1.0f,  // 2
        1.0f,  1.0f, -1.0f,  // 3
        1.0f, -1.0f,  1.0f,  // 4
        1.0f,  1.0f,  1.0f,  // 5
        -1.0f, -1.0f,  1.0f, // 6
        -1.0f,  1.0f,  1.0f, // 7
      };
      
      // static storage for shared triangles, indexed by vertex
      static const std::map<int, std::vector<int>> sharedTriangles =
      {
        std::pair<int, std::vector<int>>{0, std::vector<int>{0,6,7,8}},
        std::pair<int, std::vector<int>>{1, std::vector<int>{0,1,7,11}},
        std::pair<int, std::vector<int>>{2, std::vector<int>{0,1,2,8,9}},
        std::pair<int, std::vector<int>>{3, std::vector<int>{1,2,3,10,11}},
        std::pair<int, std::vector<int>>{4, std::vector<int>{2,3,4,9}},
        std::pair<int, std::vector<int>>{5, std::vector<int>{3,4,5,10}},
        std::pair<int, std::vector<int>>{6, std::vector<int>{4,5,6,8,9}},
        std::pair<int, std::vector<int>>{7, std::vector<int>{5,6,7,10,11}}
      };
      
      
      glm::vec3 calculateTriangleNormal(const john::mesh::triangle& t) {
        auto e0 = t.v1 - t.v0;
        auto e1 = t.v2 - t.v0;
        auto e0xe1 = glm::cross(e1, e0);
        
        glm::vec3 n = glm::normalize(e0xe1);
        
        if (e0xe1[0] == 0 && e0xe1[1] == 0 && e0xe1[2] == 0) {
          // can't mathematically normalize a 0 vector
          // auto minFloat = std::numeric_limits<float>::min(); // this gives +Inf when normalized
          auto minFloat = 0.0001f;
          n = glm::normalize(glm::vec3{minFloat, minFloat, minFloat});
        }
        
        return n;
      }
      
      void calculateCubeNormals() {
        int indexScale = 3;
        using namespace john;
        
        for (auto i = 0; i < 12; ++i) {
          auto vi = i * indexScale;
          
          
          // get indices to reference cube_vertices
          auto i0 = mesh::cube::indexOrder[vi];
          auto i1 = mesh::cube::indexOrder[vi+1];
          auto i2 = mesh::cube::indexOrder[vi+2];
          
          // scale cube_vertices indices
          i0 *= indexScale;
          i1 *= indexScale;
          i2 *= indexScale;
          
          glm::vec3 p0{mesh::cube::vertices[i0], mesh::cube::vertices[i0 + 1], mesh::cube::vertices[i0 + 2]};
          glm::vec3 p1{mesh::cube::vertices[i1], mesh::cube::vertices[i1 + 1], mesh::cube::vertices[i0 + 2]};
          glm::vec3 p2{mesh::cube::vertices[i2], mesh::cube::vertices[i2 + 1], mesh::cube::vertices[i2 + 2]};
          
          auto t = john::mesh::triangle(p0, p1, p2);
          auto tn = calculateTriangleNormal(t);
          mesh::cube::triangleNormals.push_back(tn);
        }
        
        // make a copy so we can capture in std::accumulate's lambda below
        auto tns = mesh::cube::triangleNormals;
        
        for (const auto& kv : mesh::cube::sharedTriangles) {
          // triangle indices for vector i
          auto tis = kv.second;
          
          // pick up indices from tn
          auto vn = std::accumulate(tis.begin(), tis.end(), glm::vec3{0.f}, [tns] (glm::vec3 n, int idx) {
            auto tn = tns[idx];
            return n + tn;
          });
          
          auto vnn = glm::normalize(vn);
          mesh::cube::normals.push_back(vnn);
        }
        
        
        for (auto i = 0; i < mesh::cube::normals.size(); ++i) {
          auto v = mesh::cube::normals[i];
          auto x = v.x;
          auto y = v.y;
          auto z = v.z;
          
          auto ii = i * indexScale;
          
          mesh::cube::rawNormals[ii] = x;
          mesh::cube::rawNormals[ii+1] = y;
          mesh::cube::rawNormals[ii+2] = z;
        }
      }
      
    }
    // Cube
  }
}

#endif 
