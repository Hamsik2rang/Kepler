#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {
	// 프레임 동안 처리한 작업의 개수의 집합
	struct RenderProfile
	{
		// 요구 작업
		int drawCallsCount; // 호출된 Draw()의 수
		int vertexCount; // 처리한 Vertex의 수
		int transferredDataSize; // 전송 데이터 크기
		int materialCount; // material 개수
		float frameRate; // frame 속도

		// TODO: 추가로 구현할 만한 것
		int setPassCallsCount; // shader path를 전환한 수
		int batchesCount; // Renderer가 처리한 batch의 수
		int trianglesCount; // 처리한 삼각형 개수
	};

	class RenderProfiler : public Layer
	{
	private:
		static RenderProfile m_profile;

	public:
		static RenderProfiler* s_pInstance;
		static void Create();
		static RenderProfiler* Get();

		virtual void OnUpdate(float deltaTime) override;

		// ImGUI 구성을 위한 함수들입니다.
		virtual void Begin() override;
		virtual void End() override;
		virtual void OnGUIRender() override;

	};
}