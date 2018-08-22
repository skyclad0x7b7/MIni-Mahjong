#include "Yaku.h"

#include <algorithm>

namespace mahjong
{
	Yaku::Yaku() {}
	Yaku::Yaku(const Yaku& other) {}
	Yaku::~Yaku() {}

	Yaku* Yaku::GetInstance()
	{
		static Yaku ins;
		return &ins;
	}

	void Yaku::reset()
	{
		m_completedTiles.head.reset();
		for (auto it : m_completedTiles.body)
		it.reset();
		m_toitsuNum = 0;
	}

	std::vector<CompletedTiles> Yaku::testGetYaku(const std::vector<Tile>& curTiles, const std::vector<TileGroup>& openedMentsu, const Tile& agariTile, bool isClaimed, bool isTsumo)
	{
		reset();
		m_completedTiles.body = openedMentsu;
		std::vector<CompletedTiles> ret = getAllCompletedTiles(curTiles, agariTile, isTsumo);
		return ret;
	}


	std::vector<CompletedTiles> Yaku::getAllCompletedTiles(const std::vector<Tile>& curTiles, const Tile& agariTile, bool isTsumo)
	{
		std::vector<CompletedTiles> ret;

		std::vector<CompletedTiles> toitsuCompletedTiles;
		std::vector<CompletedTiles> shuntsuCompletedTiles;
		std::vector<CompletedTiles> koutsuCompletedTiles;

		bool flag = false;
		unsigned int index = 0;

		std::vector<Tile> tmpTiles;
		while (index < curTiles.size())
		{
			if (index > 0 && curTiles[index] == curTiles[index - 1])
			{ // Skip same tiles
				index++;
				continue;
			}

			// 1. Find Toitsu (Head)
			if (m_toitsuNum == 0)
			{
				if (std::count(std::begin(curTiles), std::end(curTiles), curTiles[index]) >= 2)
				{
					flag = true;
					tmpTiles = curTiles;
					TileGroup tmpHead;
					tmpHead.setTileGroupType(TileGroupType::Toitsu);
					for (int i = 0; i < 2; i++) {
						tmpHead.putTile(*std::find(std::begin(tmpTiles), std::end(tmpTiles), curTiles[index]));
						tmpTiles.erase(std::find(std::begin(tmpTiles), std::end(tmpTiles), curTiles[index]));
					}
					m_completedTiles.head = tmpHead;
					m_toitsuNum++;
					toitsuCompletedTiles = getAllCompletedTiles(tmpTiles, agariTile, isTsumo);
					m_toitsuNum--;
				}

			}

			// 2. Find Koutsu
			if (std::count(std::begin(curTiles), std::end(curTiles), curTiles[index]) == 3)
			{
				flag = true;
				tmpTiles = curTiles;
				TileGroup tmpKoutsu;
				tmpKoutsu.setTileGroupType(TileGroupType::Ankou);
				for (int i = 0; i < 3; i++) {
					tmpKoutsu.putTile(*std::find(std::begin(tmpTiles), std::end(tmpTiles), curTiles[index]));
					tmpTiles.erase(std::find(std::begin(tmpTiles), std::end(tmpTiles), curTiles[index]));
				}
				m_completedTiles.body.push_back(tmpKoutsu);
				koutsuCompletedTiles = getAllCompletedTiles(tmpTiles, agariTile, isTsumo);
				m_completedTiles.body.erase(std::find(std::begin(m_completedTiles.body), std::end(m_completedTiles.body), tmpKoutsu));
			}

			// 3. Find Shuntsu : Sorted
			if (curTiles[index].getTileType() != TileType::Special &&
				std::find_if(std::begin(curTiles), std::end(curTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber() + 1); }) != std::end(curTiles) &&
				std::find_if(std::begin(curTiles), std::end(curTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber() + 2); }) != std::end(curTiles))
			{
				flag = true;
				TileGroup tmpShuntsu;
				tmpShuntsu.setTileGroupType(TileGroupType::Shuntsu);
				tmpTiles = curTiles;
				tmpShuntsu.putTile(*std::find_if(std::begin(tmpTiles), std::end(tmpTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber()); }));
				tmpTiles.erase(std::find_if(std::begin(tmpTiles), std::end(tmpTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber()); }));
				tmpShuntsu.putTile(*std::find_if(std::begin(tmpTiles), std::end(tmpTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber() + 1); }));
				tmpTiles.erase(std::find_if(std::begin(tmpTiles), std::end(tmpTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber() + 1); }));
				tmpShuntsu.putTile(*std::find_if(std::begin(tmpTiles), std::end(tmpTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber() + 2); }));
				tmpTiles.erase(std::find_if(std::begin(tmpTiles), std::end(tmpTiles), [&](const Tile& t) { return (t.getTileType() == curTiles[index].getTileType() && t.getTileNumber() == curTiles[index].getTileNumber() + 2); }));

				m_completedTiles.body.push_back(tmpShuntsu);
				shuntsuCompletedTiles = getAllCompletedTiles(tmpTiles, agariTile, isTsumo);
				m_completedTiles.body.erase(std::find(std::begin(m_completedTiles.body), std::end(m_completedTiles.body), tmpShuntsu));
			}
			index++;
		}

		// 4. Rest Tile Check
		tmpTiles.clear(); // Used to save agariTiles
		if (flag == false) // false flag means there's no more tile which can be Mentsu
		{
			if (curTiles.size() == 1 && curTiles[0] == agariTile) // Head-wait
			{
				TileGroup head;
				head.setTileGroupType(TileGroupType::Toitsu);
				head.putTile(agariTile);
				head.putTile(curTiles[0]);
				m_completedTiles.head = head;
				ret.push_back(m_completedTiles);
			}
			else if (curTiles.size() == 2) // body-wait
			{
				TileGroup body;
				// Check Shuntsu - Left Tile
				if (agariTile.getTileType() == curTiles[0].getTileType() && agariTile.getTileType() == curTiles[1].getTileType() &&
					agariTile.getTileNumber() == (curTiles[0].getTileNumber() - 1) && agariTile.getTileNumber() == (curTiles[1].getTileNumber() - 2)
					)
				{
					body.setTileGroupType(TileGroupType::Shuntsu);
					body.putTile(agariTile);
					body.putTile(curTiles[0]);
					body.putTile(curTiles[1]);
					ret.push_back(m_completedTiles);
				}
				// Check Shuntsu - Center Tile
				else if (agariTile.getTileType() == curTiles[0].getTileType() && agariTile.getTileType() == curTiles[1].getTileType() &&
					agariTile.getTileNumber() == (curTiles[0].getTileNumber() + 1) && agariTile.getTileNumber() == (curTiles[1].getTileNumber() - 1)
					)
				{
					body.setTileGroupType(TileGroupType::Shuntsu);
					body.putTile(curTiles[0]);
					body.putTile(agariTile);
					body.putTile(curTiles[1]);
					ret.push_back(m_completedTiles);
				}
				// Check Shuntsu - Right Tile
				else if (agariTile.getTileType() == curTiles[0].getTileType() && agariTile.getTileType() == curTiles[1].getTileType() &&
					agariTile.getTileNumber() == (curTiles[0].getTileNumber() + 2) && agariTile.getTileNumber() == (curTiles[1].getTileNumber() + 1)
					)
				{
					body.setTileGroupType(TileGroupType::Shuntsu);
					body.putTile(curTiles[0]);
					body.putTile(curTiles[1]);
					body.putTile(agariTile);
					ret.push_back(m_completedTiles);
				}
				// Check Toitsu
				else if (agariTile == curTiles[0] && agariTile == curTiles[1])
				{
					body.setTileGroupType(isTsumo ? TileGroupType::Ankou : TileGroupType::Minkou);
					body.putTile(agariTile);
					body.putTile(curTiles[0]);
					body.putTile(curTiles[1]);
					ret.push_back(m_completedTiles);
				}
				else
				{
					return std::vector<CompletedTiles>();
				}
			}
			else // Wrong tiles
			{
				return std::vector<CompletedTiles>();
			}
		}

		ret.insert(std::end(ret), std::begin(toitsuCompletedTiles), std::end(toitsuCompletedTiles));
		ret.insert(std::end(ret), std::begin(shuntsuCompletedTiles), std::end(shuntsuCompletedTiles));
		ret.insert(std::end(ret), std::begin(koutsuCompletedTiles), std::end(koutsuCompletedTiles));
		return ret;
	}
}