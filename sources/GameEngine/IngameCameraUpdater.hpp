#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/SceneContainer.hpp>
#include <StatesEngine/StateObject.hpp>

namespace GameEngine
{
class IngameCameraUpdater : public StatesEngine::StateObject
{
URHO3D_OBJECT (IngameCameraUpdater, StatesEngine::StateObject)
protected:
    Urho3D::Node *cameraNode_;
    Urho3D::Node *playerNode_;
    Urho3D::Node *tileMapNode_;
public:
    IngameCameraUpdater (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    Urho3D::Node *GetCameraNode ();
    void SetCameraNode (Urho3D::Node *cameraNode);
    void SetCameraNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);

    Urho3D::Node *GetPlayerNode ();
    void SetPlayerNode (Urho3D::Node *playerNode);
    void SetPlayerNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);

    Urho3D::Node *GetTileMapNode ();
    void SetTileMapNode (Urho3D::Node *tileMapNode);
    void SetTileMapNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);
    virtual ~IngameCameraUpdater ();
};
}
