#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"

class CGameObject
{
protected:
	float x;
	float y;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y = 0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	~CGameObject();
};
typedef CGameObject* LPGAMEOBJECT;

class CMoveableObject : public CGameObject
{
protected:
	float vx;
	float vy;
public:
	CMoveableObject(float x, float y, float vx, float vy, LPTEXTURE texture) : CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
	}
	void SetVelocity(float vx, float vy) { this->vx = vx, this->vy = vy; }
	float GetVx() { return vx; }
	float GetVy() { return vy; }
	void SetVx(float vx) { this->vx = vx; }
	void SetVy(float vy) { this->vy = vy; }
};

class CBullet : public CMoveableObject
{
protected:
	bool isActive;
public:
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture) : CMoveableObject(x, y, vx, vy, texture)
	{
		this->isActive = false;
	}
	bool GetIsActive() { return isActive; }
	void Update(DWORD dt);
	void Render();
	void SetIsActive(bool isActive) { this->isActive = isActive; }
};

class CSpaceShip : public CMoveableObject
{
protected:
	CBullet* bullet;
public:
	CSpaceShip(float x, float y, float vx, float vy, LPTEXTURE texture,
		float bl_x, float bl_y, float bl_vx, float bl_vy, LPTEXTURE bl_texture) : CMoveableObject(x, y, vx, vy, texture)
	{
		this->bullet = new CBullet(bl_x, bl_y, bl_vx, bl_vy, bl_texture);
	};
	CBullet* GetBullet() { return bullet; }
	void HandleShooting(DWORD dt);
	void Render();
	void Update(DWORD dt);
};

class CMonster : public CMoveableObject
{
protected:
	bool isAlive;
public:
	CMonster(float x, float y, float vx, float vy, LPTEXTURE texture) : CMoveableObject(x, y, vx, vy, texture)
	{
		this->isAlive = true;
	};
	void Render();
	bool GetIsAlive() { return isAlive; }
	void CheckCollision(CBullet*);
	void Update(DWORD dt);
	void Revive();
};