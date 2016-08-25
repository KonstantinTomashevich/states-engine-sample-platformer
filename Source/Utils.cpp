#include "Utils.hpp"
#include <StatesEngine/StatesEngine.hpp>

Urho3D::Node *FindNode (Urho3D::Context *context, Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    StatesEngine::StatesEngine *statesEngine = context->GetSubsystem <StatesEngine::StatesEngine> ();
    StatesEngine::SceneContainer *container = scene;
    if (!container && statesEngine->HasState ())
        container = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Get <StatesEngine::SceneContainer> ();

    if (container)
        return container->GetNode (wayToNode);
}
