#include "include/UIElements/LoadingControl.hpp"
#include "UnityEngine/Resources.hpp"

namespace LeaderboardCore::UI::Elements {
    UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition)
    {
        auto loadingIndicator = UnityEngine::GameObject::Instantiate(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::GameObject*>().First([](UnityEngine::GameObject* x){ return x->get_name() == "LoadingIndicator"; }), parent, false);
        static ConstString name("LeaderboardCoreLoadingIndicator");
        loadingIndicator->set_name(name);
        loadingIndicator->AddComponent<UnityEngine::UI::LayoutElement*>();
        UnityEngine::RectTransform* rectTransform = loadingIndicator->GetComponent<UnityEngine::RectTransform*>();
        rectTransform->set_anchoredPosition(anchoredPosition);
        return loadingIndicator;
    }
}