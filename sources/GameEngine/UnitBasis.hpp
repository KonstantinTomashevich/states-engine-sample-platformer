#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/SceneContainer.hpp>
#include "UnitInterface.hpp"

namespace GameEngine
{
class UnitBasis : public UnitInterface
{
URHO3D_OBJECT (UnitBasis, UnitInterface)
protected:
    Urho3D::Node *node_;
    Urho3D::StringHash team_;
    float maxSpeed_;
    float jumpForce_;
    float timeBeforeNewJump_;
    bool isBlockingNow_;

    bool isAttackingNow_;
    float lives_;
    float maxLives_;

    float livesRegeneration_;
    float timeFromAttackStart_;
    bool isAttackExecuted_;

    float attackDamage_;
    float timeFromLastDamage_;
    float timeFromDie_;
    float dieTime_;

    float DistanceToGround ();
    void Jump ();
    void SetToBlock ();
    void SetToAttack ();
    void SetToMoving ();
    void UpdateAttack (float timeStep);
public:
    UnitBasis (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    virtual Urho3D::Vector3 GetPosition ();
    virtual bool IsAttackingNow ();
    virtual bool IsBlockingNow ();
    virtual bool OnAtttack (Urho3D::StringHash attackerTeam, float damage);
    virtual Urho3D::StringHash GetTeam ();
    void SetTeam (Urho3D::StringHash team);
    virtual Urho3D::Rect GetLocalRect ();

    Urho3D::Node *GetNode ();
    void SetNode (Urho3D::Node *node);
    void SetNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);
    float GetAttackDamage ();
    void SetAttackDamage (float attackDamage);

    float GetMaxSpeed ();
    void SetMaxSpeed (float maxSpeed);
    float GetJumpForce ();
    void SetJumpForce (float jumpForce);
    virtual float GetLives ();
    virtual float GetMaxLives ();
    virtual float GetLivesRegeneration ();
    virtual ~UnitBasis ();
};
}
