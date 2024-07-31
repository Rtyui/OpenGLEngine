#include "rendering/internal/UniformBlocksManager.h"
#include "interface/Debug.h"

#include <GL/glew.h>
#include <vendor/glm/glm.hpp>

std::vector<std::string> UniformBlocksManager::s_UniformBlockNames =
{
    "ub_CameraMatrices"
};

std::vector<UniformBlocksManager::UniformBolckData> UniformBlocksManager::s_UniformBlocks =
{
    {"ub_CameraMatrices", (unsigned) UniformBlocksManager::UniformBlocksTypes::CameraMatrices, 0,
    sizeof(glm::mat4) * (unsigned)UniformBlocksManager::CameraMatrices::CameraMatricesNum,
    {0, sizeof(glm::mat4), 2 * sizeof(glm::mat4)}, {sizeof(glm::mat4), sizeof(glm::mat4), sizeof(glm::mat4)}},

    {"ub_LightData", (unsigned) UniformBlocksManager::UniformBlocksTypes::LightData, 0,
    sizeof(glm::vec4) * (unsigned)UniformBlocksManager::LightData::LightDataNum,
    {0, sizeof(glm::vec4)}, {sizeof(glm::vec4), sizeof(glm::vec4)}}
};


void UniformBlocksManager::CreateUniformBlocksData()
{
    for (UniformBolckData& uniformBlockData : s_UniformBlocks)
    {
        glGenBuffers(1, &(uniformBlockData.uboID));
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockData.uboID);
        glBufferData(GL_UNIFORM_BUFFER, uniformBlockData.size, NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        Debug::Log(Debug::Verbose, "Created ubo " + std::to_string(uniformBlockData.uboID) + " for " + uniformBlockData.name);
    }
}

void UniformBlocksManager::BindProgramUniformBlock(unsigned programID, unsigned index, const std::string& uniformBlockName)
{
    for (UniformBolckData& uniformBlockData : s_UniformBlocks)
    {
        if (uniformBlockData.name == uniformBlockName)
        {
            glUniformBlockBinding(programID, index, uniformBlockData.bindIndex);
            Debug::Log(Debug::Verbose, "Bound block " + uniformBlockName + " for program id " + std::to_string(programID));
            return;
        }
    }
    Debug::Log(Debug::Warning, "Could not find uniform block " + uniformBlockName);
}

void UniformBlocksManager::BindBuffersRange()
{
    for (UniformBolckData& uniformBlockData : s_UniformBlocks)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, uniformBlockData.bindIndex, uniformBlockData.uboID, 0, uniformBlockData.size);
    }
}

void UniformBlocksManager::SetUniformBlockData(unsigned blockType, unsigned blockElement, const void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, s_UniformBlocks[blockType].uboID);
    glBufferSubData(GL_UNIFORM_BUFFER, s_UniformBlocks[blockType].elementsOffsets[blockElement],
        s_UniformBlocks[blockType].elementsSizes[blockElement], data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
