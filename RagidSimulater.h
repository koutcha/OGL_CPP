#pragma once
namespace PongGame
{
	class GameResourceManager;
}
namespace Simulater
{
	class RagidSimulater
	{
	public:
		RagidSimulater();
		~RagidSimulater();
	private:
		enum State
		{
			Setting,
			Simulating,
			Reset,
		};
	};


}
