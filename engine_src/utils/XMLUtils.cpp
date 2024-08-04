#include "utils/XMLUtils.h"
#include "interface/Debug.h"
#include "Resources.h"

bool ParseVec3FromXMLNode(const pugi::xml_node& node, const char* vecName, glm::vec3& vec, const bool& mandatory)
{
    const char* vecData = node.attribute(vecName).value();

    if (mandatory && (!vecData[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing " + std::string(vecName) + " mandatory attribute!");
        return false;
    }

    if (vecData[0] && (sscanf_s(vecData, "%f %f %f", &vec.x, &vec.y, &vec.z) != 3))
    {
        Debug::Log(Debug::Error, "Error while parsing " + std::string(vecName) + " data '" + std::string(vecData) + "'!");
        return false;
    }

    return true;
}

bool ParseVec4FromXMLNode(const pugi::xml_node& node, const char* vecName, glm::vec4& vec, const bool& mandatory)
{
    const char* vecData = node.attribute(vecName).value();

    if (mandatory && (!vecData[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing " + std::string(vecName) + " mandatory attribute!");
        return false;
    }

    if (vecData[0] && (sscanf_s(vecData, "%f %f %f %f", &vec.x, &vec.y, &vec.z, &vec.w) != 4))
    {
        Debug::Log(Debug::Error, "Error while parsing " + std::string(vecName) + " data '" + std::string(vecData) + "'!");
        return false;
    }

    return true;
}

bool ParseUintFromXMLNode(const pugi::xml_node& node, const char* uintName, unsigned& val, const bool& mandatory)
{
    const char* uintData = node.attribute(uintName).value();

    if (mandatory && (!uintData[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing " + std::string(uintName) + " mandatory attribute!");
        return false;
    }

    if (uintData[0] && (sscanf_s(uintData, "%u", &val) != 1))
    {
        Debug::Log(Debug::Error, "Error while parsing " + std::string(uintName) + " data '" + std::string(uintData) + "'!");
        return false;
    }

    return true;
}

bool ParseFloatFromXMLNode(const pugi::xml_node& node, const char* floatName, float& val, const bool& mandatory)
{
    const char* floatData = node.attribute(floatName).value();

    if (mandatory && (!floatData[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing " + std::string(floatName) + " mandatory attribute!");
        return false;
    }

    if (floatData[0] && (sscanf_s(floatData, "%f", &val) != 1))
    {
        Debug::Log(Debug::Error, "Error while parsing " + std::string(floatName) + " data '" + std::string(floatData) + "'!");
        return false;
    }

    return true;
}

bool ParseBoolFromXMLNode(const pugi::xml_node& node, const char* boolName, bool& val, const bool& mandatory)
{
    const char* boolData = node.attribute(boolName).value();

    if (mandatory && (!boolData[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing " + std::string(boolName) + " mandatory attribute!");
        return false;
    }

    if ((boolData[0] != '0' && boolData[0] != '1') || boolData[1] != 0)
    {
        Debug::Log(Debug::Error, "Error while parsing " + std::string(boolName) + " data '" + std::string(boolData) + "'!");
        return false;
    }
    val = (boolData[0] == '1');

    return true;
}

bool ParseMeshFromXMLNode(const pugi::xml_node& node, Mesh** mesh, const bool& mandatory)
{
    const std::string& meshName = node.attribute("mesh").value();

    if (mandatory && (!meshName[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing mesh mandatory attribute!");
        return false;
    }

    *mesh = Resources::Singleton()->GetMesh(meshName);
    if (!*mesh)
    {
        Debug::Log(Debug::Error, "Invalid mesh '" + meshName + "'!");
        return false;
    }

    return true;
}

bool ParseMaterialFromXMLNode(const pugi::xml_node& node, Material** material, const bool& mandatory)
{
    const std::string& materialName = node.attribute("material").value();

    if (mandatory && (!materialName[0]))
    {
        Debug::Log(Debug::DebugLevel::Error, "Missing material mandatory attribute!");
        return false;
    }

    *material = Resources::Singleton()->GetMaterial(materialName);
    if (!*material)
    {
        Debug::Log(Debug::Error, "Invalid material '" + materialName + "'!");
        return false;
    }

    return true;
}

bool ParseTextureFromXMLNode(const pugi::xml_node& node, Texture** texture)
{
    const std::string& textureName = node.attribute("sprite").value();

    *texture = Resources::Singleton()->GetTexture(textureName);
    if (!*texture)
    {
        Debug::Log(Debug::Error, "Invalid texture '" + textureName + "'!");
        return false;
    }

    return true;
}

bool ParseFontFromXMLNode(const pugi::xml_node& node, Font** font)
{
    const std::string& fontName = node.attribute("font").value();

    *font = Resources::Singleton()->GetFont(fontName);
    if (!*font)
    {
        Debug::Log(Debug::Error, "Invalid font '" + fontName + "'!");
        return false;
    }

    return true;
}

bool ParseStringFromXMLNode(const pugi::xml_node& node, const char* stringName, std::string val, const bool& mandatory)
{
    return false;
}
