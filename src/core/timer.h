#pragma once
#include <chrono>
//#include <ctime>
#include "../pch.h"
class Timer
{
	public:
	
		Timer();
		void start();
		void update();
		int getMillis();
		int getDeltaMillis();

		
	private:
		std::chrono::steady_clock a;
		int m_millis;
		int m_old_millis;
		int m_delta_millis;
		std::chrono::steady_clock::time_point m_start_time, m_current_time;

		
};

