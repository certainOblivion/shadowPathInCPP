#include "pch.h"
#include "Enemy.h"
#include "Library\MathHelper.h"

Enemy::~Enemy()
{
}

void Enemy::Update(float dT)
{
	if (mPath.get() != nullptr && mPath->size() > 0)
	{
		if (Position() == mPath->front())
		{
			mPath->pop_front();
			if (mPath->size() == 0)
			{
				return;
			}
		}

		Vector2D& newPosition = Helper::MathHelper::MoveTowards(Position(), mPath->front(), 400*dT);
		SetPosition(newPosition);
	}
}

Enemy::Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc) 
	: IRenderableObject(position, rotation, dimensions, drawFunc) 
{}

void Enemy::SetPath(std::shared_ptr<std::list<Vector2D>> newPath)
{
	mPath = newPath;
}
