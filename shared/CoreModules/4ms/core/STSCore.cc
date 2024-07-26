#include "CoreModules/SmartCoreProcessor.hh"
#include "CoreModules/moduleFactory.hh"
#include "info/STS_info.hh"

namespace MetaModule
{

class STSCore : public SmartCoreProcessor<STSInfo> {
public:
	using Info = STSInfo;
	using ThisCore = STSCore;
	using enum Info::Elem;

	template<Info::Elem EL>
	void setOutput(auto val) {
		return SmartCoreProcessor<Info>::setOutput<EL>(val);
	}

	template<Info::Elem EL>
	auto getInput() {
		return SmartCoreProcessor<Info>::getInput<EL>();
	}

	template<Info::Elem EL, typename VAL>
	void setLED(const VAL &value) {
		return SmartCoreProcessor<Info>::setLED<EL>(value);
	}

	template<Info::Elem EL>
	auto getState() {
		return SmartCoreProcessor<Info>::getState<EL>();
	}

private:
	struct MappingL {
		const static Info::Elem PitchKnob = PitchLKnob;
		const static Info::Elem SampleKnob = SampleLKnob;
		const static Info::Elem StartPos_Knob = StartPos_LKnob;
		const static Info::Elem LengthKnob = LengthLKnob;
		const static Info::Elem PlayButton = PlayLButton;
		const static Info::Elem BankButton = BankLButton;
		const static Info::Elem ReverseButton = ReverseLButton;
		const static Info::Elem PlayTrigIn = PlayTrigLIn;
		const static Info::Elem _1V_OctIn = _1V_OctLIn;
		const static Info::Elem ReverseTrigIn = ReverseTrigLIn;
		const static Info::Elem LengthCvIn = LengthCvLIn;
		const static Info::Elem StartPosCvIn = StartPosCvLIn;
		const static Info::Elem SampleCvIn = SampleCvLIn;
		const static Info::Elem OutOut = OutLOut;
		const static Info::Elem EndOutOut = EndOutLOut;
		const static Info::Elem PlayLight = PlayLLight;
	};
	struct MappingR {
		const static Info::Elem PitchKnob = PitchRKnob;
		const static Info::Elem SampleKnob = SampleRKnob;
		const static Info::Elem StartPos_Knob = StartPos_RKnob;
		const static Info::Elem LengthKnob = LengthRKnob;
		const static Info::Elem PlayButton = PlayLButton;
		const static Info::Elem BankButton = BankRButton;
		const static Info::Elem ReverseButton = ReverseRButton;
		const static Info::Elem PlayTrigIn = PlayTrigRIn;
		const static Info::Elem _1V_OctIn = _1V_OctRIn;
		const static Info::Elem ReverseTrigIn = ReverseTrigRIn;
		const static Info::Elem LengthCvIn = LengthCvRIn;
		const static Info::Elem StartPosCvIn = StartPosCvRIn;
		const static Info::Elem SampleCvIn = SampleCvRIn;
		const static Info::Elem OutOut = OutROut;
		const static Info::Elem EndOutOut = EndOutROut;
		const static Info::Elem PlayLight = PlayRLight;
	};

public:
	STSCore() = default;

	void update() override {
	}

	void set_param(int param_id, float val) override {
	}

	void set_input(int input_id, float val) override {
	}

	float get_output(int output_id) const override {
		return 0.f;
	}

	void set_samplerate(float sr) override {
	}

	float get_led_brightness(int led_id) const override {
		return 0.f;
	}

	// Boilerplate to auto-register in ModuleFactory
	// clang-format off
	static std::unique_ptr<CoreProcessor> create() { return std::make_unique<ThisCore>(); }
	static inline bool s_registered = ModuleFactory::registerModuleType(Info::slug, create, ModuleInfoView::makeView<Info>(), Info::png_filename);
	// clang-format on

private:
};

} // namespace MetaModule
