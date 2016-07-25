#pragma once
#include "Library\IObject.h"
class Enemy : public Object::IRenderableObject, public Object::IUpdatableObject
{
public:
	Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc);
	virtual ~Enemy() = default;
	void SetPath(std::shared_ptr<std::list<Vector2D>> newPath);
	virtual void Update(float dT) override;
private:
	std::shared_ptr<std::list<Vector2D>> mPath;
};

