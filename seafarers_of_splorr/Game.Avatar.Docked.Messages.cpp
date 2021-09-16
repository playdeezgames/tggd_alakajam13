#pragma once
#include <Game.Colors.h>
#include <Visuals.Messages.h>
namespace game::avatar
{
	void OnEnterDarkAlleyFailsInfamyRequirement()
	{
		visuals::Messages::Write(
			{
				"==FIGHT!==",
				{
					{
						{19,9},
						"You have been waylaid by a ruffian!",
						game::Colors::RED,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,11},
						"Prepare to fight!",
						game::Colors::RED,
						visuals::HorizontalAlignment::CENTER
					}
				}
			});
	}
}