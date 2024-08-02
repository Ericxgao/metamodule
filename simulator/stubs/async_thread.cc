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

} // namespace

AsyncThread::AsyncThread() = default;

AsyncThread::AsyncThread(Callback &&new_action)
	: action(std::move(new_action)) {
}

void AsyncThread::start(unsigned module_id) {
	printf("Start\n");
	if (action && module_id > 0) {
		printf("action && module_id\n");
		if (module_id < tasks.size()) {
			printf("module_id<\n");
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
	task_runner = std::thread([]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			for (auto &task : tasks) {
				if (task.enabled) {
					task.action();
				}
			}
		}
	});
}

void pause_module_threads() {
	//TODO: how to pause a thread?
	// We could cache the enabled fields, then set all enabled to false
}

} // namespace MetaModule
