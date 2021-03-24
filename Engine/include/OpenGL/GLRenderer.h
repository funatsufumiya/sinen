#pragma once
#include <SDL.h>
#include <string>
#include <GL/glew.h>
#include "ShaderGL.h"
#include <Engine/include/Window.hpp>
namespace nen
{
	class SpriteGL
	{
	public:
		SpriteGL() : trimStart(Vector2f(0.f, 1.f)), trimEnd(Vector2f(1.f, 0.f)) {}
		ShaderParameters param;
		std::string textureIndex;
		Vector2f trimStart;
		Vector2f trimEnd;
		int drawOrder = 100;
		bool isChangeBuffer = false;
	};
	class Renderer;

	namespace gl
	{
		struct VertexArrayForGL : public VertexArray
		{
			uint32_t vertexID;
			uint32_t indexID;
		};

		class GLRenderer
		{
		public:
			GLRenderer() : mRenderer(nullptr) {}
			void initialize(struct ::SDL_Window* window, ::SDL_GLContext context);
			void prepare();
			void cleanup() {}
			void render();
			void registerTexture(std::shared_ptr<Texture>, const TextureType& type);
			void pushSprite2d(std::shared_ptr<SpriteGL> sprite2d)
			{
				auto iter = mSprite2Ds.begin();
				for (;
					iter != mSprite2Ds.end();
					++iter)
				{
					if (sprite2d->drawOrder < (*iter)->drawOrder)
					{
						break;
					}
				}
				mSprite2Ds.insert(iter, sprite2d);
			}

			void pushSprite3d(std::shared_ptr<SpriteGL> sprite3d)
			{
				auto iter = mSprite3Ds.begin();
				for (;
					iter != mSprite3Ds.end();
					++iter)
				{
					if (sprite3d->drawOrder < (*iter)->drawOrder)
					{
						break;
					}
				}
				mSprite3Ds.insert(iter, sprite3d);
			}
			void setRenderer(nen::Renderer* renderer)
			{
				mRenderer = renderer;
			}

			uint32_t AddVertexArray(const VertexArrayForGL&);

		private:
			bool loadShader();
			void createSpriteVerts();
			void createBoxVerts();
			nen::Renderer* mRenderer;

			ShaderGL* mSpriteShader;
			ShaderGL* mAlphaShader;
			GLuint mTextureID;
			std::unordered_map<std::string, GLuint> mTextureIDs;
			std::unordered_map<uint32_t, VertexArrayForGL> m_VertexArrays;
			std::unordered_map<std::string, uint32_t> m_VertexArraysIndices;
			::SDL_Window* mWindow;
			::SDL_GLContext mContext;
			std::vector<std::shared_ptr<SpriteGL>> mSprite2Ds;
			std::vector<std::shared_ptr<SpriteGL>> mSprite3Ds;
		};

	}
}