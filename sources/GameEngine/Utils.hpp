#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/SceneContainer.hpp>
#include <StatesEngine/StateObject.hpp>
#undef GetObject
#include <Urho3D/Urho2D/TileMapLayer2D.h>

namespace GameEngine
{
Urho3D::Node *FindNode (Urho3D::Context *context, Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);

template <class T> void RegisterType (Urho3D::Context *context)
{
    if (context->CreateObject (T::GetTypeNameStatic ()).Null ())
        context->RegisterFactory <T> ();
}

bool IsTileEmpty (int x, int y, Urho3D::TileMapLayer2D *layer);
bool IsTileMapCoordinateValid (float x, float y, Urho3D::TileMap2D *tileMap);
int CountOfEmptyTilesAround (int tileX, int tileY, Urho3D::TileMapLayer2D *layer);
}
