#pragma once

#include <imgui.h>
#include <implot.h>

#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {
	// ������ ���� ó���� �۾��� ������ ����
	struct RenderProfileData
	{
		// �䱸 �۾�
		int drawCallsCount; // ȣ��� Draw()�� ��
		int vertexCount; // ó���� Vertex�� ��(��Ʈ�� ����)
		int transferredDataSize; // ���� ������ ũ��
		int materialCount; // material ����

		// TODO: �߰��� ������ ���� ��
		int setPassCallsCount; // shader path�� ��ȯ�� ��(��Ʈ�� ����)
		int batchesCount; // Renderer�� ó���� batch�� ��(��Ʈ�� ����)
		int trianglesCount; // ó���� �ﰢ�� ����(��Ʈ�� ����)
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