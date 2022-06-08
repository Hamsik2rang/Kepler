#pragma once

#include "Renderer/GraphicsAPI.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

#include "Math/KeplerMath.h"

namespace kepler {

	class Renderer2D
	{
	private:
		static Renderer2D* s_pInstance;
		IGraphicsAPI* m_pGraphicsAPI;

		Renderer2D();
		~Renderer2D();

	public:
		static Renderer2D* Get();
		static void Init();
		inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }

		void BeginScene(OrthographicCamera& camera);
		void EndScene();
		void Flush();

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
	};
}