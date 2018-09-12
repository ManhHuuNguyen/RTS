#include "GameClock.h"

GameClock::GameClock() {
	lastTime = std::chrono::time_point_cast<std::chrono::milliseconds>(myClock.now()).time_since_epoch().count();
}

long long GameClock::getTimePast() {
	long long currentTime = std::chrono::time_point_cast<std::chrono::milliseconds>(myClock.now()).time_since_epoch().count();
	long long dif = currentTime - lastTime;
	lastTime = currentTime;
	return dif;
}
