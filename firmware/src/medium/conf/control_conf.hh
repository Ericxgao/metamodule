#pragma once
#include "drivers/adc_builtin_conf.hh"
#include "drivers/adc_periph_nums.hh"
#include "drivers/dma_config_struct.hh"
#include "drivers/pin.hh"
#include "drivers/pin_change_conf.hh"
#include <array>

namespace MetaModule
{

struct FrameRatePinChangeConf : mdrivlib::DefaultPinChangeConf {
	static constexpr uint32_t pin = 12;
	static constexpr mdrivlib::GPIO port = mdrivlib::GPIO::D;
	static constexpr bool on_rising_edge = true;
	static constexpr bool on_falling_edge = false;
	static constexpr uint32_t priority1 = 0;
	static constexpr uint32_t priority2 = 3;
};

using mdrivlib::GPIO;
using mdrivlib::PinNum;

struct PotAdcConf : mdrivlib::DefaultAdcPeriphConf {
	static constexpr mdrivlib::AdcResolution resolution = mdrivlib::Bits12;
	static constexpr auto adc_periph_num = mdrivlib::AdcPeriphNum::_1;
	static constexpr auto oversample = true;
	static constexpr auto oversampling_ratio = 1024;
	static constexpr auto oversampling_right_bitshift = mdrivlib::AdcOversampleRightBitShift::Shift10Right;
	static constexpr auto clock_div = mdrivlib::PLL_Div1;

	static constexpr bool enable_end_of_sequence_isr = true;
	static constexpr bool enable_end_of_conversion_isr = false;

	struct DmaConf : mdrivlib::DefaultAdcPeriphConf::DmaConf {
		static constexpr auto DMAx = 2;
		static constexpr auto StreamNum = 7;
		static constexpr auto RequestNum = DMA_REQUEST_ADC1;
		static constexpr auto dma_priority = Low;
	};
};

using mdrivlib::AdcChannelConf;
enum Pots : uint32_t { PotA, PotB, PotC, PotD, PotE, PotF, PotU, PotV, PotW, PotX, PotY, PotZ };
constexpr auto AdcSampTime = mdrivlib::AdcSamplingTime::_2Cycles;
constexpr auto PotConfs = std::to_array({
	AdcChannelConf{{GPIO::B, PinNum::_1}, mdrivlib::AdcChanNum::_5, PotA, AdcSampTime},
	AdcChannelConf{{GPIO::A, PinNum::_2}, mdrivlib::AdcChanNum::_14, PotB, AdcSampTime},
	AdcChannelConf{{GPIO::B, PinNum::_0}, mdrivlib::AdcChanNum::_9, PotC, AdcSampTime},
	AdcChannelConf{{GPIO::C, PinNum::_1}, mdrivlib::AdcChanNum::_11, PotD, AdcSampTime},
	AdcChannelConf{{GPIO::C, PinNum::_2}, mdrivlib::AdcChanNum::_12, PotE, AdcSampTime},
	AdcChannelConf{{GPIO::A, PinNum::_5}, mdrivlib::AdcChanNum::_19, PotF, AdcSampTime},
	AdcChannelConf{{GPIO::A, PinNum::_6}, mdrivlib::AdcChanNum::_3, PotU, AdcSampTime},
	AdcChannelConf{{GPIO::A, PinNum::_7}, mdrivlib::AdcChanNum::_7, PotV, AdcSampTime},
	AdcChannelConf{{GPIO::C, PinNum::_5}, mdrivlib::AdcChanNum::_8, PotW, AdcSampTime},
	AdcChannelConf{{GPIO::A, PinNum::_3}, mdrivlib::AdcChanNum::_15, PotX, AdcSampTime},
	AdcChannelConf{{GPIO::C, PinNum::_3}, mdrivlib::AdcChanNum::_13, PotY, AdcSampTime},
	AdcChannelConf{{GPIO::A, PinNum::_4}, mdrivlib::AdcChanNum::_18, PotZ, AdcSampTime},
});

constexpr int32_t MinPotValue = 72; // more like 69, actually 0x3D = 61
constexpr float MaxPotValue = 4095.f - (float)MinPotValue - 4.f;

} // namespace MetaModule
