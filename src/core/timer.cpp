#include "timer.h"


Timer::Timer()
{
	
}

void Timer::start()
{
	m_start_time = std::chrono::steady_clock::now();
	

}

int Timer::getMillis()
{
	return m_millis;
}

void Timer::update()
{
	std::chrono::steady_clock a;
	
	m_current_time = std::chrono::steady_clock::now();
	auto duration = m_current_time - m_start_time;
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	m_old_millis = m_millis;
	m_millis = (int)millis.count();

	m_delta_millis = m_millis - m_old_millis;
	//~ std::cout << "milliseconds : " << (millis.count()) << std::endl;
}

int Timer::getDeltaMillis()
{
	return m_delta_millis;
}
