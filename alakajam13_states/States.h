#pragma once
namespace state
{ 
	struct About
	{
		static void Start(); 
	};
	struct ConfirmQuit
	{
		static void Start();
	};
	struct LeavePlay 
	{ 
		static void Start(); 
	};
	struct MainMenu
	{
		static void Start();
	};
	struct Options 
	{ 
		static void Start(); 
	};
	struct Splash 
	{ 
		static void Start(); 
	};
	struct StartGame
	{ 
		static void Start(); 
	};
	struct Tip
	{
		static void Start();
	};
	struct GameOver
	{
		static void Start();
	};
	namespace in_play
	{
		struct Board
		{
			static void Start();
		};
		struct Next
		{
			static void Start();
		};
	}
}

