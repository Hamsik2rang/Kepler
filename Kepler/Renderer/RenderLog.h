#pragma once

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
		inline float GetBackValue() const { return m_pValue[m_curSize]; }
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
		ImPlotLineData batchesCount; // Renderer�� ó���� batch�� ��(��Ʈ�� ����)
		ImPlotLineData trianglesCount; // ó���� �ﰢ�� ����(��Ʈ�� ����)
	};
}