#pragma once

#include "Kepler.h"
#include "GameObject.h"
#include "Constant.h"

class Wall : public GameObject
{
private:
	kepler::Vec2f m_position;
	kepler::Vec2f m_size;
#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

public:
	Wall(const kepler::Vec2f& position, const kepler::Vec2f& size)
		: GameObject(eColliderType::Box, eColliderCategory::Wall)
		, m_position{ position }
		, m_size{ size }
	{}
#ifdef _DEUBG
	inline void OnRender() { kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_debugColor); }
	inline void OnUpdate(float deltaTime) { m_debugColor = { 0.0f, 1.0f, 0.0f, 1.0f }; }
	inline void OnCollision(CollisionData& data) { m_debugColor = { 0.0f, 1.0f, 0.0f, 1.0f }; }
#endif
	inline virtual kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return kepler::Vec2f(); }
	inline virtual kepler::Vec2f GetLastDirection() const override { return kepler::Vec2f(); }
};

class Net : public GameObject
{
private:
	kepler::Vec2f m_position;
	kepler::Vec2f m_size;
#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

public:
	Net(const kepler::Vec2f& position, const kepler::Vec2f& size)
		: GameObject(eColliderType::Box, eColliderCategory::Net)
		, m_position{ position }
		, m_size{ size }
	{}

#ifdef _DEUBG
	inline void OnRender() { kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_debugColor); }
	inline void OnUpdate(float deltaTime) { m_debugColor = { 0.0f, 1.0f, 0.0f, 1.0f }; }
	inline void OnCollision(CollisionData& data) { m_debugColor = { 0.0f, 1.0f, 0.0f, 1.0f }; }
#endif
	inline virtual kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return kepler::Vec2f(); }
	inline virtual kepler::Vec2f GetLastDirection() const override { return kepler::Vec2f(); }
};

class Ground : public GameObject
{
private:
	kepler::Vec2f m_position;
	kepler::Vec2f m_size;
#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

public:
	Ground(const kepler::Vec2f& position, const kepler::Vec2f& size)
		: GameObject(eColliderType::Box, eColliderCategory::Ground)
		, m_position{ position }
		, m_size{ size }
	{}

#ifdef _DEUBG
	inline void OnRender() { kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_debugColor); }
	inline void OnUpdate(float deltaTime) { m_debugColor = { 0.0f, 1.0f, 0.0f, 1.0f }; }
	inline void OnCollision(CollisionData& data) { m_debugColor = { 0.0f, 1.0f, 0.0f, 1.0f }; }
#endif
	inline virtual kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return kepler::Vec2f(); }
	inline virtual kepler::Vec2f GetLastDirection() const override { return kepler::Vec2f(); }
};


class Level
{
private:
	float m_width;
	float m_height;
	std::shared_ptr<kepler::ITexture2D> m_pLevelSprite;

	std::shared_ptr<Wall> m_pLeftWall;
	std::shared_ptr<Wall> m_pRightWall;
	std::shared_ptr<Net> m_pNet;
	std::shared_ptr<Ground> m_pGround;

public:
	void Init(float width, float height);
	void OnRender();
	void OnUpdate(float deltaTime);

	inline void SetWidth(float width) { m_width = width; }
	inline void SetHeight(float height) { m_height = height; }
	inline float GetWidth() const { return m_width; }
	inline float GetHeight() const { return m_height; }

};