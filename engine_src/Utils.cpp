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