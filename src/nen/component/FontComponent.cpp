#include <nen.hpp>
#include <iostream>
namespace nen
{
	FontComponent::FontComponent(Actor &actor, int drawOrder) : Sprite2DComponent(actor, drawOrder)
	{
	}

	void FontComponent::SetString(const std::string &str, const Color &color, int pointSize)
	{
		if (!mFont->isLoaded())
		{
			mFont->Load("Assets/Font/mplus/mplus-1p-medium.ttf");
		}

		if (this->GetSprite())
		{
			GetActor().GetScene().GetRenderer()->RemoveSprite2D(this->GetSprite());
			this->GetSprite().reset();
		}
		Create(mFont->RenderText(str, color, pointSize));
	}
	void FontComponent::SetFont(std::shared_ptr<Font> font)
	{
		mFont = font;
	}
}