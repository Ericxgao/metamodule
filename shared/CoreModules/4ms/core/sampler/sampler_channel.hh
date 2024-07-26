#include "CoreModules/elements/base_element.hh"
// #include "src/flags.hh"
// #include "src/controls.hh"
// #include "src/params.hh"
// #include "src/looping_delay.hh"

namespace MetaModule
{

template<class Parent, class Mapping>
class SamplerChannel {
private:
	template<typename Parent::Info::Elem EL>
	void setOutput(auto val) {
		return parent->template setOutput<EL>(val);
	}

	template<typename Parent::Info::Elem EL>
	auto getInput() {
		return parent->template getInput<EL>();
	}

	template<typename Parent::Info::Elem EL, typename VAL>
	void setLED(const VAL &value) {
		return parent->template setLED<EL>(value);
	}

	template<typename Parent::Info::Elem EL>
	auto getState() {
		return parent->template getState<EL>();
	}

private:
	Parent *parent;

public:
	SamplerChannel(Parent *parent_)
		: parent(parent_) {
	}

	void update() {
	}
};

} // namespace MetaModule
