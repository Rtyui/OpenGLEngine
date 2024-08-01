#include "interface/ConsoleCommands.h"
#include "interface/Debug.h"
#include "entities/Scene.h"

std::vector<ConsoleCommands::ConsoleCommand> ConsoleCommands::s_ConsoleCommands = {
    { "print scene", ConsoleCommands::PrintScene }
};

void ConsoleCommands::ExecuteCommand(const std::string& command)
{
    for (ConsoleCommand consoleCommand : ConsoleCommands::s_ConsoleCommands)
    {
        if (!command.compare(0, consoleCommand.m_CommandName.length(), consoleCommand.m_CommandName))
        {
            consoleCommand.m_Func();
            return;
        }
    }

    UnknownCommandAction();
}

void ConsoleCommands::PrintScene(void)
{
    Debug::Log(Debug::DebugLevel::Info, Scene::GetCurrentScene()->GetName());
}

void ConsoleCommands::UnknownCommandAction(void)
{
    Debug::Log(Debug::DebugLevel::Warning, "Unknown command");
}
