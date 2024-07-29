#pragma once
#include "util/callable.hh"

namespace MetaModule
{

class AsyncThread {
public:
	AsyncThread(unsigned id, Callback &&new_thread);
	~AsyncThread();

private:
	unsigned id;
};

} // namespace MetaModule
