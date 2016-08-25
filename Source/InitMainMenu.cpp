#include "InitMainMenu.hpp"
#include "MenuUi.hpp"
#include <StatesEngine/StatesEngine.hpp>
#include <Urho3D/Resource/ResourceCache.h>
#include "Utils.hpp"

void MainMenuFunctions::InitMainMenu(Urho3D::Context *context)
{
    RegisterType <MenuUi> (context);
    StatesEngine::StatesEngine *statesEngine = context->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->DisposeAll ("any");
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->RemoveAll ("any");
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <MenuUi> ();
    context->GetSubsystem <Urho3D::ResourceCache> ()->ReleaseAllResources ();
}
