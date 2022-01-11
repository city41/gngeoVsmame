# GnGeo vs MAME

So far this repo has a tiny ngdevkit program that shows sprite scaling differences in GnGeo vs MAME.

## The S and C ROMs

Are located in the tileRoms directory. Here they are as loaded in [neospriteviewer](https://city41.github.io/neospriteviewer/)

### C ROMs

The inputs  

![numbered tiles scaled](./numberedTiles_scaled.png)  

![numbered tiles ](./numberedTiles.png)

![c roms in viewer](./cTilesInNeoSpriteViewer.png)

The "dotted" roms are the same images as the number tiles, but scaled down to 8x8, then the blank rows/cols are added so that when the Neo Geo scales the sprite down to 8x8, you get a cleanly scaled tile. This is commonly done in games that use tiles that are always scaled to 8x8.l

### S ROM

![s rom in viewer](./sTilesInNeoSpriteViewer.png)

## GnGeo result

![gngeo result](./gngeoResult2.png)

## MAME (0.220) result

![mame result](./mame0220result.png)

