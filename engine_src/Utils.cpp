#include "Utils.h"
#include "interface/Debug.h"

unsigned Utils::CharCountInString(const char c, const std::string& s)
{
    unsigned occurences = 0;
    for (unsigned i = 0; i < s.size(); ++i)
    {
        if (s[i] == c)
        {
            ++occurences;
        }
    }
    return occurences;
}

std::string Utils::Mat4ToString(const glm::mat4& mat4)
{
    std::string ret;
    ret += "[ " + std::to_string(mat4[0][0]) + " " + std::to_string(mat4[0][1]) + " " + std::to_string(mat4[0][2]) + " " + std::to_string(mat4[0][3]) + "\n";
    ret += "  " + std::to_string(mat4[1][0]) + " " + std::to_string(mat4[1][1]) + " " + std::to_string(mat4[1][2]) + " " + std::to_string(mat4[1][3]) + "\n";
    ret += "  " + std::to_string(mat4[2][0]) + " " + std::to_string(mat4[2][1]) + " " + std::to_string(mat4[2][2]) + " " + std::to_string(mat4[2][3]) + "\n";
    ret += "  " + std::to_string(mat4[3][0]) + " " + std::to_string(mat4[3][1]) + " " + std::to_string(mat4[3][2]) + " " + std::to_string(mat4[3][3]) + " ]" + "\n";
    return ret;
}

bool Utils::ParseVec3FromXMLNode(const pugi::xml_node& node, const char* vecName, glm::vec3& vec, const bool& mandatory)
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

bool Utils::ParseBoolFromXMLNode(const pugi::xml_node& node, const char* boolName, bool& val, const bool& mandatory)
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
