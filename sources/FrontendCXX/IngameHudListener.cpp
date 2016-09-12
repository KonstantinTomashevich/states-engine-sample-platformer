#include "IngameHudListener.hpp"
#include "IngameHud.hpp"
#include "InitMainMenu.hpp"

#include <Urho3D/UI/UIEvents.h>
#include <StatesEngine/StatesEngine.hpp>
#include <GameEngine/UnitInterface.hpp>

namespace FrontendCXX
{
IngameHudListener::IngameHudListener (IngameHud *hud) : Urho3D::Object (hud->GetContext ())
{
    hud_ = hud;
    isSubscribed_ = false;
}

void IngameHudListener::Subscribe ()
{
    if (!isSubscribed_)
    {
        isSubscribed_ = true;
        SubscribeToEvent (hud_->pauseButton_, Urho3D::E_CLICK, URHO3D_HANDLER (IngameHudListener, PauseClicked));
        SubscribeToEvent (hud_->resumeButton_, Urho3D::E_CLICK, URHO3D_HANDLER (IngameHudListener, ResumeClicked));
        SubscribeToEvent (hud_->menuButton_, Urho3D::E_CLICK, URHO3D_HANDLER (IngameHudListener, MenuClicked));
        SubscribeToEvent (hud_->joystickKnob_, Urho3D::E_DRAGMOVE, URHO3D_HANDLER (IngameHudListener, JoystickDragMove));
        SubscribeToEvent (hud_->joystickKnob_, Urho3D::E_DRAGEND, URHO3D_HANDLER (IngameHudListener, JoystickDragEnd));
    }
}

void IngameHudListener::Unsubscribe ()
{
    UnsubscribeFromAllEvents ();
    isSubscribed_ = false;
}

IngameHudListener::~IngameHudListener ()
{
    Unsubscribe ();
}

void IngameHudListener::PauseClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    hud_->SetPauseWindowVisible (true);
    StatesEngine::StatesEngineSubsystem *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    Urho3D::Vector <Urho3D::SharedPtr <GameEngine::UnitInterface> > units = statesEngine->
            GetState <StatesEngine::StateObjectsManager> ()->GetAll <GameEngine::UnitInterface> ();

    if (!units.Empty ())
        for (int index = 0; index < units.Size (); index++)
            if (units.At (index))
                units.At (index)->SetIsWillBeUpdated (false);

    StatesEngine::SceneContainer *scene = statesEngine->
            GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> ();
    scene->Get ()->SetUpdateEnabled (false);
}

void IngameHudListener::ResumeClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    hud_->SetPauseWindowVisible (false);
    StatesEngine::StatesEngineSubsystem *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    Urho3D::Vector <Urho3D::SharedPtr <GameEngine::UnitInterface> > units = statesEngine->
            GetState <StatesEngine::StateObjectsManager> ()->GetAll <GameEngine::UnitInterface> ();

    if (!units.Empty ())
        for (int index = 0; index < units.Size (); index++)
            if (units.At (index))
                units.At (index)->SetIsWillBeUpdated (true);

    StatesEngine::SceneContainer *scene = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> ();
    scene->Get ()->SetUpdateEnabled (true);
}

void IngameHudListener::MenuClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    MainMenuFunctions::InitMainMenu (context_);
}

void IngameHudListener::JoystickDragMove (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    int dX = eventData [Urho3D::DragMove::P_DX].GetInt ();
    int dY = eventData [Urho3D::DragMove::P_DY].GetInt ();

    int knobX = hud_->joystickKnob_->GetPosition ().x_ + dX;
    int knobY = hud_->joystickKnob_->GetPosition ().y_ + dY;

    if (knobX <= 0)
        knobX = 0;
    if (knobX + hud_->joystickKnob_->GetWidth () > hud_->joystick_->GetWidth ())
        knobX = hud_->joystick_->GetWidth () - hud_->joystickKnob_->GetWidth ();

    if (knobY <= 0)
        knobY = 0;
    if (knobY + hud_->joystickKnob_->GetHeight () > hud_->joystick_->GetHeight ())
        knobY = hud_->joystick_->GetHeight () - hud_->joystickKnob_->GetHeight ();
    hud_->joystickKnob_->SetPosition (knobX, knobY);
}

void IngameHudListener::JoystickDragEnd (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    hud_->joystickKnob_->SetPosition (hud_->joystick_->GetWidth () / 2 - hud_->joystickKnob_->GetWidth () / 2,
                                      hud_->joystick_->GetHeight () / 2 - hud_->joystickKnob_->GetHeight () / 2);
}
}
