#pragma once
#include <optional>
#include <string>
#include <functional>
namespace application
{
	struct UIState
	{
		static void SetFinalState(int);
		template<typename TState>
		static std::function<void()> DoSetFinalState(const TState& state)
		{
			return [state]() 
			{
				SetFinalState((int)state);
			};
		}
		static int GetFinalState();

		static void Write(int);
		static int Read();
		static std::function<void()> GoTo(int);

		static void Push(int);
		static void Pop();
		static std::function<void()> PushTo(int);
		static bool CanPop();

		template<typename TState>
		static void Write(const TState& state)
		{
			Write((int)state);
		}
		template<typename TState>
		static std::function<void()> GoTo(const TState& state)
		{
			return GoTo((int)state);
		}
		template<typename TState>
		static void Push(const TState& state)
		{
			Push((int)state);
		}
		template<typename TState>
		static std::function<void()> PushTo(const TState& state)
		{
			return PushTo((int)state);
		}
	};
}