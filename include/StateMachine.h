#pragma once

#include "Timer.h"
#include "Types.h"

#include "../vendor/entt/entt.hpp"

void UnitStateMachine(entt::registry& registry, entt::entity entity, Event event);