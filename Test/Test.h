#ifndef __MINI_MAHJONG_TEST_H__
#define __MINI_MAHJONG_TEST_H__

#include <Source/Yaku.h>

#include <vector>

namespace Mini
{
    void Test01();
    void Test02();
    void Test03();
    void Test04();
    void Test05();
    void Test06();
    void Test07();
    void Test08();
    void Test09();
    void Test10();
    void Test11();
    void Test12();
    void Test13();

    /* Utility for Test */
    void CalcAndPrintYaku(std::vector<Yaku*> yakuList, const ReassembledTileGroup& reassembledTileGroup, const Tile *pickedTile, bool isMenzen, bool isRon, WindType roundWind, WindType selfWind);
    void CheckPossibleAndPrintResult(const std::vector<TileGroup>& calledTileGroupList, const std::vector<const Tile *>& handTiles, const Tile *pickedTile, std::vector<ReassembledTileGroup>& result);
}

#endif // __MINI_MAHJONG_TEST_H__