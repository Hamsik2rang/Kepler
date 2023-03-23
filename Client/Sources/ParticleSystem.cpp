#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(uint32_t particlePoolSize)
    : m_poolIndex{0},
      m_particlePool{particlePoolSize}
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

    particle.position = position;
    particle.velocity = velocity * 0.1f;
    particle.velocityAdd = velocityAdd * 0.1f;
    particle.colorBegin = colorBegin;
    particle.colorEnd = colorEnd;
    particle.rotation = rotation;
    particle.sizeBegin = sizeBegin * kepler::Random::Float(0.1f, 1.0f);
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

void ParticleSystem::OnRender(bool bBatching)
{
    for (const auto& e : m_particlePool)
    {
        if (!e.bIsActived)
        {
            continue;
        }
        float t = e.lifeRemaining / e.lifeTime;
        float size = (e.sizeBegin * t) + (e.sizeEnd * (1.0f - t));
        float rotation = (e.rotation * t) + ((e.rotation + 90.0f) * (1.0f - t));

        kepler::Vec4f color = kepler::math::Lerp(e.colorEnd, e.colorBegin, t);
        if (bBatching)
        {
            kepler::Renderer2D::Get()->DrawQuad(e.position, rotation, {size, size}, color);
        }
        else
        {
            kepler::Renderer2D::Get()->DrawNonBatchedQuad(e.position, rotation, {size, size}, color);
        }
    }
}
