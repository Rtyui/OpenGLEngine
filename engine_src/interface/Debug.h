#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <vendor/glm/glm.hpp>

void GLAPIENTRY HandleGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
    const void* userParam);
class Debug
{
public:
    enum DebugLevel
    {
        Error = 0,
        Warning,
        Info,
        Verbose
    };
    static std::vector<unsigned> s_DebugLevelColorText;
    static std::vector<glm::vec3> s_DebugLevelConsoleColorText;
    static unsigned s_DebugLevel;
public:
    static void Log(DebugLevel debugLevel, const std::string& message);
    static void LogConsole(DebugLevel debugLevel, const std::string& message);
    static void RegisterGLErrorCallback();
};

