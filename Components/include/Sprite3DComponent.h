#pragma once
#include <SDL.h>
#include <Engine.hpp>

class Sprite3DComponent : public Component
{
public:
	Sprite3DComponent(class Actor &owner, int drawOrder = 100, Texture tex = Texture());
	~Sprite3DComponent();

	virtual void Draw(class Shader *shader);
	virtual void Update(float deltaTime) override;

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	void SetTexture(std::shared_ptr<class Texture> texture, float scale = 1.0f);
	void SetReverse(bool Use) { mUseReverse = Use; }
	void SetFlip(bool Use) { mUseFlip = Use; }
	void SetScale(const Vector2f &scale)
	{
		mTexWidth = static_cast<int>(static_cast<float>(mTexWidth) * scale.x);
		mTexHeight = static_cast<int>(static_cast<float>(mTexHeight) * scale.y);
	}
	void SetUseTrimming(const bool isuse) { mUseTrim = isuse; }
	void SetTrimmingStartPos(const Vector2i &pos)
	{
		trim_s.x = pos.x + (int)Window::Center.x;
		trim_s.y = pos.y + (int)Window::Center.y;
	}
	void SetTrimmingEndPos(const Vector2i &pos)
	{
		trim_e.x = pos.x;
		trim_e.y = pos.y;
	}
	void SetBoolean(bool boolean) { mBoolean = boolean; }
	void SetColor(const Color::Color &color) { mColor = color; }
	Color::Color GetColor() { return mColor; }

private:
	std::shared_ptr<class SpriteVK> mTextureVK;
	std::shared_ptr<class SpriteGL> sprite;
	int mDrawOrder = 0;
	int mTexWidth = 0;
	int mTexHeight = 0;
	bool mUseReverse = false;
	bool mUseFlip = false;
	bool mBoolean = false;
	Vector2f vuv = Vector2f(1.f, 1.f);
	Color::Color mColor = Color::Color(1.f, 1.f, 1.f, 1.f);
	bool mUseTrim = false;
	Vector2i trim_s;
	Vector2i trim_e;
	Actor &mOwner;

	std::shared_ptr<Texture> mTexture;
};
