#pragma once

#include "Renderer/GraphicsAPI.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Texture.h"

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
		
		void DrawQuad(const Vec2f& position, const Vec2f& size, const Vec4f& color);
		void DrawQuad(const Vec3f& position, const Vec2f& size, const Vec4f& color);
		void DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const Vec4f& color);
		void DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const Vec4f& color);
		
		void DrawQuad(const Mat44f& transform, const Vec4f& color);
		void DrawQuad(const Mat44f& transform, std::shared_ptr<ITexture2D>& texture, const Vec4f& color);
	};

}