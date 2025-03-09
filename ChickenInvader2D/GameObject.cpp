#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include <cstdlib>
#include <ctime>

/*
	Initialize game object
*/
//extern CSpaceShip* spaceship;

CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

//void CGameObject::Render()
//{
//	//CGame::GetInstance()->Draw(x, y, texture);
//}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

#define BULLET_SPEED 0.2f
#define SPACESHIP_SPEED 0.2f
#define MONSTER_SPEED 0.2f

#define SPACESHIP_WIDTH 20
#define SPACESHIP_HEIGHT 20
#define MONSTER_SPACE 20

void CBullet::Update(DWORD dt)
{
	if (isActive)
	{
		vy = -BULLET_SPEED;
		y += vy * dt;
	}
}

void CBullet::Render()
{
	if (isActive)
	{
		CGame::GetInstance()->Draw(x, y, texture);
	}
}

void CSpaceShip::Update(DWORD dt)
{
	// Reset velocity
	vx = 0;
	vy = 0;
	// Check for key presses and update velocity accordingly
	if (GetAsyncKeyState('W') & 0x8000) {
		vy = -SPACESHIP_SPEED;  // Move up
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		vy = SPACESHIP_SPEED;   // Move down
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		vx = -SPACESHIP_SPEED;  // Move left
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		vx = SPACESHIP_SPEED;   // Move right
	}
	// Update spaceship's position based on velocity    
	x += vx * dt;
	y += vy * dt;
	// Get the back buffer width and height for boundary checking
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();
	// Boundary checking for x-axis
	if (x <= SPACESHIP_WIDTH / 2) {
		x = SPACESHIP_WIDTH / 2;
	}
	else if (x >= BackBufferWidth - SPACESHIP_WIDTH / 2) {
		x = (float)(BackBufferWidth - SPACESHIP_WIDTH / 2);
	}
	// Boundary checking for y-axis
	if (y <= SPACESHIP_HEIGHT / 2) {
		y = SPACESHIP_HEIGHT / 2;
	}
	else if (y >= BackBufferHeight - SPACESHIP_HEIGHT / 2) {
		y = (float)(BackBufferHeight - SPACESHIP_HEIGHT / 2);
	}
	// Handle shooting 
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		HandleShooting(dt);
	}

	if (bullet->GetIsActive())
	{
		if (bullet->GetY() < 0)
		{
			bullet->SetIsActive(false);
		}
	}

	bullet->Update(dt);
}

void CSpaceShip::HandleShooting(DWORD dt) {
	if (!bullet->GetIsActive()) {
		bullet->SetPosition(x, y);
		bullet->SetVelocity(0, -BULLET_SPEED);
		bullet->SetIsActive(true);
	}
}

void CSpaceShip::Render() {
	CGame::GetInstance()->Draw(x, y, texture);
	bullet->Render();
}

void CMonster::Render() {
	if (isAlive) CGame::GetInstance()->Draw(x, y, texture);
}

void CMonster::Revive() {
	if (!isAlive) {
		isAlive = true;
		int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
		x = rand() % BackBufferWidth;
	}
}

void CMonster::Update(DWORD dt)
{
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	// Ensure vx is not reset every frame
	if (vx == 0) {
		vx = MONSTER_SPEED;  // Initialize movement direction once
		vy = 0;
	}

	y = MONSTER_SPACE;		// Set y coord that allow monster moving around that space.
	
	x += vx * dt;			// update horizontal direction with velocity

	// Reverse direction when hitting screen edges
	if (x <= 0 || x >= BackBufferWidth) {
		vx = -vx;  // Reverse direction

		// Clamp `x` within screen bounds
		if (x <= 0) {
			x = 0;
		}
		else if (x >= BackBufferWidth) {
			x = (float)(BackBufferWidth);
		}
	}

	CSpaceShip* spaceship = CGame::GetInstance()->GetSpaceShip();
	CheckCollision(spaceship->GetBullet());
	Revive(); // once the chicken is hit by a bullet, it is revived at random coord on the screen
}

void CMonster::CheckCollision(CBullet* bullet)
{
	float width = texture->getWidth();
	float height = texture->getHeight();
	if ((bullet->GetX() >= x - width / 2 && bullet->GetX() <= x + width / 2)
		&& (bullet->GetY() >= y - height / 2 && bullet->GetY() <= y + height / 2))
	{
		isAlive = false;
		x = y = vx = vy = 0;
	}
}