#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>

#include <StatesEngine/StatesEngine.hpp>
#include <StatesEngine/StateObjectsManager.hpp>
#include <StatesEngine/SceneContainer.hpp>
#include <FrontendCXX/InitMainMenu.hpp>
#include "Urho3DApplication.hpp"

Urho3DApplication::Urho3DApplication (Urho3D::Context *context) : Urho3D::Application(context)
{
    isDrawDebug_ = false;
}

void Urho3DApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void Urho3DApplication::Start ()
{
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseMode (Urho3D::MM_FREE);
    input->SetMouseVisible (true);
    context_->RegisterSubsystem (this);
    input->SetTouchEmulation (true);

    StatesEngine::StatesEngineSubsystem *statesEngine = new StatesEngine::StatesEngineSubsystem (context_);
    statesEngine->Init ();
    context_->RegisterSubsystem (statesEngine);
    statesEngine->SetupState (Urho3D::SharedPtr <StatesEngine::StateObjectsManager> (new StatesEngine::StateObjectsManager (context_)));

    FrontendCXX::MainMenuFunctions::InitMainMenu (context_);
    SubscribeToEvent (Urho3D::E_POSTRENDERUPDATE, URHO3D_HANDLER (Urho3DApplication, DrawDebug));
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());

    // *** If you want see state objects manager log, you will uncomment it.
    // statesEngine->GetState <StatesEngine::StateObjectsManager> ()->SetIsUseLog (true);
    // context_->GetSubsystem <Urho3D::Log> ()->SetLevel (Urho3D::LOG_DEBUG);
    // ***
}

void Urho3DApplication::Stop ()
{
    StatesEngine::StatesEngineSubsystem *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    statesEngine->DisposeState ();
    context_->RemoveSubsystem ("StatesEngine");
}

bool Urho3DApplication::IsDrawDebug ()
{
    return isDrawDebug_;
}

void Urho3DApplication::SetIsDrawDebug (bool isDrawDebug)
{
    isDrawDebug_ = isDrawDebug;
}

void Urho3DApplication::DrawDebug (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    StatesEngine::StatesEngineSubsystem *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    if (isDrawDebug_ && statesEngine && statesEngine->HasState () &&
            statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> () &&
            statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> ()->Get ())
    {
        StatesEngine::SceneContainer *scene = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> ();
        if (scene->Get ()->HasComponent <Urho3D::PhysicsWorld2D> ())
            scene->Get ()->GetComponent <Urho3D::PhysicsWorld2D> ()->DrawDebugGeometry ();
    }
}

Urho3DApplication::~Urho3DApplication ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (Urho3DApplication)
