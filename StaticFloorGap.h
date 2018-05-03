#pragma once
#include "Pickup.h"	// inherit from pickup as it is so similar 

// Created so we can lower the ground height on collision to head to lower levels
class StaticFloorGap :
	public Pickup
{
public:
	using Pickup::Pickup;
	bool isGap();
};

