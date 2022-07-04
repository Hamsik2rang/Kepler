#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {
	// ������ ���� ó���� �۾��� ������ ����
	struct RenderProfile
	{
		// �䱸 �۾�
		int drawCallsCount; // ȣ��� Draw()�� ��
		int vertexCount; // ó���� Vertex�� ��
		int transferredDataSize; // ���� ������ ũ��
		int materialCount; // material ����
		float frameRate; // frame �ӵ�

		// TODO: �߰��� ������ ���� ��
		int setPassCallsCount; // shader path�� ��ȯ�� ��
		int batchesCount; // Renderer�� ó���� batch�� ��
		int trianglesCount; // ó���� �ﰢ�� ����
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

		// ImGUI ������ ���� �Լ����Դϴ�.
		virtual void Begin() override;
		virtual void End() override;
		virtual void OnGUIRender() override;

	};
}