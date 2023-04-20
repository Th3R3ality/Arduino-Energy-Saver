// Arduino Energy Saver.cpp
// PoC in C++

#include <iostream>
#include <chrono>
#include <tuple>
#include <thread>

template <class result_t = std::chrono::milliseconds, class clock_t = std::chrono::steady_clock, class duration_t = std::chrono::milliseconds>
auto since(std::chrono::time_point<clock_t, duration_t> const& start) {
	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}
auto start = std::chrono::steady_clock::now();

long long millis() {
	return since(start).count();
}

long long milliseconds(int clock_time) {
	int hours = (clock_time - (clock_time - static_cast<int>(clock_time / 100) * 100)) / 100;
	int minutes = clock_time - hours * 100;
	return hours * 60 * 60 * 1000 + minutes * 60 * 1000;
}

std::tuple<int, int, int>timestamp(long long milliseconds) {
	int hours = static_cast<int>(milliseconds / 1000 / 60 / 60);
	int minutes = static_cast<int>(milliseconds / 1000 / 60) - hours * 60;
	int seconds = static_cast<int>(milliseconds / 1000) - minutes * 60 - hours * 60 * 60;

	return { hours, minutes, seconds };
}

int main()
{
	int input_time = 0;

	std::cout << "Input Current time: ";
	std::cin >> input_time;
	std::cout << "Input: " << input_time << std::endl;
	auto time_offset = milliseconds(input_time);
	std::cout << "Input in Milliseconds: " << time_offset << std::endl;

	int hours, minutes, seconds;
	std::tie(hours, minutes, seconds) = timestamp(milliseconds(input_time));

	std::cout << "Hours: " << hours 
		<< "\nMinutes: " << minutes 
		<< "\nSeconds: " << seconds
		<< std::endl;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::tie(hours, minutes, seconds) = timestamp(millis() + time_offset);
		std::cout << "Time: " << hours << ":" << minutes << ":" << seconds << std::endl;
	}
}