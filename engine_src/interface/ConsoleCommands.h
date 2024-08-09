#pragma once

#include <string>
#include <vector>

typedef void (*CommandFunc)(const std::string& params);

class ConsoleCommands
{
public:
    struct ConsoleCommand
    {
        const std::string   m_Command;
        const std::string   m_CommandUsage;
        CommandFunc         m_Func;
    };

private:

    static std::vector<ConsoleCommand>      s_ConsoleCommands;

public:

    static void ExecuteCommand(const std::string& command);
    static std::vector<ConsoleCommand> GetCommandsWithPrefix(const std::string& prefix);

    static void PrintScene(const std::string& params);
    static void PrintNumber(const std::string& params);

    static void UnknownCommandAction(void);
};
