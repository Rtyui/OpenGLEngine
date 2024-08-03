#pragma once
#include <string>
#include <vendor/pugixml/pugixml.hpp>
#include <vendor/glm/glm.hpp>
class Utils
{
public:
    static unsigned CharCountInString(const char c, const std::string& s);

    static std::string Mat4ToString(const glm::mat4& mat4);

    static bool ParseVec3FromXMLNode(const pugi::xml_node& node, const char* vecName, glm::vec3& vec, const bool& mandatory);

    static bool ParseBoolFromXMLNode(const pugi::xml_node& node, const char* boolName, bool& val, const bool& mandatory);
};

