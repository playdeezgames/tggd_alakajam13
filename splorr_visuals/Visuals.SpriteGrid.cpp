#include <Application.Engine.h>
#include <Common.XY.h>
#include <functional>
#include <json.hpp>
#include <map>
#include <vector>
#include "Visuals.Colors.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
#include "Visuals.Sprites.h"
#include "Visuals.SpriteGrid.h"
namespace visuals
{
	struct InternalSpriteCell
	{
		std::string sprite;
		std::string color;
	};

	struct InternalSpriteGrid
	{
		common::XY<int> xy;
		common::XY<int> cellSize;
		bool dropShadow;
		std::string dropShadowColor;
		common::XY<int> dropShadowXY;
		std::map<int, std::map<int, InternalSpriteCell>> cells;
	};

	static std::vector<InternalSpriteGrid> spriteGrids;
	static std::map<std::string, std::map<std::string, size_t>> spriteGridTable;

	static void DrawInternalSpriteGrid(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t gridIndex)
	{
		auto& spriteGrid = spriteGrids[gridIndex];
		for (auto& gridColumn : spriteGrid.cells)
		{
			int plotX = spriteGrid.xy.GetX() + gridColumn.first * spriteGrid.cellSize.GetX();
			for (auto& gridCell : gridColumn.second)
			{
				int plotY = spriteGrid.xy.GetY() + gridCell.first * spriteGrid.cellSize.GetY();
				auto& cell = gridCell.second;
				common::XY<int> plot = { plotX, plotY };
				if (spriteGrid.dropShadow)
				{
					visuals::Sprites::Draw(cell.sprite, renderer, plot + spriteGrid.dropShadowXY, visuals::Colors::Read(spriteGrid.dropShadowColor));
				}
				visuals::Sprites::Draw(cell.sprite, renderer, plot, visuals::Colors::Read(cell.color));
			}
		}
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> SpriteGrid::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t gridIndex = spriteGrids.size();
		spriteGrids.push_back(
			{
				common::XY<int>(
					model[visuals::data::Properties::X],
					model[visuals::data::Properties::Y]),
				common::XY<int>(
					model[visuals::data::Properties::CELL_WIDTH],
					model[visuals::data::Properties::CELL_HEIGHT]),
				(bool)model[visuals::data::Properties::DROP_SHADOW],
				model[visuals::data::Properties::DROP_SHADOW_COLOR],
				common::XY<int>(
					model[visuals::data::Properties::DROP_SHADOW_X],
					model[visuals::data::Properties::DROP_SHADOW_Y])
			});
		if (model.count(visuals::data::Properties::SPRITE_GRID_ID) > 0)
		{
			std::string imageId = model[visuals::data::Properties::SPRITE_GRID_ID];
			spriteGridTable[layoutName][imageId] = gridIndex;
		}
		return [gridIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalSpriteGrid(renderer, gridIndex);
		};
	}

	void SpriteGrid::SetCell(const std::string& layoutName, const std::string& spriteGridId, const common::XY<int>& location, const std::string& spriteName, const std::string& color)
	{
		size_t gridIndex = spriteGridTable[layoutName][spriteGridId];
		auto& spriteGrid = spriteGrids[gridIndex];
		spriteGrid.cells[location.GetX()][location.GetY()] =
		{
			spriteName,
			color
		};
	}

	void SpriteGrid::ClearCell(const std::string& layoutName, const std::string& spriteGridId, const common::XY<int>& location)
	{
		size_t gridIndex = spriteGridTable[layoutName][spriteGridId];
		auto& spriteGrid = spriteGrids[gridIndex];
		auto gridColumn = spriteGrid.cells.find(location.GetX());
		if (gridColumn != spriteGrid.cells.end())
		{
			auto gridCell = gridColumn->second.find(location.GetY());
			if (gridCell != gridColumn->second.end())
			{
				gridColumn->second.erase(gridCell);
			}
		}
	}

	static void DoWriteText(const std::string& layoutName, const std::string& spriteGridId, const common::XY<int>& location, const std::string& fontName, const std::string& text, const std::string& color)
	{
		auto currentLocation = location;
		size_t gridIndex = spriteGridTable[layoutName][spriteGridId];
		auto& spriteGrid = spriteGrids[gridIndex];
		for (auto& ch : text)
		{
			spriteGrid.cells[currentLocation.GetX()][currentLocation.GetY()] =
			{
				visuals::Fonts::GetGlyphSpriteName(fontName, ch).value(),
				color
			};
			currentLocation = currentLocation + common::XY<int>(1, 0);
		}
	}

	void SpriteGrid::WriteText(const std::string& layoutName, const std::string& spriteGridId, const common::XY<int>& location, const std::string& fontName, const std::string& text, const std::string& color, const visuals::HorizontalAlignment& alignment)
	{
		auto x = location.GetX();
		switch (alignment)
		{
		case visuals::HorizontalAlignment::CENTER:
			x -= (int)text.size() / 2;
			break;
		case visuals::HorizontalAlignment::RIGHT:
			x -= (int)text.size();
			break;
		}
		DoWriteText(layoutName, spriteGridId, { x,location.GetY() }, fontName, text, color);
	}

	void SpriteGrid::Clear(const std::string& layoutName, const std::string& spriteGridId)
	{
		size_t gridIndex = spriteGridTable[layoutName][spriteGridId];
		auto& spriteGrid = spriteGrids[gridIndex];
		spriteGrid.cells.clear();
	}

	int SpriteGrid::GetCellHeight(const std::string& layoutName, const std::string& spriteGridId)
	{
		size_t gridIndex = spriteGridTable[layoutName][spriteGridId];
		auto& spriteGrid = spriteGrids[gridIndex];
		return spriteGrid.cellSize.GetY();
	}

	std::function<void(const common::XY<int>&, const std::string&, const std::string&)>
		SpriteGrid::DoWriteToGrid(
			const std::string& layoutName,
			const std::string& spriteGridId,
			const std::string& fontName,
			const visuals::HorizontalAlignment& alignment)
	{
		return [layoutName, spriteGridId, fontName, alignment](const common::XY<int> location, const std::string & text, const std::string & color)
		{
			visuals::SpriteGrid::WriteText(layoutName, spriteGridId, location, fontName, text, color, alignment);
		};
	}
}
