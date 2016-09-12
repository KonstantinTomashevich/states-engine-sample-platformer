#include "MenuUi.hpp"
#include <GameEngine/Constants.hpp>
#include <StatesEngine/StatesEngine.hpp>

#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/IO/Log.h>

namespace FrontendCXX
{
void MenuUi::InitUI ()
{
    Urho3D::UI *ui = context_->GetSubsystem <Urho3D::UI> ();
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    ui->Clear ();

    background_ = ui->GetRoot ()->CreateChild <Urho3D::Sprite> ("background");
    background_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/main_menu_background.png"));

    level1Button_ = ui->GetRoot ()->CreateChild <Urho3D::Button> ("level1Button");
    level1Button_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    level1Button_->SetVar (Constants::LEVEL_NAME_VAR_HASH, Urho3D::String ("level1"));
    level1ButtonText_ = level1Button_->CreateChild <Urho3D::Text> ("text");

    level1ButtonText_->SetText ("Level 1");
    level1ButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    level1ButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    level1ButtonText_->SetEffectStrokeThickness (2);
    level1ButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    level1ButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    level2Button_ = ui->GetRoot ()->CreateChild <Urho3D::Button> ("level2Button");
    level2Button_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    level2Button_->SetVar (Constants::LEVEL_NAME_VAR_HASH, Urho3D::String ("level2"));
    level2ButtonText_ = level2Button_->CreateChild <Urho3D::Text> ("text");

    level2ButtonText_->SetText ("Level 2");
    level2ButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    level2ButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    level2ButtonText_->SetEffectStrokeThickness (2);
    level2ButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    level2ButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    level3Button_ = ui->GetRoot ()->CreateChild <Urho3D::Button> ("level3Button");
    level3Button_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    level3Button_->SetVar (Constants::LEVEL_NAME_VAR_HASH, Urho3D::String ("level3"));
    level3ButtonText_ = level3Button_->CreateChild <Urho3D::Text> ("text");

    level3ButtonText_->SetText ("Level 3");
    level3ButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    level3ButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    level3ButtonText_->SetEffectStrokeThickness (2);
    level3ButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    level3ButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    exitButton_ = ui->GetRoot ()->CreateChild <Urho3D::Button> ("exitButton");
    exitButton_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    exitButton_->SetVar (Constants::LEVEL_NAME_VAR_HASH, Urho3D::String ("exit"));
    exitButtonText_ = exitButton_->CreateChild <Urho3D::Text> ("text");

    exitButtonText_->SetText ("Exit");
    exitButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    exitButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    exitButtonText_->SetEffectStrokeThickness (2);
    exitButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    exitButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);
}

void MenuUi::ResizeUI (int width, int height)
{
    background_->SetSize (height * 2.0f, height);
    background_->SetPosition (0, 0);

    level1Button_->SetSize (height * 0.45f, height * 0.15f);
    level1Button_->SetPosition (width - height * 0.485f, height * 0.035f);
    level1ButtonText_->SetFont (level1ButtonText_->GetFont (), height * 0.07f);

    level2Button_->SetSize (height * 0.45f, height * 0.15f);
    level2Button_->SetPosition (width - height * 0.485f, height * 0.22f);
    level2ButtonText_->SetFont (level2ButtonText_->GetFont (), height * 0.07f);

    level3Button_->SetSize (height * 0.45f, height * 0.15f);
    level3Button_->SetPosition (width - height * 0.485f, height * 0.44f);
    level3ButtonText_->SetFont (level3ButtonText_->GetFont (), height * 0.07f);

    exitButton_->SetSize (height * 0.45f, height * 0.15f);
    exitButton_->SetPosition (width - height * 0.485f, height * 0.66f);
    exitButtonText_->SetFont (exitButtonText_->GetFont (), height * 0.07f);
}

void MenuUi::UpdateUI (float timeStep)
{

}

void MenuUi::HideUI ()
{
    level1Button_->Remove ();
    level1ButtonText_->Remove ();
    level2Button_->Remove ();
    level2ButtonText_->Remove ();
    level3Button_->Remove ();
    level3ButtonText_->Remove ();
    exitButton_->Remove ();
    exitButtonText_->Remove ();
    background_->Remove ();
}

MenuUi::MenuUi (Urho3D::Context *context) : StatesEngine::StateObject (context), listener_ (this)
{
    lastWidth_ = 0;
    lastHeight_ = 0;

    level1Button_ = 0;
    level1ButtonText_ = 0;
    level2Button_ = 0;
    level2ButtonText_ = 0;
    level3Button_ = 0;
    level3ButtonText_ = 0;
    exitButton_ = 0;
    exitButtonText_ = 0;
    background_ = 0;
}

bool MenuUi::Init ()
{
    InitUI ();
    Urho3D::Graphics *graphics = context_->GetSubsystem <Urho3D::Graphics> ();
    lastWidth_ = graphics->GetWidth ();
    lastHeight_ = graphics->GetHeight ();
    ResizeUI (lastWidth_, lastHeight_);
    ready_ = true;
    listener_.Subscribe ();
    return true;
}

bool MenuUi::Update (float timeStep)
{
    Urho3D::Graphics *graphics = context_->GetSubsystem <Urho3D::Graphics> ();
    if (lastWidth_ != graphics->GetWidth () || lastHeight_ != graphics->GetHeight ())
    {
        lastWidth_ = graphics->GetWidth ();
        lastHeight_ = graphics->GetHeight ();
        ResizeUI (lastWidth_, lastHeight_);
    }
    UpdateUI (timeStep);
    return true;
}

bool MenuUi::Dispose ()
{
    if (ready_)
    {
        listener_.Unsubscribe ();
        HideUI ();
    }
    ready_ = false;
    return true;
}

MenuUi::~MenuUi ()
{
    Dispose ();
}
}
