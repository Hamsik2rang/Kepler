#pragma once

#include <imgui.h>

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