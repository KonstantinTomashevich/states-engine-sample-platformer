#include "IngameHud.hpp"
#include <StatesEngine/StatesEngine.hpp>

#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/IO/Log.h>

void IngameHud::InitUI ()
{
    Urho3D::UI *ui = context_->GetSubsystem <Urho3D::UI> ();
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    ui->Clear ();

    screenBackground_ = ui->GetRoot ()->CreateChild <Urho3D::Button> ("screenBackground");
    screenBackground_->SetColor (Urho3D::Color (0, 0, 0, 0));

    coinIcon_ = screenBackground_->CreateChild <Urho3D::Sprite> ("coinIcon");
    coinIcon_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/coin.png"));

    coinsText_ = screenBackground_->CreateChild <Urho3D::Text> ("coinsText");
    coinsText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    coinsText_->SetTextEffect (Urho3D::TE_STROKE);
    coinsText_->SetEffectStrokeThickness (3);
    coinsText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));

    livesIcon_ = screenBackground_->CreateChild <Urho3D::Sprite> ("livesIcon");
    livesIcon_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/heart.png"));

    livesText_ = screenBackground_->CreateChild <Urho3D::Text> ("livesText");
    livesText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    livesText_->SetTextEffect (Urho3D::TE_STROKE);
    livesText_->SetEffectStrokeThickness (3);
    livesText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));

    pauseButton_ = screenBackground_->CreateChild <Urho3D::Button> ("pauseButton");
    pauseButton_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    pauseButtonText_ = pauseButton_->CreateChild <Urho3D::Text> ("text");

    pauseButtonText_->SetText ("Pause");
    pauseButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    pauseButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    pauseButtonText_->SetEffectStrokeThickness (2);
    pauseButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    pauseButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    attackButton_ = screenBackground_->CreateChild <Urho3D::Button> ("attackButton");
    attackButton_->SetColor (Urho3D::Color (1, 1, 1, 0.6f));
    attackButton_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/attack.png"));

    blockButton_ = screenBackground_->CreateChild <Urho3D::Button> ("blockButton");
    blockButton_->SetColor (Urho3D::Color (1, 1, 1, 0.6f));
    blockButton_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/block.png"));

    joystick_ = screenBackground_->CreateChild <Urho3D::UIElement> ("joystick");
    joystickBackground_ = joystick_->CreateChild <Urho3D::Sprite> ("background");
    joystickBackground_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/window_background.png"));
    joystickBackground_->SetBlendMode (Urho3D::BLEND_ALPHA);

    joystickKnob_ = joystick_->CreateChild <Urho3D::Button> ("joystickKnob");
    joystickKnob_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/joystick_knob.png"));

    pauseShadow_ = screenBackground_->CreateChild <Urho3D::Sprite> ("pauseShadow");
    pauseShadow_->SetColor (Urho3D::Color (0, 0, 0, 0.5f));
    pauseWindow_ = screenBackground_->CreateChild <Urho3D::UIElement> ("pauseWindow");
    pauseWindowBackground_ = pauseWindow_->CreateChild <Urho3D::Sprite> ();
    pauseWindowBackground_->SetBlendMode (Urho3D::BLEND_ALPHA);
    pauseWindowBackground_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/window_background.png"));

    resumeButton_ = pauseWindow_->CreateChild <Urho3D::Button> ("resumeButton");
    resumeButton_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    resumeButtonText_ = resumeButton_->CreateChild <Urho3D::Text> ("text");

    resumeButtonText_->SetText ("Resume");
    resumeButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    resumeButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    resumeButtonText_->SetEffectStrokeThickness (2);
    resumeButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    resumeButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    menuButton_ = pauseWindow_->CreateChild <Urho3D::Button> ("menuButton");
    menuButton_->SetTexture (cache->GetResource <Urho3D::Texture2D> ("Textures/ui/button_background.png"));
    menuButtonText_ = menuButton_->CreateChild <Urho3D::Text> ("text");

    menuButtonText_->SetText ("Menu");
    menuButtonText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    menuButtonText_->SetTextEffect (Urho3D::TE_STROKE);
    menuButtonText_->SetEffectStrokeThickness (2);
    menuButtonText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    menuButtonText_->SetAlignment (Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    levelFailedText_ = pauseWindow_->CreateChild <Urho3D::Text> ("levelFailed");
    levelFailedText_->SetText ("You lose!");
    levelFailedText_->SetFont (cache->GetResource <Urho3D::Font> ("Fonts/Anonymous Pro.ttf"));
    levelFailedText_->SetTextEffect (Urho3D::TE_STROKE);
    levelFailedText_->SetEffectStrokeThickness (2);
    levelFailedText_->SetEffectColor (Urho3D::Color (0, 0, 0, 1));
    SetPauseWindowVisible (false);
}

void IngameHud::ResizeUI (int width, int height)
{
    screenBackground_->SetPosition (0, 0);
    screenBackground_->SetSize (width, height);

    coinIcon_->SetSize (height * 0.15f, height * 0.15f);
    coinIcon_->SetPosition (height * 0.035f, height * 0.035f);

    coinsText_->SetPosition (height * 0.22f, height * 0.035f);
    coinsText_->SetFont (coinsText_->GetFont (), height * 0.12f);

    livesIcon_->SetSize (height * 0.15f, height * 0.15f);
    livesIcon_->SetPosition (height * 0.035f, height * 0.22f);

    livesText_->SetPosition (height * 0.22f, height * 0.22f);
    livesText_->SetFont (livesText_->GetFont (), height * 0.12f);

    pauseButton_->SetSize (height * 0.45f, height * 0.15f);
    pauseButton_->SetPosition (width - height * 0.485f, height * 0.035f);
    pauseButtonText_->SetFont (pauseButtonText_->GetFont (), height * 0.09f);

    attackButton_->SetSize (height * 0.2f, height * 0.2f);
    attackButton_->SetPosition (height * 0.035f, height - height * 0.235f);

    blockButton_->SetSize (height * 0.2f, height * 0.2f);
    blockButton_->SetPosition (height * 0.27f, height - height * 0.235f);

    joystick_->SetSize (height * 0.3f, height * 0.3f);
    joystick_->SetPosition (width - height * 0.335f, height - height * 0.335f);
    joystickBackground_->SetSize (height * 0.3f, height * 0.3f);

    joystickKnob_->SetSize (height * 0.1f, height * 0.1f);
    joystickKnob_->SetPosition (height * 0.1f, height * 0.1f);

    pauseShadow_->SetSize (width, height);
    pauseWindow_->SetSize (height * 0.75f, height * 0.75f);
    pauseWindow_->SetPosition (width / 2.0f - height * 0.375f, height * 0.125f);
    pauseWindowBackground_->SetSize (height * 0.75f, height * 0.75f);

    resumeButton_->SetSize (pauseWindow_->GetWidth () * 0.75f, pauseWindow_->GetHeight () * 0.25f);
    resumeButton_->SetPosition (pauseWindow_->GetWidth () * 0.125f, pauseWindow_->GetHeight () * 0.125f);
    resumeButtonText_->SetFont (resumeButtonText_->GetFont (), height * 0.09f);

    menuButton_->SetSize (pauseWindow_->GetWidth () * 0.75f, pauseWindow_->GetHeight () * 0.25f);
    menuButton_->SetPosition (pauseWindow_->GetWidth () * 0.125f, pauseWindow_->GetHeight () * 0.5f);
    menuButtonText_->SetFont (menuButtonText_->GetFont (), height * 0.09f);

    levelFailedText_->SetFont (levelFailedText_->GetFont (), height * 0.09f);
    levelFailedText_->SetPosition (pauseWindow_->GetWidth () * 0.125f, pauseWindow_->GetHeight () * 0.15f);
}

void IngameHud::UpdateUI (float timeStep)
{
    coinsText_->SetText (Urho3D::String (playerController_->GetCoins ()));
    livesText_->SetText (Urho3D::String ((int) playerController_->GetLives ()));

    if (playerController_->GetLives () < 0.0f)
    {
        SetPauseWindowVisible (true);
        resumeButton_->SetVisible (false);
        levelFailedText_->SetVisible (true);
    }

    if (showTouchControls_)
    {
        attackButton_->SetVisible (pauseButton_->IsVisible ());
        blockButton_->SetVisible (pauseButton_->IsVisible ());
        joystick_->SetVisible (pauseButton_->IsVisible ());

        Urho3D::Vector2 joystickCoordinates_;
        joystickCoordinates_.x_ = ((joystickKnob_->GetPosition ().x_ +
                joystickKnob_->GetWidth () / 2.0f) - joystick_->GetWidth () / 2.0f) *
                2.0f / joystick_->GetWidth ();

        joystickCoordinates_.y_ = ((joystickKnob_->GetPosition ().y_ +
                joystickKnob_->GetHeight () / 2.0f) - joystick_->GetHeight () / 2.0f) *
                2.0f / joystick_->GetHeight ();
        playerController_->SetControlls (attackButton_->IsPressed (),
                                         blockButton_->IsPressed (), joystickCoordinates_);
    }
    else
    {
        attackButton_->SetVisible (false);
        blockButton_->SetVisible (false);
        joystick_->SetVisible (false);
    }
}

void IngameHud::HideUI ()
{
    screenBackground_->RemoveAllChildren ();
}

IngameHud::IngameHud (Urho3D::Context *context) : StateObject (context), listener_ (this)
{
    lastWidth_ = 0;
    lastHeight_ = 0;
    showTouchControls_ = true;
    playerController_ = 0;

    screenBackground_ = 0;
    coinIcon_ = 0;
    coinsText_ = 0;
    livesIcon_ = 0;
    livesText_ = 0;

    pauseButton_ = 0;
    pauseButtonText_ = 0;
    pauseShadow_ = 0;
    pauseWindow_ = 0;
    pauseWindowBackground_ = 0;
    resumeButton_ = 0;
    resumeButtonText_ = 0;
    menuButton_ = 0;
    menuButtonText_ = 0;

    attackButton_= 0;
    blockButton_= 0;
    joystick_= 0;
    joystickBackground_= 0;
    joystickKnob_= 0;
}

bool IngameHud::Init ()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    if (!playerController_)
        playerController_ = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <IngamePlayerController> ();

    if (playerController_)
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
    else
    {
        ready_ = false;
        return false;
    }
}

bool IngameHud::Update (float timeStep)
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

bool IngameHud::Dispose ()
{
    if (ready_)
    {
        listener_.Unsubscribe ();
        HideUI ();
    }
    ready_ = false;
    return true;
}

IngamePlayerController *IngameHud::GetPlayerController()
{
    return playerController_;
}

void IngameHud::SetPlayerController(IngamePlayerController *playerController)
{
    playerController_ = playerController;
}

void IngameHud::SetPauseWindowVisible (bool isEnabled)
{
    pauseWindow_->SetVisible (isEnabled);
    pauseShadow_->SetVisible (isEnabled);
    pauseButton_->SetVisible (!isEnabled);
    levelFailedText_->SetVisible (false);
}

bool IngameHud::IsSnowTouchControls ()
{
    return showTouchControls_;
}

void IngameHud::SetIsShowTouchControls (bool showTouchControls)
{
    showTouchControls_ = showTouchControls;
}

IngameHud::~IngameHud ()
{
    Dispose ();
}

