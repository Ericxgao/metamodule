#include "CoreModules/async_thread.hh"
#include <array>
#include <list>
#include <thread>

namespace MetaModule
{

namespace //anonymous
{

//TODO: use pImpl with AsyncThread
// Rack and sim Impl will have a std::thread
// Firmware Impl will have an id in a static array

static std::list<std::thread> threads;

static std::atomic<bool> kill_all_signal = false;
static std::atomic<bool> pause_all_signal = false;

} // namespace

AsyncThread::AsyncThread() = default;

AsyncThread::AsyncThread(Callback &&new_action)
	: action(std::move(new_action)) {
	printf("New thread, no id\n");
}

void AsyncThread::start(unsigned module_id) {
	if (action && module_id > 0) {
		id = module_id;
		printf("Start thread %d\n", id);

		threads.emplace_back([this]() {
			while (!kill_all_signal && id != 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				if (!pause_all_signal)
					action();
			}
			// printf("Died\n");
		});
	}
}

void AsyncThread::start(unsigned module_id, Callback &&new_action) {
	action = std::move(new_action);
	start(module_id);
}

AsyncThread::~AsyncThread() {
	printf("Kill thread %d\n", id);
	id = 0; //kill

	// how to wait for it to die?
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void start_module_threads() {
	// printf("Starting threads...\n");
	kill_all_signal = false;
}

void pause_module_threads() {
	pause_all_signal = true;
}

void kill_module_threads() {
	printf("Killing threads...\n");
	kill_all_signal = true;

	for (auto &t : threads) {
		t.join();
	}
}

} // namespace MetaModule
