#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Scene/Node.h>
#undef GetObject
#include <Urho3D/Urho2D/TileMapLayer2D.h>

class IngameStateFunctions
{
protected:
    static void ParseTileMap (Urho3D::Context *context, Urho3D::Node *tileMapNode);
    static bool ParseObjectsLayer (Urho3D::Context *context,
                                   Urho3D::Node *objectsNode, Urho3D::TileMapLayer2D *layer);
    static bool ParseWallsLayer (Urho3D::Context *context, Urho3D::TileMapLayer2D *layer);
public:
    static void InitIngameState (Urho3D::Context *context, Urho3D::String pathToLevel);
    static bool IsTileEmpty (int x, int y, Urho3D::TileMapLayer2D *layer);
    static bool IsTileMapCoordinateValid (float x, float y, Urho3D::TileMap2D *tileMap);
    static int CountOfEmptyTilesAround (int tileX, int tileY, Urho3D::TileMapLayer2D *layer);
};
