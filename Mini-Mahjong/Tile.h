#ifndef Mini_Mahjong_Tile_H__
#define Mini_Mahjong_Tile_H__

#pragma once

#include <stdint.h>

#include "Constant.h"

namespace mahjong
{
	class Tile
	{
	public:
		Tile(const TileType tileType, const int data, const bool isDora);
		
		TileType getTileType() const;
		int getTileNumber() const;
		uint8_t getData() const;

		bool isDora() const;

		bool operator==(const Tile& t) const;
		bool operator!=(const Tile& t) const;
		bool operator<(const Tile& t) const;
		bool operator<=(const Tile& t) const;
		bool operator>(const Tile& t) const;
		bool operator>=(const Tile& t) const;
	private:
		uint8_t m_data;
		bool m_isDora;
	};
}

#endif