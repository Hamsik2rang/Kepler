#pragma once
#pragma once

#include <memory>

#include "Kepler.h"
#include "RayTracer.h"

class ResearchLayer : public kepler::Layer
{
private:
    kepler::OrthographicCamera m_camera;
    kepler::Entity* m_pEntity;
    kepler::Scene m_scene;

    kepler::Vec4f m_circleColor;
    kepler::Vec3f m_circleCenterPos;
    float m_circleRadius;

    RayTracer m_rayTracer;
public:
    ResearchLayer()
        : Layer("Research"),
          m_pEntity{nullptr},
          m_circleColor{0.0f, 128.0f, 255.0f, 255.0f},
          m_circleCenterPos{800.0f, 450.0f, 0.0f},
          m_circleRadius{50.0f}
    {
    }

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(kepler::Event& e) override;
    virtual void OnRender() override;
    virtual void OnGUIRender() override;
};