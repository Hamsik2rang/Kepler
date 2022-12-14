#pragma once

#include <vector>

#include "Kepler.h"

class ParticleSystem
{
private:
    struct Particle
    {
        kepler::Vec2f position;
        kepler::Vec2f velocity;
        kepler::Vec2f velocityAdd;
        kepler::Vec4f colorBegin;
        kepler::Vec4f colorEnd;
        float rotation;
        float sizeBegin;
        float sizeEnd;

        float lifeTime = 1.0f;
        float lifeRemaining = 1.0f;

        bool bIsActived = false;
    };

    std::vector<Particle> m_particlePool;
    uint32_t m_poolIndex;

public:
    ParticleSystem(uint32_t particlePoolSize = 1000);

    void Emit(
        kepler::Vec2f position,
        kepler::Vec2f velocity,
        kepler::Vec2f velocityAdd,
        kepler::Vec4f colorBegin,
        kepler::Vec4f colorEnd,
        float rotation,
        float sizeBegin,
        float sizeEnd,
        float lifeTime = 1.0f);

    void OnUpdate(const float deltaTime);
    void OnRender(bool bBatching);
};