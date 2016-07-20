#pragma once
#include <memory>
#include <list>
#include "Grid.h"
class IUpdatableObject
{
	virtual void Update(float dT) = 0;
};