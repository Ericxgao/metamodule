#include "CoreModules/async_thread.hh"
#include <array>
#include <thread>

namespace MetaModule
{

namespace //anonymous
{

constexpr int MAX_MODULES_IN_PATCH = 32;

struct Task {
	Callback action;
	std::atomic<bool> enabled = false;
};

static std::array<Task, MAX_MODULES_IN_PATCH> tasks;

std::thread task_runner;
std::atomic<bool> kill_signal = false;

} // namespace

AsyncThread::AsyncThread() = default;

AsyncThread::AsyncThread(Callback &&new_action)
	: action(std::move(new_action)) {
}

void AsyncThread::start(unsigned module_id) {
	if (action && module_id > 0) {
		if (module_id < tasks.size()) {
			id = module_id;
			tasks[id].action = action;
			tasks[id].enabled = true;
		}
	}
}

void AsyncThread::start(unsigned module_id, Callback &&new_action) {
	action = std::move(new_action);
	start(module_id);
}

AsyncThread::~AsyncThread() {
	if (id < tasks.size()) {
		tasks[id].enabled = false;
	}
}

void start_module_threads() {
	kill_signal = false;
	task_runner = std::thread([]() {
		while (!kill_signal) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			for (auto &task : tasks) {
				if (task.enabled) {
					task.action();
				}
			}
		}
	});
}

void pause_module_threads() {
	kill_signal = true;
}

void kill_module_threads() {
	kill_signal = true;
	task_runner.join();
}

} // namespace MetaModule
