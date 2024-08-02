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
