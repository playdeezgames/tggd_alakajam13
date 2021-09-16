#pragma once
#include "Visuals.Confirmation.h"
namespace visuals
{
	struct Confirmations
	{
		static bool HasConfirmation();
		static Confirmation Read();
		static void Write(const Confirmation&);
		static void Reset();
	};
}
