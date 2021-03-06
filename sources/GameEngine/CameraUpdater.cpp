#include "CameraUpdater.hpp"
#include <StatesEngine/StatesEngine.hpp>
#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/IO/Log.h>
#include "Utils.hpp"

namespace GameEngine
{
CameraUpdater::CameraUpdater (Urho3D::Context *context) : StateObject (context)
{
    cameraNode_ = 0;
    playerNode_ = 0;
    tileMapNode_ = 0;
}

bool CameraUpdater::Init ()
{
    if (cameraNode_ && playerNode_ && tileMapNode_)
    {
        ready_ = true;
        return true;
    }
    else
    {
        ready_ = false;
        return false;
    }
}

bool CameraUpdater::Update (float timeStep)
{
    Urho3D::Camera *camera = cameraNode_->GetComponent <Urho3D::Camera> ();
    Urho3D::TileMap2D *tileMap = tileMapNode_->GetComponent <Urho3D::TileMap2D> ();
    Urho3D::TileMapInfo2D mapInfo = tileMap->GetTmxFile ()->GetInfo ();

    float halfHeight = camera->GetOrthoSize () / 2;
    float halfWidth = halfHeight * camera->GetAspectRatio ();

    float minimumX = tileMapNode_->GetPosition ().x_ + halfWidth;
    float minimumY = tileMapNode_->GetPosition ().y_ + halfHeight;
    float maximumX = mapInfo.GetMapWidth () - halfWidth;
    float maximumY = mapInfo.GetMapHeight () - halfHeight;

    Urho3D::Vector3 newCameraPosition = playerNode_->GetPosition ();
    if (newCameraPosition.x_ < minimumX)
        newCameraPosition.x_ = minimumX;
    if (newCameraPosition.y_ < minimumY)
        newCameraPosition.y_ = minimumY;

    if (newCameraPosition.x_ > maximumX)
        newCameraPosition.x_ = maximumX;
    if (newCameraPosition.y_ > maximumY)
        newCameraPosition.y_ = maximumY;
    cameraNode_->SetPosition (newCameraPosition);
}

bool CameraUpdater::Dispose ()
{
    ready_ = false;
    return true;
}

Urho3D::Node *CameraUpdater::GetCameraNode ()
{
    return cameraNode_;
}

void CameraUpdater::SetCameraNode (Urho3D::Node *cameraNode)
{
    cameraNode_ = cameraNode;
}

void CameraUpdater::SetCameraNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    SetCameraNode (FindNode (context_, wayToNode, scene));
}

Urho3D::Node *CameraUpdater::GetPlayerNode ()
{
    return playerNode_;
}

void CameraUpdater::SetPlayerNode (Urho3D::Node *playerNode)
{
    playerNode_ = playerNode;
}

void CameraUpdater::SetPlayerNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    SetPlayerNode (FindNode (context_, wayToNode, scene));
}

Urho3D::Node *CameraUpdater::GetTileMapNode()
{
    return tileMapNode_;
}

void CameraUpdater::SetTileMapNode (Urho3D::Node *tileMapNode)
{
    tileMapNode_ = tileMapNode;
}

void CameraUpdater::SetTileMapNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    SetTileMapNode (FindNode (context_, wayToNode, scene));
}

CameraUpdater::~CameraUpdater ()
{
    Dispose ();
}
}

