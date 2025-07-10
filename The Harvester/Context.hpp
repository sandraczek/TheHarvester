#pragma once
#include "EntityManager.hpp"
#include "ComponentStorage.hpp"
#include "Resources.hpp"

// graphics + audio

struct Context {
	Resources resources;
	bool debugMode = false;
	EntityManager entities;
	ComponentStorage components;
};