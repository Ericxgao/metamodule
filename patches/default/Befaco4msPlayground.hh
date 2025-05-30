static const char Befaco4msPlayground_patch[] =
	R"(
PatchData:
  patch_name: Befaco4msPlayground
  description: Patch Description
  module_slugs:
    0: HubMedium
    1: EvenVCO
    2: Mixer
    3: Kickall
    4: MultiLFO
    5: CLKD
    6: ENVVCA
    7: CLKM
    8: SH
    9: Noise
    10: DualAtenuverter
    11: PonyVCO
  int_cables:
    - out:
        module_id: 3
        jack_id: 0
      ins:
        - module_id: 2
          jack_id: 0
    - out:
        module_id: 4
        jack_id: 1
      ins:
        - module_id: 5
          jack_id: 1
        - module_id: 7
          jack_id: 1
    - out:
        module_id: 5
        jack_id: 0
      ins:
        - module_id: 3
          jack_id: 0
    - out:
        module_id: 1
        jack_id: 2
      ins:
        - module_id: 6
          jack_id: 4
    - out:
        module_id: 6
        jack_id: 2
      ins:
        - module_id: 2
          jack_id: 1
    - out:
        module_id: 7
        jack_id: 0
      ins:
        - module_id: 6
          jack_id: 1
        - module_id: 8
          jack_id: 0
    - out:
        module_id: 8
        jack_id: 0
      ins:
        - module_id: 1
          jack_id: 1
    - out:
        module_id: 9
        jack_id: 0
      ins:
        - module_id: 10
          jack_id: 0
    - out:
        module_id: 10
        jack_id: 0
      ins:
        - module_id: 8
          jack_id: 1
    - out:
        module_id: 11
        jack_id: 0
      ins:
        - module_id: 2
          jack_id: 2
    - out:
        module_id: 6
        jack_id: 0
      ins:
        - module_id: 11
          jack_id: 1
    - out:
        module_id: 1
        jack_id: 1
      ins:
        - module_id: 10
          jack_id: 1
    - out:
        module_id: 10
        jack_id: 1
      ins:
        - module_id: 11
          jack_id: 0
  mapped_ins: []
  mapped_outs:
    - panel_jack_id: 0
      out:
        module_id: 2
        jack_id: 0
    - panel_jack_id: 2
      out:
        module_id: 3
        jack_id: 0
    - panel_jack_id: 3
      out:
        module_id: 6
        jack_id: 2
  static_knobs:
    - module_id: 1
      param_id: 0
      value: 0.333333
    - module_id: 1
      param_id: 1
      value: 0.5
    - module_id: 1
      param_id: 2
      value: 0.5
    - module_id: 2
      param_id: 0
      value: 1
    - module_id: 2
      param_id: 1
      value: 0.369578
    - module_id: 2
      param_id: 2
      value: 0.352723
    - module_id: 2
      param_id: 3
      value: 1
    - module_id: 3
      param_id: 0
      value: 0.63125
    - module_id: 3
      param_id: 1
      value: 0
    - module_id: 3
      param_id: 2
      value: 0.493976
    - module_id: 3
      param_id: 3
      value: 0.818072
    - module_id: 3
      param_id: 4
      value: 0.295
    - module_id: 3
      param_id: 5
      value: 0.153012
    - module_id: 4
      param_id: 0
      value: 0.19966
    - module_id: 4
      param_id: 1
      value: 0.5
    - module_id: 4
      param_id: 2
      value: 0.492771
    - module_id: 5
      param_id: 0
      value: 0.1005
    - module_id: 6
      param_id: 0
      value: 0.196
    - module_id: 6
      param_id: 1
      value: 0.246
    - module_id: 6
      param_id: 2
      value: 0.5
    - module_id: 6
      param_id: 3
      value: 0.5
    - module_id: 6
      param_id: 4
      value: 0.5
    - module_id: 6
      param_id: 5
      value: 0
    - module_id: 6
      param_id: 6
      value: 0.5
    - module_id: 6
      param_id: 7
      value: 0
    - module_id: 7
      param_id: 0
      value: 0.104941
    - module_id: 10
      param_id: 0
      value: 0.619277
    - module_id: 10
      param_id: 1
      value: 0.5
    - module_id: 10
      param_id: 2
      value: 0.80241
    - module_id: 10
      param_id: 3
      value: 0.530241
    - module_id: 11
      param_id: 0
      value: 0.162651
    - module_id: 11
      param_id: 1
      value: 0
    - module_id: 11
      param_id: 2
      value: 0
    - module_id: 11
      param_id: 3
      value: 0.666667
    - module_id: 11
      param_id: 4
      value: 0.333333
  mapped_knobs:
    - name: ''
      set:
        - panel_knob_id: 0
          module_id: 4
          param_id: 0
          curve_type: 0
          min: 0
          max: 0.623
        - panel_knob_id: 1
          module_id: 1
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 2
          module_id: 10
          param_id: 2
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 3
          module_id: 3
          param_id: 3
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 4
          module_id: 3
          param_id: 5
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 6
          module_id: 7
          param_id: 0
          curve_type: 0
          min: 0
          max: 0.541
        - panel_knob_id: 7
          module_id: 5
          param_id: 0
          curve_type: 0
          min: 0
          max: 0.201
        - panel_knob_id: 9
          module_id: 2
          param_id: 0
          curve_type: 0
          min: 0.26
          max: 1
        - panel_knob_id: 10
          module_id: 2
          param_id: 1
          curve_type: 0
          min: 0.195
          max: 1
        - panel_knob_id: 11
          module_id: 2
          param_id: 2
          curve_type: 0
          min: 0.274
          max: 1
    - name: ''
      set: []
    - name: ''
      set: []
    - name: ''
      set: []
)";
