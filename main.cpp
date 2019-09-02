#include "app_epub/EpubFactory.hpp"

using app::epub::Factory;
using app::epub::FactoryType;
using app::epub::Commands;
using app::epub::CommandKey;
using app::epub::CommandValue;

using app::utils::Log;
using app::utils::LogType;

int main(Jint argLen, Jchar *arg[])
{
    auto commands = reinterpret_cast<Commands &>(Factory::Instance().Get<FactoryType::COMMANDS>());
    commands.SetCommands(arg, argLen);
    commands.Run();
    return 0;
}