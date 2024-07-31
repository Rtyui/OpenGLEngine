#pragma once

#include <string>
#include <vector>

typedef void (*CommandFunc)(void);

class ConsoleCommands
{
public:
    struct ConsoleCommand
    {
        const std::string   m_commandName;
        CommandFunc         m_func;
    };

private:

    static std::vector<ConsoleCommand>      s_consoleCommands;

public:

    static void ExecuteCommand(const std::string& command);

    static void PrintScene(void);

    static void UnknownCommandAction(void);
};
