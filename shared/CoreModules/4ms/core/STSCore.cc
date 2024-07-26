#include "CoreModules/SmartCoreProcessor.hh"
#include "CoreModules/moduleFactory.hh"
#include "info/STS_info.hh"
#include "sampler/sampler_channel.hh"
#include <utility>

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
#ifdef __clang__
#define to_underlying __to_underlying
#endif
	struct MappingL {
		const static unsigned PitchKnob = std::to_underlying(PitchLKnob);
		const static unsigned SampleKnob = std::to_underlying(SampleLKnob);
		const static unsigned StartPosKnob = std::to_underlying(StartPos_LKnob);
		const static unsigned LengthKnob = std::to_underlying(LengthLKnob);
		const static unsigned PlayButton = std::to_underlying(PlayLButton);
		const static unsigned BankButton = std::to_underlying(BankLButton);
		const static unsigned ReverseButton = std::to_underlying(ReverseLButton);
		const static unsigned PlayTrigIn = std::to_underlying(PlayTrigLIn);
		const static unsigned VOctIn = std::to_underlying(_1V_OctLIn);
		const static unsigned ReverseTrigIn = std::to_underlying(ReverseTrigLIn);
		const static unsigned LengthCvIn = std::to_underlying(LengthCvLIn);
		const static unsigned StartPosCvIn = std::to_underlying(StartPosCvLIn);
		const static unsigned SampleCvIn = std::to_underlying(SampleCvLIn);
		const static unsigned RecIn = std::to_underlying(LeftRecIn);
		const static unsigned Out = std::to_underlying(OutLOut);
		const static unsigned EndOut = std::to_underlying(EndOutLOut);
		const static unsigned PlayLight = std::to_underlying(PlayLLight);
	};
	struct MappingR {
		const static unsigned PitchKnob = std::to_underlying(PitchRKnob);
		const static unsigned SampleKnob = std::to_underlying(SampleRKnob);
		const static unsigned StartPosKnob = std::to_underlying(StartPos_RKnob);
		const static unsigned LengthKnob = std::to_underlying(LengthRKnob);
		const static unsigned PlayButton = std::to_underlying(PlayLButton);
		const static unsigned BankButton = std::to_underlying(BankRButton);
		const static unsigned ReverseButton = std::to_underlying(ReverseRButton);
		const static unsigned PlayTrigIn = std::to_underlying(PlayTrigRIn);
		const static unsigned VOctIn = std::to_underlying(_1V_OctRIn);
		const static unsigned ReverseTrigIn = std::to_underlying(ReverseTrigRIn);
		const static unsigned LengthCvIn = std::to_underlying(LengthCvRIn);
		const static unsigned StartPosCvIn = std::to_underlying(StartPosCvRIn);
		const static unsigned SampleCvIn = std::to_underlying(SampleCvRIn);
		const static unsigned RecIn = std::to_underlying(RightRecIn);
		const static unsigned Out = std::to_underlying(OutROut);
		const static unsigned EndOut = std::to_underlying(EndOutROut);
		const static unsigned PlayLight = std::to_underlying(PlayRLight);
	};

public:
	STSCore() = default;

	void update() override {
	}

	void set_param(int param_id, float val) override {
		if (!chanL.set_param(param_id, val)) {
			chanR.set_param(param_id, val);
		}
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
	SamplerChannel<MappingL> chanL;
	SamplerChannel<MappingR> chanR;
};

} // namespace MetaModule
