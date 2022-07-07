#pragma once

#include <imgui.h>

#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {
	// 프레임 동안 처리한 작업의 개수의 집합
	struct RenderProfileData
	{
		// 요구 작업
		int drawCallsCount; // 호출된 Draw()의 수
		int vertexCount; // 처리한 Vertex의 수(차트로 구현)
		int transferredDataSize; // 전송 데이터 크기
		int materialCount; // material 개수

		// TODO: 추가로 구현할 만한 것
		int setPassCallsCount; // shader path를 전환한 수(차트로 구현)
		int batchesCount; // Renderer가 처리한 batch의 수(차트로 구현)
		int trianglesCount; // 처리한 삼각형 개수(차트로 구현)
	};

	class RenderProfiler : public Layer
	{
	private:
		const int CHART_SIZE = 600;

		RenderProfileData m_profile;
		
		std::unique_ptr<float[]> m_pDrawCallsCounts	{ new float[CHART_SIZE] { 0.0f, } };
		std::unique_ptr<float[]> m_pBatchesCounts	{ new float[CHART_SIZE] { 0.0f, } };
		std::unique_ptr<float[]> m_pVertexCounts	{ new float[CHART_SIZE] { 0.0f, } };
		std::unique_ptr<float[]> m_pTrianglesCounts	{ new float[CHART_SIZE] { 0.0f, } };

		// GUI Font
		ImFont* m_pFont;
	public:
		static RenderProfiler* s_pInstance;
		static void Create();
		static RenderProfiler* Get();

		virtual void OnUpdate(float deltaTime) override;

		// GUI Function
		virtual void OnAttach() override;
		virtual void OnGUIRender() override;

		inline void SetProfile(const RenderProfileData& profile) { m_profile = profile; }
	};
}