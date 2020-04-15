#include <Source/TileGroup.h>
#include <Source/Assert.h>
#include <Source/Utils.h>

namespace Mini
{
    // ==================================================
    // class TileGroup implementation
    // ==================================================
    TileGroup::TileGroup(const TileGroupType type, const std::vector<Tile*>& tiles, Tile *calledTile, bool argIsCalled)
    {
        std::vector<Tile*> tmpTiles = tiles;

        if (calledTile)
        {
            tmpTiles.emplace_back(calledTile);
        }

        SortTiles(tmpTiles);

        if (type == TileGroupType::Head)
        {
            debug_assert(tmpTiles.size() == 2, "Head's tiles count must be 2");
            debug_assert(tmpTiles[0]->GetIdentifier() == tmpTiles[1]->GetIdentifier(), "Head's tiles must be same");
            debug_assert(calledTile == nullptr, "Head can't be called");
            debug_assert(argIsCalled == false, "Head can't be called");
        }

        if (type == TileGroupType::Koutsu)
        {
            debug_assert(tmpTiles.size() == 3, "Koutsu's tiles count must be 3");
            debug_assert(tmpTiles[0]->GetIdentifier() == tmpTiles[1]->GetIdentifier() && tmpTiles[1]->GetIdentifier() == tmpTiles[2]->GetIdentifier(), "Koutsu's tiles must be same");
        }

        if (type == TileGroupType::Kangtsu)
        {
            debug_assert(tmpTiles.size() == 4, "Kangtsu's tiles count must be 3");
            debug_assert(tmpTiles[0]->GetIdentifier() == tmpTiles[1]->GetIdentifier() && tmpTiles[1]->GetIdentifier() == tmpTiles[2]->GetIdentifier() && tmpTiles[2]->GetIdentifier() == tmpTiles[3]->GetIdentifier(), "Kangtsu's tiles must be same");
        }

        if (type == TileGroupType::Shuntsu)
        {
            debug_assert(tmpTiles.size() == 3, "Shuntsu's tiles count must be 3");
            debug_assert((tmpTiles[2]->GetIdentifier() - tmpTiles[1]->GetIdentifier()) == 1 && (tmpTiles[1]->GetIdentifier() - tmpTiles[0]->GetIdentifier()) == 1, "Shuntsu's tiles must be consecutive");
        }

        tgType = type;
        tgTiles = tmpTiles;
        isCalled = argIsCalled;
    }

    // Setters
    void TileGroup::SetType(const TileGroupType type)
    {
        tgType = type;
    }

    void TileGroup::SetIsCalled(const bool value)
    {
        isCalled = value;
    }

    // Getters
    TileGroupType TileGroup::GetType() const
    {
        return tgType;
    }

    bool TileGroup::GetIsCalled() const
    {
        return isCalled;
    }

    const std::vector<Tile*>& TileGroup::GetReadOnlyTiles() const
    {
        return tgTiles;
    }

    std::string TileGroup::ToString() const
    {
        std::string ret;
        for (auto& tile : tgTiles)
        {
            ret += tile->ToString() + " ";
        }
        return ret;
    }

    void TileGroup::Sort()
    {
        SortTiles(tgTiles);
    }

} // namespace Mini