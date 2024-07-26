#include "CoreModules/elements/base_element.hh"
// #include "src/flags.hh"
#include "src/controls.hh"
// #include "src/params.hh"
// #include "src/looping_delay.hh"

namespace MetaModule
{

template<class Parent, class Mapping>
class SamplerChannel {
	Parent *parent;

public:
	SamplerChannel(Parent *parent_)
		: parent(parent_) {
	}

	void update() {
	}

	void set_samplerate(float sr) {
		auto newSampleRate = uint32_t(std::round(sr));
		if (newSampleRate != sampleRate) {
			sampleRate = newSampleRate;
			// sampler.set_samplerate(sampleRate);
		}
	}

	bool set_param(unsigned param_id, float val) {
		switch (param_id) {
			case (Mapping::PitchKnob): {
				controls.pots[SamplerKit::PitchPot] = val * 4095;
			} break;

			case (Mapping::StartPosKnob): {
				controls.pots[SamplerKit::StartPot] = val * 4095;
			} break;

			case (Mapping::LengthPot): {
				controls.pots[SamplerKit::LengthPot] = val * 4095;
			} break;

			case (Mapping::SampleKnob): {
				controls.pots[SamplerKit::SamplePot] = val * 4095;
			} break;

			case (Mapping::PlayButton): {
				controls.play_button.sideload_set(val > 0.5f);
			} break;

			case (Mapping::BankButton): {
				controls.bank_button.sideload_set(val > 0.5f);
			} break;

			case (Mapping::ReverseButton): {
				controls.rev_button.sideload_set(val > 0.5f);
			} break;

			default:
				return false; //not handled
		}
		return true; //handled
	}

private:
	float sampleRate = 48000;
	SamplerKit::Controls controls;
};

} // namespace MetaModule
