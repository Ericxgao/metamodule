#include "CoreModules/CoreHelper.hh"
#include "CoreModules/moduleFactory.hh"
#include "info/STS_info.hh"
#include "sampler/sampler_channel.hh"

namespace MetaModule
{

class STSCore : public CoreProcessor {
public:
	using Info = STSInfo;
	using ThisCore = STSCore;
	using enum Info::Elem;

private:
	struct MappingL {
		const static unsigned PitchKnob = CoreHelper<STSInfo>::param_index<PitchLKnob>();
		const static unsigned SampleKnob = CoreHelper<STSInfo>::param_index<SampleLKnob>();
		const static unsigned StartPosKnob = CoreHelper<STSInfo>::param_index<StartPos_LKnob>();
		const static unsigned LengthKnob = CoreHelper<STSInfo>::param_index<LengthLKnob>();
		const static unsigned PlayButton = CoreHelper<STSInfo>::param_index<PlayLButton>();
		const static unsigned BankButton = CoreHelper<STSInfo>::param_index<BankLButton>();
		const static unsigned ReverseButton = CoreHelper<STSInfo>::param_index<ReverseLButton>();
		const static unsigned PlayTrigIn = CoreHelper<STSInfo>::input_index<PlayTrigLIn>();
		const static unsigned VOctIn = CoreHelper<STSInfo>::input_index<_1V_OctLIn>();
		const static unsigned ReverseTrigIn = CoreHelper<STSInfo>::input_index<ReverseTrigLIn>();
		const static unsigned LengthCvIn = CoreHelper<STSInfo>::input_index<LengthCvLIn>();
		const static unsigned StartPosCvIn = CoreHelper<STSInfo>::input_index<StartPosCvLIn>();
		const static unsigned SampleCvIn = CoreHelper<STSInfo>::input_index<SampleCvLIn>();
		const static unsigned RecIn = CoreHelper<STSInfo>::input_index<LeftRecIn>();
		const static unsigned Out = CoreHelper<STSInfo>::output_index<OutLOut>();
		const static unsigned EndOut = CoreHelper<STSInfo>::output_index<EndOutLOut>();
		const static unsigned PlayLight = CoreHelper<STSInfo>::first_light_index<PlayLLight>();
		const static unsigned PlayButR = CoreHelper<STSInfo>::first_light_index<PlayLButton>() + 0;
		const static unsigned PlayButG = CoreHelper<STSInfo>::first_light_index<PlayLButton>() + 1;
		const static unsigned PlayButB = CoreHelper<STSInfo>::first_light_index<PlayLButton>() + 2;
		const static unsigned RevButR = CoreHelper<STSInfo>::first_light_index<ReverseLButton>() + 0;
		const static unsigned RevButG = CoreHelper<STSInfo>::first_light_index<ReverseLButton>() + 1;
		const static unsigned RevButB = CoreHelper<STSInfo>::first_light_index<ReverseLButton>() + 2;
		const static unsigned BankButR = CoreHelper<STSInfo>::first_light_index<BankLButton>() + 0;
		const static unsigned BankButG = CoreHelper<STSInfo>::first_light_index<BankLButton>() + 1;
		const static unsigned BankButB = CoreHelper<STSInfo>::first_light_index<BankLButton>() + 2;
	};
	struct MappingR {
		const static unsigned PitchKnob = CoreHelper<STSInfo>::param_index<PitchRKnob>();
		const static unsigned SampleKnob = CoreHelper<STSInfo>::param_index<SampleRKnob>();
		const static unsigned StartPosKnob = CoreHelper<STSInfo>::param_index<StartPos_RKnob>();
		const static unsigned LengthKnob = CoreHelper<STSInfo>::param_index<LengthRKnob>();
		const static unsigned PlayButton = CoreHelper<STSInfo>::param_index<PlayLButton>();
		const static unsigned BankButton = CoreHelper<STSInfo>::param_index<BankRButton>();
		const static unsigned ReverseButton = CoreHelper<STSInfo>::param_index<ReverseRButton>();
		const static unsigned PlayTrigIn = CoreHelper<STSInfo>::input_index<PlayTrigRIn>();
		const static unsigned VOctIn = CoreHelper<STSInfo>::input_index<_1V_OctRIn>();
		const static unsigned ReverseTrigIn = CoreHelper<STSInfo>::input_index<ReverseTrigRIn>();
		const static unsigned LengthCvIn = CoreHelper<STSInfo>::input_index<LengthCvRIn>();
		const static unsigned StartPosCvIn = CoreHelper<STSInfo>::input_index<StartPosCvRIn>();
		const static unsigned SampleCvIn = CoreHelper<STSInfo>::input_index<SampleCvRIn>();
		const static unsigned RecIn = CoreHelper<STSInfo>::input_index<RightRecIn>();
		const static unsigned Out = CoreHelper<STSInfo>::output_index<OutROut>();
		const static unsigned EndOut = CoreHelper<STSInfo>::output_index<EndOutROut>();
		const static unsigned PlayLight = CoreHelper<STSInfo>::first_light_index<PlayRLight>();
		const static unsigned PlayButR = CoreHelper<STSInfo>::first_light_index<PlayRButton>() + 0;
		const static unsigned PlayButG = CoreHelper<STSInfo>::first_light_index<PlayRButton>() + 1;
		const static unsigned PlayButB = CoreHelper<STSInfo>::first_light_index<PlayRButton>() + 2;
		const static unsigned RevButR = CoreHelper<STSInfo>::first_light_index<ReverseRButton>() + 0;
		const static unsigned RevButG = CoreHelper<STSInfo>::first_light_index<ReverseRButton>() + 1;
		const static unsigned RevButB = CoreHelper<STSInfo>::first_light_index<ReverseRButton>() + 2;
		const static unsigned BankButR = CoreHelper<STSInfo>::first_light_index<BankRButton>() + 0;
		const static unsigned BankButG = CoreHelper<STSInfo>::first_light_index<BankRButton>() + 1;
		const static unsigned BankButB = CoreHelper<STSInfo>::first_light_index<BankRButton>() + 2;
	};

public:
	STSCore() = default;

	void update() override {
		chanL.update();
		chanR.update();
	}

	void set_param(int param_id, float val) override {
		if (chanL.set_param(param_id, val))
			return;
		else
			chanR.set_param(param_id, val);
	}

	void set_input(int input_id, float val) override {
		if (chanL.set_input(input_id, val))
			return;
		else
			chanR.set_input(input_id, val);
	}

	float get_output(int output_id) const override {
		return 0.f;
	}

	void set_samplerate(float sr) override {
		chanL.set_samplerate(sr);
		chanR.set_samplerate(sr);
	}

	float get_led_brightness(int led_id) const override {
		if (auto found = chanL.get_led_brightness(led_id); found.has_value())
			return *found;

		else if (auto found = chanR.get_led_brightness(led_id); found.has_value())
			return *found;

		else
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
