#include "src/controls.hh"
#include <cmath>
#include <optional>

// #include "src/flags.hh"
// #include "src/params.hh"

namespace MetaModule
{

template</*class Parent,*/ class Mapping>
class SamplerChannel {
	// Parent *parent;

public:
	SamplerChannel() = default;
	// SamplerChannel(Parent *parent_)
	// 	: parent(parent_) {
	// }

	void update() {
	}

	void set_samplerate(float sr) {
		auto new_sample_rate = uint32_t(std::round(sr));
		if (new_sample_rate != sample_rate) {
			sample_rate = new_sample_rate;
			controls.set_samplerate(sample_rate);
			// sampler.set_samplerate(sampleRate);
		}
	}

	bool set_param(unsigned param_id, float val) {
		switch (param_id) {
			case Mapping::PitchKnob: {
				controls.pots[SamplerKit::PitchPot] = val * 4095;
			} break;

			case Mapping::StartPosKnob: {
				controls.pots[SamplerKit::StartPot] = val * 4095;
			} break;

			case Mapping::LengthKnob: {
				controls.pots[SamplerKit::LengthPot] = val * 4095;
			} break;

			case Mapping::SampleKnob: {
				controls.pots[SamplerKit::SamplePot] = val * 4095;
			} break;

			case Mapping::PlayButton: {
				controls.play_button.sideload_set(val > 0.5f);
			} break;

			case Mapping::BankButton: {
				controls.bank_button.sideload_set(val > 0.5f);
			} break;

			case Mapping::ReverseButton: {
				controls.rev_button.sideload_set(val > 0.5f);
			} break;

			default:
				return false; //not handled
		}
		return true; //handled
	}

	bool set_input(unsigned input_id, float volts) {
		auto bipolar_cv = [](float cv) {
			// (-5v, +5v] => [0, 4095]
			return std::clamp<int32_t>(std::round(cv * 409.6f + 2047.f), 0, 4095);
		};
		auto unipolar_cv = [](float cv) {
			// [0v, +5v] => [0, 4095]
			return std::clamp<int32_t>(std::round(cv * 819.f), 0, 4095);
		};

		switch (input_id) {
			case Mapping::PlayTrigIn: {
				controls.play_jack.register_state(volts > GateThreshold);
			} break;

			case Mapping::ReverseTrigIn: {
				controls.rev_jack.register_state(volts > GateThreshold);
			} break;

			case Mapping::VOctIn: {
				controls.cvs[SamplerKit::PitchCV] = bipolar_cv(volts);
			} break;

			case Mapping::LengthCvIn: {
				controls.pots[SamplerKit::LengthCV] = unipolar_cv(volts);
			} break;

			case Mapping::StartPosCvIn: {
				controls.pots[SamplerKit::StartCV] = unipolar_cv(volts);
			} break;

			case Mapping::SampleCvIn: {
				controls.pots[SamplerKit::SampleCV] = unipolar_cv(volts);
			} break;

			case Mapping::RecIn: {
				//not used
			} break;

			default:
				return false; //not handled
		}
		return true; //handled
	}

	float get_output(unsigned output_id) const {
		if (output_id == Mapping::Out)
			return out;
		else if (output_id == Mapping::EndOut)
			return controls.end_out.sideload_get();
		else
			return 0;
	}

	std::optional<float> get_led_brightness(unsigned led_id) const {
		switch (led_id) {
			case Mapping::PlayLight:
				return controls.playing_led.sideload_get();

			case Mapping::PlayButR:
				return controls.play_led_data.r;
			case Mapping::PlayButG:
				return controls.play_led_data.g;
			case Mapping::PlayButB:
				return controls.play_led_data.b;

			case Mapping::BankButR:
				return controls.bank_led_data.r;
			case Mapping::BankButG:
				return controls.bank_led_data.g;
			case Mapping::BankButB:
				return controls.bank_led_data.b;

			case Mapping::RevButR:
				return controls.rev_led_data.r;
			case Mapping::RevButG:
				return controls.rev_led_data.g;
			case Mapping::RevButB:
				return controls.rev_led_data.b;
		}
		return std::nullopt;
	}

private:
	float sample_rate = 48000;
	SamplerKit::Controls controls;

	float out; //placeholder

	static constexpr float GateThreshold = 1.0f;
};

} // namespace MetaModule
