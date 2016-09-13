#include "InitMainMenu.hpp"
#include "MenuUi.hpp"
#include <StatesEngine/StatesEngine.hpp>
#include <Urho3D/Resource/ResourceCache.h>
#include <GameEngine/Utils.hpp>

namespace FrontendCXX
{
void MainMenuFunctions::InitMainMenu (Urho3D::Context *context)
{
    GameEngine::RegisterType <MenuUi> (context);
    StatesEngine::StatesEngineSubsystem *statesEngine = context->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->DisposeAll ("any");
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->RemoveAll ("any");
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <MenuUi> ();
    context->GetSubsystem <Urho3D::ResourceCache> ()->ReleaseAllResources ();
}
}
