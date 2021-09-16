#include <stack>
#include "Visuals.Confirmations.h"
namespace visuals
{
	static std::stack<Confirmation> confirmations;

	bool Confirmations::HasConfirmation()
	{
		return !confirmations.empty();
	}

	Confirmation Confirmations::Read()
	{
		Confirmation result = confirmations.top();
		confirmations.pop();
		return result;
	}

	void Confirmations::Write(const Confirmation& confirmation)
	{
		confirmations.push(confirmation);
	}

	void Confirmations::Reset()
	{
		while (HasConfirmation())
		{
			Read();
		}
	}
}
