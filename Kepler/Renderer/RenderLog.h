#pragma once

#include "Core/Base.h"

namespace kepler {

	class ImPlotLineData
	{
	private:
		float* m_pKey;
		float* m_pValue;
		int m_curSize;

	public:
		static const int s_size = 1000;

		ImPlotLineData();
		~ImPlotLineData();
		void Add(const float value);
		inline float GetBackValue() const
		{ 
			KEPLER_CORE_ASSERT(m_curSize <= s_size, "ImPlot Size subcription out"); 
			return m_pValue[m_curSize - 1];
		}
		inline float* GetKey() const { return m_pKey; }
		inline float* GetValue() const { return m_pValue; }
		inline int GetCurSize() const { return m_curSize; }
	};

	// 프레임 동안 처리한 작업의 개수의 집합
	struct RenderLog
	{
		// 요구 작업
		ImPlotLineData drawCallsCount; // 호출된 Draw()의 수
		ImPlotLineData vertexCount; // 처리한 Vertex의 수(차트로 구현)
		ImPlotLineData transferredDataSize; // 전송 데이터 크기
		ImPlotLineData materialCount; // material 개수

		// TODO: 추가로 구현할 만한 것
		ImPlotLineData setPassCallsCount; // shader path를 전환한 수(차트로 구현)
		ImPlotLineData batchesCount; // Renderer가 처리한 batch의 수(차트로 구현)
		ImPlotLineData trianglesCount; // 처리한 삼각형 개수(차트로 구현)
	};
}