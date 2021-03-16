#include <Components.hpp>
#include <Actors.hpp>
#include <Engine.hpp>
#include <Scene.hpp>
#include <iostream>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>

Sprite3DComponent::Sprite3DComponent(Actor &owner, const int drawOrder, Texture tex)
	: Component(owner), mOwner(owner), mDrawOrder(drawOrder)
{
}

void Sprite3DComponent::Update(float deltaTime)
{
	if (mOwner.isRecompute())
	{
		if (!mTexture)
		{

			auto tex = std::make_shared<Texture>();
			tex->Load("Assets/Default.png");
			SetTexture(tex);
		}
		float scaleOwner = mOwner.GetScale();
		auto pos = mOwner.GetPosition();
		auto translate = glm::translate(glm::identity<glm::mat4>(), glm::vec3(pos.x, pos.y, pos.z));
		glm::quat quat;
		{
			auto rot = mOwner.GetRotation();
			quat.x = rot.x;
			quat.y = rot.y;
			quat.z = rot.z;
			quat.w = rot.w;
		}
		auto rotate = glm::toMat4(quat);
		auto scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(scaleOwner, scaleOwner, 1));

		if (mOwner.GetScene()->GetRenderer()->GetGraphicsAPI() == GraphicsAPI::Vulkan)
		{
			auto view = mOwner.GetScene()->GetRenderer()->GetViewMatrix();
			static auto proj = glm::perspective(glm::radians(90.f), Window::Size.x / Window::Size.y, 0.01f, 10000.f);
			mTextureVK->param.projView = proj * view;
			mTextureVK->param.world = translate * rotate * scale;
		}
		else
		{
			auto view = mOwner.GetScene()->GetRenderer()->GetViewMatrix();
			static auto proj = glm::perspective(glm::radians(90.f), Window::Size.x / Window::Size.y, 0.01f, 10000.f);
			sprite->param.projView = proj * view;
			sprite->param.world = translate * rotate * scale;
		}
		mOwner.RecomuteFinished();
	}
	else
	{
		if (mOwner.GetScene()->GetRenderer()->GetGraphicsAPI() == GraphicsAPI::Vulkan)
		{
			auto view = mOwner.GetScene()->GetRenderer()->GetViewMatrix();
			static auto proj = glm::perspective(glm::radians(90.f), Window::Size.x / Window::Size.y, 0.01f, 10000.f);
			mTextureVK->param.projView = proj * view;
		}
		else
		{
			auto view = mOwner.GetScene()->GetRenderer()->GetViewMatrix();
			static auto proj = glm::perspective(glm::radians(90.f), Window::Size.x / Window::Size.y, 0.01f, 10000.f);
			sprite->param.projView = proj * view;
		}
	}
}

Sprite3DComponent::~Sprite3DComponent()
{
	mOwner.GetScene()->GetRenderer()->RemoveSprite3D(this);
}

void Sprite3DComponent::Draw(Shader *shader)
{
}

void Sprite3DComponent::SetTexture(std::shared_ptr<Texture> texture, const float scale)
{
	if (texture)
		mTexture = texture;
	else
	{
		auto tex = std::make_shared<Texture>();
		tex->Load("Assets/Default.png");
		SetTexture(tex);
	}
	auto renderer = mOwner.GetScene()->GetRenderer();
	if (renderer->GetGraphicsAPI() == GraphicsAPI::Vulkan)
	{
		mTextureVK = std::make_shared<SpriteVK>();

		renderer->GetVK().registerImageObject(mTexture);
		mTextureVK->mTexture = mTexture;
		renderer->GetVK().registerTexture(mTextureVK, mTexture->id, TextureType::Image3D);

		const auto &rot = mOwner.GetRotation();
		auto quat = glm::qua<float>();
		quat.x = rot.x;
		quat.y = rot.y;
		quat.z = rot.z;
		quat.w = rot.w;
		auto world = glm::translate(
						 glm::identity<glm::mat4>(),
						 glm::vec3(mOwner.GetPosition().x, mOwner.GetPosition().y, mOwner.GetPosition().z)) *
					 glm::toMat4(quat) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(mOwner.GetScale(), mOwner.GetScale(), 1));
		auto view = glm::lookAtRH(glm::vec3(0, 0, 0), glm::vec3(1.f, 0, 0), glm::vec3(0, 0, 1.f));
		auto proj = glm::perspective(glm::radians(90.f), Window::Size.x / Window::Size.y, 0.01f, 10000.f);

		mTextureVK->param.world = world;
		mTextureVK->param.projView = proj * view;
	}
	if (mOwner.GetScene()->GetRenderer()->GetGraphicsAPI() == GraphicsAPI::OpenGL)
	{
		mOwner.GetScene()->GetRenderer()->GetGL().registerTexture(mTexture, TextureType::Image3D);
		sprite = std::make_shared<SpriteGL>();
		sprite->textureIndex = mTexture->id;

		const auto &rot = mOwner.GetRotation();
		auto quat = glm::qua<float>();
		quat.x = rot.x;
		quat.y = rot.y;
		quat.z = rot.z;
		quat.w = rot.w;
		auto world = glm::translate(
						 glm::identity<glm::mat4>(),
						 glm::vec3(mOwner.GetPosition().x, mOwner.GetPosition().y, mOwner.GetPosition().z)) *
					 glm::toMat4(quat) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(mOwner.GetScale(), mOwner.GetScale(), 1));
		auto view = glm::lookAtRH(glm::vec3(0, 0, 0), glm::vec3(1.f, 0, 0), glm::vec3(0, 0, 1.f));
		auto proj = glm::perspective(glm::radians(90.f), Window::Size.x / Window::Size.y, 0.01f, 10000.f);

		sprite->param.world = world;
		sprite->param.projView = proj * view;
		mOwner.GetScene()->GetRenderer()->GetGL().pushSprite3d(sprite);
	}
	mOwner.GetScene()->GetRenderer()->AddSprite3D(this);
}
