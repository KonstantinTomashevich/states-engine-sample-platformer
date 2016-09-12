#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <StatesEngine/StateObject.hpp>

class UnitInterface : public StatesEngine::StateObject
{
URHO3D_OBJECT (UnitInterface, StatesEngine::StateObject)
public:
    UnitInterface (Urho3D::Context *context);
    virtual Urho3D::Vector3 GetPosition () {}
    virtual bool IsAttackingNow () {}
    virtual bool IsBlockingNow () {}
    virtual bool OnAtttack (Urho3D::StringHash attackerTeam, float damage) {}
    virtual Urho3D::StringHash GetTeam () {}
    virtual Urho3D::Rect GetLocalRect () {}
    virtual float GetLives () {}
};

