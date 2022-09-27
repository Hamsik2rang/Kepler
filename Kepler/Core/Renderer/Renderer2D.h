#pragma once

#include "Core/Renderer/GraphicsAPI.h"
#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/RenderLog.h"

#include "KeplerMath.h"

namespace kepler {
	class Renderer2D
	{
	private:
		static Renderer2D* s_pInstance;
		IGraphicsAPI* m_pGraphicsAPI;
		RenderLog m_renderLog;

		Renderer2D();
		~Renderer2D();

	public:
		static Renderer2D* Get();
		static void Init();
		inline EGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }
		inline RenderLog& GetRenderLog() { return m_renderLog; }

		void BeginScene(OrthographicCamera& camera);
		void EndScene();
		void Flush();	// Batch Rendering�� �������� �ؽ�ó���� ��� �ѹ��� drawcall�ϴ� �Լ�

		// �Ķ���Ϳ� ���� �簢���� �׷��ִ� �Լ���
		void DrawQuad(const Vec2f& position, const Vec2f& size, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
		void DrawQuad(const Vec3f& position, const Vec2f& size, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
		void DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
		void DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });

		void DrawQuad(const Vec2f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFilpX = false, bool bFlipY = false, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
		void DrawQuad(const Vec3f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFilpX = false, bool bFlipY = false, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
		void DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFilpX = false, bool bFlipY = false, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
		void DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFilpX = false, bool bFlipY = false, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });

		void DrawQuad(const Mat44f& transform, const Vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawQuad(const Mat44f& transform, const std::shared_ptr<ITexture2D>& texture, bool bFlipX, bool bFlipY, const Vec4f& color);

		void DrawNonBatchedQuad(const Vec2f& position, const float rotation, const Vec2f& size, const Vec4f& color = { 1.0f,1.0f, 1.0f, 1.0f });
	};
}