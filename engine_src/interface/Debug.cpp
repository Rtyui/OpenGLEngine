#include "interface/Debug.h"
#include "interface/Console.h"

#include <iostream>

constexpr auto ESC = "\033[";
constexpr auto RESET = "\033[m";

unsigned Debug::s_DebugLevel = Debug::Verbose;

std::vector<unsigned> Debug::s_DebugLevelColorText = {
    31, // red
    33, // yellow
    32, // green
    37  // white
};

std::vector<glm::vec3> Debug::s_DebugLevelConsoleColorText =
{
    {1.f, 0.f, 0.f},
    {1.f, 1.f, 0.f},
    {0.f, 1.f, 0.f},
    {1.f, 1.f, 1.f}
};

void Debug::Log(DebugLevel debugLevel, const std::string& message)
{
    if ((unsigned) debugLevel <= s_DebugLevel)
    {
        std::cout << ESC << s_DebugLevelColorText[debugLevel] << "m" << message << RESET << std::endl;
        Debug::LogConsole(debugLevel, message);
        /*Console* console = Console::s_Console;
        if (console)
        {
            console->PushLine(message, s_DebugLevelConsoleColorText[debugLevel]);
        }*/
    }
}

void Debug::LogConsole(DebugLevel debugLevel, const std::string& message)
{
    if ((unsigned)debugLevel <= s_DebugLevel)
    {
        Console* console = Console::s_Console;
        if (console)
        {
            console->PushLine(message, s_DebugLevelConsoleColorText[debugLevel]);
        }
    }
}

void Debug::RegisterGLErrorCallback()
{
    glDebugMessageCallback(HandleGLError, 0);
}

void GLAPIENTRY HandleGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
    const void* userParam)
{
    /*fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);*/
    Debug::Log(Debug::Error, "GL Error: " + std::string(message));
}
