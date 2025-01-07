#include <gtest/gtest.h>

#include <filesystem>

#include "FileLoader.h"

class FileLoaderTest : public ::testing::Test {
protected:
    std::filesystem::path getAssetPath(const std::string& filename) {
        return std::filesystem::current_path() / "assets" / filename;
    }
};

TEST_F(FileLoaderTest, LoadMTLFile) {
    std::string testPath  = getAssetPath("models/Goku/Goku.mtl").string();
    auto        materials = loadMTLFile(testPath);
    EXPECT_FALSE(materials.empty());
}

TEST_F(FileLoaderTest, LoadOBJFile) {
    std::string testPath = getAssetPath("models/Goku/Goku.obj").string();
    auto        objData  = loadFromOBJ(testPath);
    EXPECT_FALSE(objData.vertices.empty());
    EXPECT_FALSE(objData.materials.empty());
}
