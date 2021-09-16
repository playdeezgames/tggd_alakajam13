#pragma once
namespace state
{ 
	struct About
	{
		static void Start(); 
	};
	struct ConfirmAbandon 
	{ 
		static void Start(); 
	};
	struct ConfirmOverwrite 
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
	struct LoadGame 
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
	struct SaveGame
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
	struct Statistics 
	{ 
		static void Start(); 
	};
	struct Tip
	{
		static void Start();
	};
}

