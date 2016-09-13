#include "MenuUiListener.hpp"
#include "MenuUi.hpp"
#include <GameEngine/Constants.hpp>
#include "InitIngameState.hpp"
#include <Urho3D/UI/UIEvents.h>

namespace FrontendCXX
{
MenuUiListener::MenuUiListener (MenuUi *ui) : Urho3D::Object (ui->GetContext ())
{
    isSubscribed_ = false;
    ui_ = ui;
}

void MenuUiListener::Subscribe ()
{
    if (!isSubscribed_)
    {
        isSubscribed_ = true;
        SubscribeToEvent (ui_->level1Button_, Urho3D::E_CLICK, URHO3D_HANDLER (MenuUiListener, LevelClicked));
        SubscribeToEvent (ui_->level2Button_, Urho3D::E_CLICK, URHO3D_HANDLER (MenuUiListener, LevelClicked));
        SubscribeToEvent (ui_->level3Button_, Urho3D::E_CLICK, URHO3D_HANDLER (MenuUiListener, LevelClicked));
        SubscribeToEvent (ui_->exitButton_, Urho3D::E_CLICK, URHO3D_HANDLER (MenuUiListener, ExitClicked));
    }
}

void MenuUiListener::Unsubscribe ()
{
    UnsubscribeFromAllEvents ();
    isSubscribed_ = false;
}

MenuUiListener::~MenuUiListener ()
{
    Unsubscribe ();
}

void MenuUiListener::LevelClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::UIElement *button = (Urho3D::UIElement *) eventData [Urho3D::Click::P_ELEMENT].GetPtr ();
    Urho3D::String levelName = button->GetVar (Constants::LevelNameVarHash).GetString ();
    IngameStateFunctions::InitIngameState (context_,
                                           Urho3D::String ("Levels/") + levelName + Urho3D::String (".tmx"));
}

void MenuUiListener::ExitClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    context_->GetSubsystem <Urho3D::Engine> ()->Exit ();
}
}
