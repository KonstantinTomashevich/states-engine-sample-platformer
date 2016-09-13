#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/SceneContainer.hpp>

#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Urho2D/TileMapLayer2D.h>
#include "UnitBasis.hpp"

namespace GameEngine
{
class WarriorAi : public UnitBasis
{
URHO3D_OBJECT (WarriorAi, UnitBasis)
protected:
    float scanningRadius_;
    float newDecinisionTime_;
    float timeFromLastDecinision_;
    Urho3D::Node *tileMapNode_;

    void UpdateAnimation ();
    UnitInterface *GetNearestEnemy ();
    void UpdateAi (UnitInterface *enemy, float timeStep);
    Urho3D::TileMapLayer2D *FindWallsLayer ();
public:
    WarriorAi (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    float GetScanningRadius ();
    void SetScanningRadius (float scanningRadius);
    float GetNewDecinisionTime ();
    void SetNewDecinisionTime (float newDecinisionTime);

    Urho3D::Node *GetTileMapNode ();
    void SetTileMapNode (Urho3D::Node *tileMapNode);
    void SetTileMapNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);
    virtual ~WarriorAi ();
};
}
