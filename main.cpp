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
    Commands commands = reinterpret_cast<Commands &>(Factory::Instance().Get<FactoryType::COMMANDS>());
    commands.SetCommands(arg, argLen);
    commands.SetResultsListener(nullptr, [](void *, CommandKey &k, CommandValue &v) {
        if (!k.Equals(v))
            Log::Print<LogType::DBUG>("key: %s, value: %s", k.GetArray(), v.GetArray());
        else
            Log::Print<LogType::DBUG>("fuckman");
    }).Run();
    return 0;
}