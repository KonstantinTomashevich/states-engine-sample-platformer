#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/StringHash.h>
#include <StatesEngine/StateObject.hpp>
#include "PlayerController.hpp"

namespace GameEngine
{
class EventsHandler : public StatesEngine::StateObject
{
URHO3D_OBJECT (EventsHandler, StatesEngine::StateObject)
protected:
    bool isSubscribed_;
    PlayerController *playerController_;

    void PickupCoin (Urho3D::Node *coin);
public:
    EventsHandler (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    PlayerController *GetPlayerController ();
    void SetPlayerController (PlayerController *playerController);

    void OnPhysicsCollision (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnAttackInArea (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    virtual ~EventsHandler ();
};
}
