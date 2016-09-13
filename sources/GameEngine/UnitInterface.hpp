#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <StatesEngine/StateObject.hpp>

namespace GameEngine
{
class UnitInterface : public StatesEngine::StateObject
{
URHO3D_OBJECT (UnitInterface, StatesEngine::StateObject)
public:
    UnitInterface (Urho3D::Context *context);
    virtual Urho3D::Vector3 GetPosition () = 0;
    virtual bool IsAttackingNow () = 0;
    virtual bool IsBlockingNow () = 0;
    virtual bool OnAttack (Urho3D::StringHash attackerTeam, float damage) = 0;
    virtual Urho3D::StringHash GetTeam () = 0;
    virtual Urho3D::Rect GetLocalRect () = 0;
    virtual float GetLives () = 0;
    virtual float GetMaxLives () = 0;
    virtual float GetLivesRegeneration () = 0;
    virtual ~UnitInterface ();
};
}

