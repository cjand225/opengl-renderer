#include <gtest/gtest.h>

#include <filesystem>

#include "FileLoader.h"
#include "Geometry.h"
#include "Material.h"

class AssetLoaderTest : public ::testing::Test {
protected:
    std::filesystem::path getAssetPath(const std::string& filename) {
        auto path = std::filesystem::current_path() / "assets" / filename;
        return path;
    }
};

TEST_F(AssetLoaderTest, LoadMTLFile) {
    std::string testPath = getAssetPath("models/Goku/Goku.mtl").string();
    ASSERT_TRUE(std::filesystem::exists(testPath)) << "MTL file not found at: " << testPath;

    auto materials = loadMTLFile(testPath);
    EXPECT_FALSE(materials.empty()) << "No materials loaded from MTL file";

    EXPECT_TRUE(materials.find("face.png") != materials.end()) << "Expected material 'face.png' not found";
}

TEST_F(AssetLoaderTest, LoadOBJFile) {
    std::string testPath = getAssetPath("models/Goku/Goku.obj").string();
    ASSERT_TRUE(std::filesystem::exists(testPath)) << "OBJ file not found at: " << testPath;

    OBJData objData = loadFromOBJ(testPath);

    EXPECT_FALSE(objData.vertices.empty()) << "No vertices loaded";
    EXPECT_FALSE(objData.UVs.empty()) << "No UVs loaded";
    EXPECT_FALSE(objData.materials.empty()) << "No materials loaded";
    EXPECT_FALSE(objData.materialGroups.empty()) << "No material groups loaded";
}

TEST_F(AssetLoaderTest, BasicGeometryValidation) {
    std::string testPath = getAssetPath("models/Goku/Goku.obj").string();
    auto        objData  = loadFromOBJ(testPath);

    // Validate face data matches vertex data
    for (const auto& [material, faces] : objData.materialGroups) {
        for (const auto& face : faces) {
            // Check vertex indices are valid
            for (int idx : face.vertexIndices) {
                EXPECT_GE(idx, 0) << "Negative vertex index found";
                EXPECT_LT(idx, objData.vertices.size()) << "Vertex index out of bounds";
            }

            // Check UV indices are valid
            for (int idx : face.uvIndices) {
                EXPECT_GE(idx, 0) << "Negative UV index found";
                EXPECT_LT(idx, objData.UVs.size()) << "UV index out of bounds";
            }
        }
    }
}
