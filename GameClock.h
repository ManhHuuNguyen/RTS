#pragma once
#include <chrono>
class GameClock {

	public:
		std::chrono::high_resolution_clock myClock;
		long long lastTime;

		GameClock();
		long long getTimePast();

};
