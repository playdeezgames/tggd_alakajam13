#include "Application.UIState.h"
#include <stack>
namespace application
{
	static std::optional<int> finalState;

	void UIState::SetFinalState(int s)
	{
		finalState = s;
	}

	int UIState::GetFinalState()
	{
		return finalState.value();
	}


	static int uiState = 0;//TODO: we may need to set this!
	static std::stack<int> stateStack;

	void UIState::Write(int state)
	{
		uiState = state;
	}

	std::function<void()> UIState::GoTo(int state)
	{
		return [state]()
		{
			Write(state);
		};
	}

	int UIState::Read()
	{
		return uiState;
	}

	void UIState::Push(int state)
	{
		stateStack.push(uiState);
		Write(state);
	}

	std::function<void()> UIState::PushTo(int state)
	{
		return [state]() {
			Push(state);
		};
	}

	void UIState::Pop()
	{
		uiState = stateStack.top();
		stateStack.pop();
	}

	bool UIState::CanPop()
	{
		return !stateStack.empty();
	}

}