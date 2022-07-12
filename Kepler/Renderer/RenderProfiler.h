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
		std::unique_ptr<float[]> x_data { new float[size] { 0.0f, } };
		std::unique_ptr<float[]> y_data { new float[size] { 0.0f, } };
	};

	class RenderProfiler : public Layer
	{
	private:
		RenderProfileData m_profile;
		
		ImPlotLineData m_drawCallsData;
		ImPlotLineData m_batchesData;
		ImPlotLineData m_vertexData;
		ImPlotLineData m_trianglesData;

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