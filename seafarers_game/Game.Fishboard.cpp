#include <Common.RNG.h>
#include <Data.Game.FishBoard.h>
#include <Data.Game.FishGame.h>
#include "Game.Fishboard.h"
#include "Game.Fisheries.h"
#include "Game.Fishes.h"
#include <map>
namespace game
{
	const size_t Fishboard::COLUMNS = 5;
	const size_t Fishboard::ROWS = 5;

	static int INITIAL_GUESSES = 10;

	static void ClearFishGame()
	{
		data::game::FishGame::Clear();
		data::game::FishGame::WriteGuesses(INITIAL_GUESSES);
	}

	static void ClearFishboard()
	{
		data::game::FishBoard::Clear();
		for (size_t column = 0; column < Fishboard::COLUMNS; ++column)
		{
			for (size_t row = 0; row < Fishboard::ROWS; ++row)
			{
				data::game::FishBoard::Write({
					{(int)column,(int)row},
					false,
					std::nullopt});
			}
		}
	}

	static Fish GenerateFish()
	{
		std::map<Fish, size_t> fishGenerator;
		auto fisheries = Fisheries::Available();
		for (auto fishery : fisheries)
		{
			fishGenerator[fishery.fish] = fishGenerator[fishery.fish] + 1;
		}
		return common::RNG::FromGenerator(fishGenerator, Fish::COD);//TODO: have a different default fish
	}

	void Fishboard::Generate()
	{
		ClearFishGame();
		ClearFishboard();

		auto fish = GenerateFish();
		auto descriptor = Fishes::Read(fish);
		int x = (int)common::RNG::FromRange(0u, COLUMNS + 1u - descriptor.size.GetX());
		int y = (int)common::RNG::FromRange(0u, ROWS + 1u - descriptor.size.GetY());
		common::XY<int> origin = { x,y };
		for (auto& location : descriptor.shape)
		{
			data::game::FishBoard::Write({
				(location + origin),
				false,
				(int)fish});
		}
	}
}