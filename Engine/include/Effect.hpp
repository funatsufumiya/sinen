#pragma once
#include <boost/serialization/singleton.hpp>
namespace nen
{
	class Effect : public boost::serialization::singleton<Effect>
	{
	private:
		friend class boost::serialization::detail::singleton_wrapper<Effect>;
		Effect() = default;
	public:
		void Init(class VKBase* vkrenderer);
		bool Load(const std::u16string& filePath);
		void Remove();
		void Draw();
	};

}
