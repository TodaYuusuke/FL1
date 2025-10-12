#pragma once
#include "math/Math.h"

class IAgent {
public:
	virtual ~IAgent() = default;

	virtual const LWP::Math::Vector3& GetPos() const = 0;
	virtual void MoveTowards(const LWP::Math::Vector3& target, float speed) = 0;

	virtual const float GetHealth() const = 0;
	virtual void Attack() = 0;
};