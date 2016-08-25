#include "IngameEventsHandler.hpp"
#include "Constants.hpp"
#include "UnitInterface.hpp"

#include <Urho3D/Scene/Node.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <StatesEngine/StatesEngine.hpp>
#include <Urho3D/IO/Log.h>

void IngameEventsHandler::PickupCoin (Urho3D::Node *coin)
{
    float cash = coin->GetVar (Constants::COIN_CASH_VAR_HASH).GetFloat ();
    playerController_->SetCoins (playerController_->GetCoins () + cash);
    coin->Remove ();
}

IngameEventsHandler::IngameEventsHandler (Urho3D::Context *context) : StateObject (context), playerController_ ()
{
    isSubscribed_ = false;
}

bool IngameEventsHandler::Init()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    if (!playerController_)
        playerController_ = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <IngamePlayerController> ();

    if (playerController_)
    {
        SubscribeToEvent (Urho3D::E_PHYSICSBEGINCONTACT2D,
                          URHO3D_HANDLER (IngameEventsHandler, OnPhysicsCollision));
        SubscribeToEvent (Constants::EVENT_ATTACK_IN_AREA,
                                  URHO3D_HANDLER (IngameEventsHandler, OnAttackInArea));
        isSubscribed_ = true;
        ready_ = true;
        return true;
    }
    else
    {
        ready_ = false;
        return false;
    }
}

bool IngameEventsHandler::Update(float timeStep)
{

}

bool IngameEventsHandler::Dispose()
{
    UnsubscribeFromAllEvents ();
    ready_ = false;
    isSubscribed_ = false;
    return true;
}

Urho3D::SharedPtr<IngamePlayerController> IngameEventsHandler::GetPlayerController ()
{
    return playerController_;
}

void IngameEventsHandler::SetPlayerController (Urho3D::SharedPtr <IngamePlayerController> playerController)
{
    playerController_ = playerController;
}

void IngameEventsHandler::OnPhysicsCollision(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Node *nodeA = (Urho3D::Node *)
            eventData [Urho3D::PhysicsBeginContact2D::P_NODEA].GetPtr ();
    Urho3D::Node *nodeB = (Urho3D::Node *)
            eventData [Urho3D::PhysicsBeginContact2D::P_NODEB].GetPtr ();

    if (nodeA == playerController_->GetNode ()&& nodeB->GetVar (
                Constants::OBJECT_TYPE_VAR_HASH).GetString () == Constants::OBJECT_TYPE_COIN)
        PickupCoin (nodeB);

    if (nodeB == playerController_->GetNode ()&& nodeA->GetVar (
                Constants::OBJECT_TYPE_VAR_HASH).GetString () == Constants::OBJECT_TYPE_COIN)
        PickupCoin (nodeA);
}

void IngameEventsHandler::OnAttackInArea (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Rect rect = Urho3D::Rect (
                eventData [Constants::AttackInAreaData::AREA_RECT].GetVector4 ());
    Urho3D::StringHash teamId = eventData [
            Constants::AttackInAreaData::ATTACKER_TEAM_ID].GetStringHash ();
    float damage = eventData [Constants::AttackInAreaData::DAMAGE].GetFloat ();

    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    Urho3D::Vector <Urho3D::SharedPtr <UnitInterface> > *units = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->GetAll <UnitInterface> ();

    if (!units->Empty ())
        for (int index = 0; index < units->Size (); index++)
        {
            UnitInterface *unitInterface = units->At (index);
            Urho3D::Rect unitRect = unitInterface->GetLocalRect ();
            unitRect.min_ += Urho3D::Vector2 (unitInterface->GetPosition ().x_,
                                              unitInterface->GetPosition ().y_);
            unitRect.max_ += Urho3D::Vector2 (unitInterface->GetPosition ().x_,
                                              unitInterface->GetPosition ().y_);
            // If intersects.
            if (((unitRect.min_.x_ >= rect.min_.x_ && unitRect.min_.x_ <= rect.max_.x_) &&
                    (unitRect.min_.y_ >= rect.min_.y_ && unitRect.min_.y_ <= rect.max_.y_)) ||
                    ((unitRect.max_.x_ >= rect.min_.x_ && unitRect.max_.x_ <= rect.max_.x_) &&
                    (unitRect.max_.y_ >= rect.min_.y_ && unitRect.max_.y_ <= rect.max_.y_)))
                unitInterface->OnAtttack (teamId, damage);
        }
    delete units;
}

IngameEventsHandler::~IngameEventsHandler()
{

}

