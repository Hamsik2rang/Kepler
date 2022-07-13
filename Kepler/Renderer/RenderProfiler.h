#pragma once

#include <imgui.h>
#include <implot.h>

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

	struct ImPlotLineData
	{
	public:
		static const int size = 1000;
		std::unique_ptr<float[]> x_data;
		std::unique_ptr<float[]> y_data;

		ImPlotLineData()
		{
			x_data = std::make_unique<float[]>(size);
			y_data = std::make_unique<float[]>(size);
			for (int i = 0; i < size; i++)
			{
				x_data[i] = 0;
				y_data[i] = 0;
			}
		}
	};

	class RenderProfiler : public Layer
	{
	private:
		RenderProfileData m_profile;
		
		ImPlotLineData m_drawCallsData;
		ImPlotLineData m_batchesData;
		ImPlotLineData m_vertexData;
		ImPlotLineData m_trianglesData;

	public:
		virtual void OnUpdate(float deltaTime) override;

		// GUI Function
		virtual void OnGUIRender() override;

		inline void SetProfile(const RenderProfileData& profile) { m_profile = profile; }
	};
}