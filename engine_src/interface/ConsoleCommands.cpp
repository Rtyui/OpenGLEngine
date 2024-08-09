#include "interface/ConsoleCommands.h"
#include "interface/Debug.h"
#include "entities/Scene.h"

#include <sstream>

std::vector<ConsoleCommands::ConsoleCommand> ConsoleCommands::s_ConsoleCommands = {
    { "print current scene", "print current scene", ConsoleCommands::PrintScene},
    { "print number", "print number <number>", ConsoleCommands::PrintNumber}
};

void ConsoleCommands::ExecuteCommand(const std::string& command)
{
    for (ConsoleCommand consoleCommand : ConsoleCommands::s_ConsoleCommands)
    {
        if (!command.compare(0, consoleCommand.m_Command.length(), consoleCommand.m_Command))
        {
            consoleCommand.m_Func(command.substr(consoleCommand.m_Command.length(), std::string::npos));
            return;
        }
    }

    UnknownCommandAction();
}

std::vector<ConsoleCommands::ConsoleCommand> ConsoleCommands::GetCommandsWithPrefix(const std::string& prefix)
{
    std::vector<ConsoleCommand> commandsUsages;

    for (ConsoleCommand consoleCommand : ConsoleCommands::s_ConsoleCommands)
    {
        if (!consoleCommand.m_Command.compare(0, prefix.length(), prefix))
        {
            commandsUsages.push_back(consoleCommand);
        }
    }
    return commandsUsages;
}

void ConsoleCommands::PrintScene(const std::string& params)
{
    Debug::Log(Debug::DebugLevel::Info, Scene::GetCurrentScene()->GetName());
}

void ConsoleCommands::PrintNumber(const std::string& params)
{
    std::istringstream stream(params);
    int number;
    stream >> number;
    Debug::Log(Debug::DebugLevel::Info, std::to_string(number));
}

void ConsoleCommands::UnknownCommandAction(void)
{
    Debug::Log(Debug::DebugLevel::Warning, "Unknown command");
}
