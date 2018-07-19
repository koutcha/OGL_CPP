#pragma once
#include <memory>
namespace PongGame
{
	class SequanceBase
	{
	public:
		SequanceBase();
		virtual ~SequanceBase();
	private:
		virtual std::unique_ptr<SequanceBase> update(SequanceBase& parent) = 0;
	};

}


