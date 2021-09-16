#include <Data.Game.Avatar.h>
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.StateTransition.h"
#include <map>
namespace game::avatar
{
	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>>& GetActionDescriptors();
}
namespace game
{
	static void SetState(const game::avatar::State& state)
	{
		data::game::Avatar::WriteState((int)state);
	}


	bool Avatar::DoAction(const avatar::Action& action)
	{
		auto state = GetState();
		if (state)
		{
			auto descriptor = avatar::GetActionDescriptors().find(action);
			if (descriptor != avatar::GetActionDescriptors().end())
			{
				auto transition = descriptor->second.find(state.value());
				{
					if (transition != descriptor->second.end())
					{
						auto result = transition->second();
						avatar::Log::Write({ result.logColor, result.logText });
						SetState(result.dockedState);
					}
				}
			}
		}
		return false;
	}

	std::optional<game::avatar::State> Avatar::GetState()
	{
		auto state = data::game::Avatar::ReadState();
		if (state)
		{
			return (game::avatar::State)state.value();
		}
		return std::nullopt;
	}
}