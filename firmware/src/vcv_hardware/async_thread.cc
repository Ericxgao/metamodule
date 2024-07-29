#include "CoreModules/async_thread.hh"
#include "drivers/timekeeper.hh"
#include "util/zip.hh"
#include <array>

namespace MetaModule
{

namespace //anonymous
{
constexpr int MAX_MODULES_IN_PATCH = 32;
static std::array<Callback, MAX_MODULES_IN_PATCH> tasks;
static std::array<bool, MAX_MODULES_IN_PATCH> task_enabled{};

mdrivlib::Timekeeper async_task;

} // namespace

AsyncThread::AsyncThread(unsigned id, Callback &&new_thread)
	: id{id} {
	if (id < tasks.size()) {
		tasks[id] = std::move(new_thread);
		__DMB();
		task_enabled[id] = true;
	}
}

AsyncThread::~AsyncThread() {
	if (id < tasks.size())
		task_enabled[id] = false;
}

void start_module_threads() {
	mdrivlib::TimekeeperConfig task_config{
		.TIMx = TIM7,
		.period_ns = mdrivlib::TimekeeperConfig::Hz(2000),
		.priority1 = 3,
		.priority2 = 3,
	};

	async_task.init(task_config, []() {
		for (auto [task, enabled] : zip(tasks, task_enabled)) {
			if (!enabled)
				continue;
			task();
		}
	});
}

void pause_module_threads() {
	async_task.stop();
}

} // namespace MetaModule
