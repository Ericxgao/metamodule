#include "CoreModules/async_thread.hh"
#include "drivers/timekeeper.hh"
#include "util/zip.hh"
#include <array>

namespace MetaModule
{

namespace //anonymous
{
constexpr int MAX_MODULES_IN_PATCH = 32;

struct Task {
	Callback action;
	bool enabled = false;
};

static std::array<Task, MAX_MODULES_IN_PATCH> tasks_core0;
static std::array<Task, MAX_MODULES_IN_PATCH> tasks_core1;

mdrivlib::Timekeeper async_task_core0;
mdrivlib::Timekeeper async_task_core1;

} // namespace

AsyncThread::AsyncThread(unsigned id, Callback &&new_action)
	: id{id} {

	uint32_t current_core = ((__get_MPIDR() & 0xFF) == 1) ? 1 : 0; //0 = CA7 Core 1, 1 = CA7 Core 2
	auto &tasks = current_core == 1 ? tasks_core1 : tasks_core0;
	if (id < tasks.size()) {
		tasks[id].action = std::move(new_action);
		__DMB();
		tasks[id].enabled = true;
	}
}

AsyncThread::~AsyncThread() {
	uint32_t current_core = ((__get_MPIDR() & 0xFF) == 1) ? 1 : 0; //0 = CA7 Core 1, 1 = CA7 Core 2
	auto &tasks = current_core == 1 ? tasks_core1 : tasks_core0;
	if (id < tasks.size())
		tasks[id].enabled = false;
}

void start_module_threads() {
	uint32_t current_core = ((__get_MPIDR() & 0xFF) == 1) ? 1 : 0; //0 = CA7 Core 1, 1 = CA7 Core 2
	auto &tasks = current_core == 1 ? tasks_core1 : tasks_core0;
	auto &task_runner = current_core == 1 ? async_task_core1 : async_task_core0;

	mdrivlib::TimekeeperConfig task_config{
		.TIMx = current_core == 0 ? TIM7 : TIM6,
		.period_ns = mdrivlib::TimekeeperConfig::Hz(2000),
		.priority1 = 3,
		.priority2 = 3,
	};

	task_runner.init(task_config, [&tasks]() {
		for (auto &task : tasks) {
			if (!task.enabled)
				continue;
			task.action();
		}
	});
	task_runner.start();
}

void pause_module_threads() {
	uint32_t current_core = ((__get_MPIDR() & 0xFF) == 1) ? 1 : 0; //0 = CA7 Core 1, 1 = CA7 Core 2
	auto &task_runner = current_core == 1 ? async_task_core1 : async_task_core0;
	task_runner.stop();
}

} // namespace MetaModule
