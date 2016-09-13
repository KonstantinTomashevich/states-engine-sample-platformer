#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Scene/Node.h>
#undef GetObject
#include <Urho3D/Urho2D/TileMapLayer2D.h>

namespace FrontendCXX
{
namespace IngameStateFunctions
{
void ParseTileMap (Urho3D::Context *context, Urho3D::Node *tileMapNode);
bool ParseObjectsLayer (Urho3D::Context *context,
                               Urho3D::Node *objectsNode, Urho3D::TileMapLayer2D *layer);
bool ParseWallsLayer (Urho3D::Context *context, Urho3D::TileMapLayer2D *layer);
void InitIngameState (Urho3D::Context *context, Urho3D::String pathToLevel);
}
}
