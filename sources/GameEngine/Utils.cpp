#include "Utils.hpp"
#include <Urho3D/Urho2D/TileMap2D.h>
#include <StatesEngine/StatesEngine.hpp>

namespace GameEngine
{
Urho3D::Node *FindNode (Urho3D::Context *context, Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    StatesEngine::StatesEngineSubsystem *statesEngine = context->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    StatesEngine::SceneContainer *container = scene;

    if (!container && statesEngine->HasState ())
        container = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> ();

    if (container)
        return container->GetNode (wayToNode);
    else
        return 0;
}

bool IsTileEmpty (int x, int y, Urho3D::TileMapLayer2D *layer)
{
    if (IsTileMapCoordinateValid (x, y, layer->GetTileMap ()))
        return (!layer->GetTileNode (x, y));
    else
        return false;
}

bool IsTileMapCoordinateValid (float x, float y, Urho3D::TileMap2D *tileMap)
{
    return (x >= 0 && y >= 0 && x <= tileMap->GetInfo ().width_ &&
            y <= tileMap->GetInfo ().height_);
}

int CountOfEmptyTilesAround (int tileX, int tileY, Urho3D::TileMapLayer2D *layer)
{
    int count = 0;
    for (int x = tileX - 1; x < tileX + 2; x++)
        for (int y = tileY - 1; y < tileY + 2; y++)
            if (x != tileX && y != tileY && IsTileEmpty (x, y, layer))
                count++;
    return count;
}
}
