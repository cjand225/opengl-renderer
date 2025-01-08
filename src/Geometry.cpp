#include <unordered_map>

#include "Geometry.h"
std::vector<Vertex> flattenVertices(const std::vector<Face>& faces, const std::vector<Vertex>& vertices) {
    std::vector<Vertex> flatVertices;
    for (const auto& face : faces) {
        for (int index : face.vertexIndices) {
            flatVertices.push_back(vertices[index]);
        }
    }
    return flatVertices;
}

std::vector<UV> flattenUVs(const std::vector<Face>& faces, const std::vector<UV>& uvs) {
    std::vector<UV> flatUVs;

    for (const auto& face : faces) {
        for (int uvIndex : face.uvIndices) {
            if (uvIndex >= 0 && uvIndex < uvs.size()) {
                flatUVs.push_back(uvs[uvIndex]);
            } else {
                flatUVs.push_back({0.0f, 0.0f, 0.0f});  // Default UV
            }
        }
    }

    return flatUVs;
}

std::vector<unsigned int> flattenIndices(const std::vector<Face>& faces) {
    std::vector<unsigned int> flatIndices;
    unsigned int              currentIndex = 0;

    for (const auto& face : faces) {
        for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
            flatIndices.push_back(currentIndex++);
        }
    }

    return flatIndices;
}

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

void flattenGroupData(const std::vector<Face>&   faces,
                      const std::vector<Vertex>& vertices,
                      const std::vector<UV>&     uvs,
                      std::vector<Vertex>&       outVertices,
                      std::vector<UV>&           outUVs,
                      std::vector<unsigned int>& outIndices) {
    std::unordered_map<std::pair<int, int>, unsigned int, PairHash> vertexUVMap;
    unsigned int                                                    newIndex = 0;

    for (const auto& face : faces) {
        for (size_t i = 0; i < face.vertexIndices.size(); i++) {
            int vertexIndex = face.vertexIndices[i];
            int uvIndex     = face.uvIndices[i];

            std::pair<int, int> vertexUVPair(vertexIndex, uvIndex);

            if (vertexUVMap.find(vertexUVPair) == vertexUVMap.end()) {
                vertexUVMap[vertexUVPair] = newIndex++;
                outVertices.push_back(vertices[vertexIndex]);

                if (uvIndex >= 0 && uvIndex < uvs.size()) {
                    outUVs.push_back(uvs[uvIndex]);
                } else {
                    outUVs.push_back({0.0f, 0.0f, 0.0f});
                }
            }

            outIndices.push_back(vertexUVMap[vertexUVPair]);
        }
    }
}
