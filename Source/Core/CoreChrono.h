#pragma once
#include <chrono>
namespace Rainbow3D {
	inline float CalculateDeltaTime() {
        float delta_time;
        static std::chrono::steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };
        {
            using namespace std::chrono;

            steady_clock::time_point tick_time_point = steady_clock::now();
            duration<float> time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
            delta_time = time_span.count();

            m_last_tick_time_point = tick_time_point;
        }
        return delta_time;
	}
}