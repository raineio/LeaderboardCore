#pragma once

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

namespace LeaderboardCore::UI::Elements {
    UnityEngine::GameObject* CreateLeaderboardComponent(UnityEngine::Transform* parent);
}