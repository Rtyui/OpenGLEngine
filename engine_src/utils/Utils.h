#pragma once
#include <string>
#include <vendor/pugixml/pugixml.hpp>
#include <vendor/glm/glm.hpp>
class Utils
{
public:
    static unsigned CharCountInString(const char c, const std::string& s);

    static std::string Mat4ToString(const glm::mat4& mat4);


};

