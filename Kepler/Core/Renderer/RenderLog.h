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

	// ������ ���� ó���� �۾��� ������ ����
	struct RenderLog
	{
		// �䱸 �۾�
		ImPlotLineData drawCallsCount; // ȣ��� Draw()�� ��
		ImPlotLineData vertexCount; // ó���� Vertex�� ��(��Ʈ�� ����)
		ImPlotLineData transferredDataSize; // ���� ������ ũ��
		ImPlotLineData materialCount; // material ����

		// TODO: �߰��� ������ ���� ��
		ImPlotLineData setPassCallsCount; // shader path�� ��ȯ�� ��(��Ʈ�� ����)
		ImPlotLineData trianglesCount; // ó���� �ﰢ�� ����(��Ʈ�� ����)
	};
}