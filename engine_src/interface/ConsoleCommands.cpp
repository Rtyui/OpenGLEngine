#include "interface/ConsoleCommands.h"
#include "interface/Debug.h"

std::vector<ConsoleCommands::ConsoleCommand> s_consoleCommands = {
    { "caca", ConsoleCommands::PrintCaca}
};

void ConsoleCommands::PrintCaca(void)
{
    Debug::Log(Debug::DebugLevel::Error, "Caca");
}