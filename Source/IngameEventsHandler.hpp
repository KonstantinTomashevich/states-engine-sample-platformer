#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/StringHash.h>
#include <StatesEngine/StateObject.hpp>
#include "IngamePlayerController.hpp"

class IngameEventsHandler : public StatesEngine::StateObject
{
URHO3D_OBJECT (IngameEventsHandler, StatesEngine::StateObject)
protected:
    bool isSubscribed_;
    Urho3D::SharedPtr <IngamePlayerController> playerController_;

    void PickupCoin (Urho3D::Node *coin);
public:
    IngameEventsHandler (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    Urho3D::SharedPtr <IngamePlayerController> GetPlayerController ();
    void SetPlayerController (Urho3D::SharedPtr <IngamePlayerController> playerController);
    void OnPhysicsCollision (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnAttackInArea (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    virtual ~IngameEventsHandler ();
};
