#include "rendering/Renderer.h"
#include "Resources.h"
#include "interface/Debug.h"
#include "interface/Display.h"
#include "interface/TimeClock.h"
#include "interface/Input.h"
#include "entities/Scene.h"
#include "interface/events/EventsManager.h"
#include "interface/Console.h"
#include "components/Transform.h"

#include <vendor/glm/glm.hpp>

int main(void)
{
    Debug::s_DebugLevel = Debug::Verbose;

    Display::Singleton();
    EventsManager::Singleton();
    Input::Singleton();
    TimeClock::Singleton();
    Resources::Singleton();
    Renderer::Singleton();
    Console::Singleton();

    Resources::Singleton()->GetScene("Scene1")->Load();
    bool adev = false;

    while (!Display::Singleton()->WindowShouldClose())
    {
        TimeClock::Singleton()->Update();
        Console::Singleton()->Update();
        Scene::GetCurrentScene()->Update();
        Scene::GetCurrentScene()->GetEntity("Entity1")->GetTransform()->IncreaseRotation(glm::vec3(0.f, 10.f * TimeClock::Singleton()->GetDeltaTime(), 0.f));
        //Scene::s_CurrentScene->GetEntity("Entity2")->GetTransform()->IncreaseRotation(10.f * TimeClock::GetDeltaTime());
        Renderer::Singleton()->Clear();
        Renderer::Singleton()->Draw();
        Display::Singleton()->Update();
    }


    Console::Destroy();
    Renderer::Destroy();
    Resources::Destroy();
    TimeClock::Destroy();
    Input::Destroy();
    EventsManager::Destroy();
    Display::Destroy();
    return 0;
}