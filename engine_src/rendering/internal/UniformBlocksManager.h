#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <vendor/glm/glm.hpp>
class UniformBlocksManager
{
public:
    enum class UniformBlocksTypes
    {
        CameraMatrices = 0,
        LightData,
        UniformBlockTypesNum
    };
    enum class CameraMatrices
    {
        Projection = 0,
        View,
        UIProjection,
        CameraMatricesNum
    };
    enum class LightData
    {
        Position = 0,
        Color,
        LightDataNum
    };

private:
    struct UniformBolckData
    {
        std::string name;
        unsigned bindIndex;
        unsigned uboID;
        unsigned size;
        std::vector<unsigned> elementsOffsets;
        std::vector<unsigned> elementsSizes;

    };
private:
    static std::vector<std::string> s_UniformBlockNames;
    static std::vector<UniformBolckData> s_UniformBlocks;
public:
    static void CreateUniformBlocksData();
    static void BindProgramUniformBlock(unsigned programID, unsigned index, const std::string& uniformBlockName);
    static void BindBuffersRange();

    static void SetUniformBlockData(unsigned blockType, unsigned blockElement, const void* data);
};

