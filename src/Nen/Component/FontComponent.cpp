#include <Nen.hpp>
#include <iostream>
namespace nen
{
	FontComponent::FontComponent(Actor& actor, int drawOrder) : Sprite2DComponent(actor, drawOrder)
	{
	}

	void FontComponent::SetString(const std::string& str, const Color& color, int pointSize)
	{
		if (!mFont->isLoaded())
		{
			mFont->Load("Assets/Font/mplus/mplus-1p-medium.ttf", 32);
		}

		if (this->GetSprite())
		{
			GetActor().GetScene().GetRenderer()->RemoveSprite2D(this->GetSprite());
			this->GetSprite().reset();
		}
		Create(mFont->RenderText(str, color));
	}
	void FontComponent::SetFont(std::shared_ptr<Font> font)
	{
		mFont = font;
	}
}