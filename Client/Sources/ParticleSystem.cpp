#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(uint32_t particlePoolSize)
	: m_poolIndex{ 0 }
	, m_particlePool{ particlePoolSize }
{

}

void ParticleSystem::Emit(
	kepler::Vec2f position,
	kepler::Vec2f velocity,
	kepler::Vec2f velocityAdd,
	kepler::Vec4f colorBegin,
	kepler::Vec4f colorEnd,
	float rotation,
	float sizeBegin,
	float sizeEnd,
	float lifeTime)
{
	auto& particle = m_particlePool[m_poolIndex];
	if (particle.bIsActived)
	{
		return;
	}

	particle.position = position;
	particle.velocity = velocity * 0.1f;
	particle.velocityAdd = velocityAdd * 0.1f;
	particle.colorBegin = colorBegin;
	particle.colorEnd = colorEnd;
	particle.rotation = rotation;
	particle.sizeBegin = sizeBegin * kepler::Random::Float(0.5f, 1.0f);
	particle.sizeEnd = sizeEnd;
	particle.lifeTime = lifeTime;
	particle.lifeRemaining = lifeTime;
	particle.bIsActived = true;

	m_poolIndex = (m_poolIndex + 1) % m_particlePool.size();
}

void ParticleSystem::OnUpdate(const float deltaTime)
{
	for (auto& e : m_particlePool)
	{
		e.lifeRemaining -= deltaTime;
		if (e.lifeRemaining < 0.0f)
		{
			e.bIsActived = false;
			continue;
		}

		e.velocity += e.velocityAdd * deltaTime;
		e.position += e.velocity * deltaTime;
	}
}

void ParticleSystem::OnRender()
{
	for (const auto& e : m_particlePool)
	{
		if (!e.bIsActived)
		{
			continue;
		}
		float t = e.lifeRemaining / e.lifeTime;
		float size = (e.sizeBegin * t) + (e.sizeEnd * (1.0f - t));

		kepler::Vec4f color = kepler::math::Lerp(e.colorEnd, e.colorBegin, t);
		kepler::Renderer2D::Get()->DrawQuad(e.position, e.rotation, { size,size }, color);
	}
}
