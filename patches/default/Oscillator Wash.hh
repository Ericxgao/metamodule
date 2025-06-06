// Automatically generated, do not edit
static const char patch_Oscillator_Wash[] =
	R"(
PatchData:
  patch_name: Oscillator Wash
  description: "3 sine waves with common volume and pan LFOs. Each LFO has it's own offset so that they're always moving."
  module_slugs:
    0: '4msCompany:HubMedium'
    1: 'Befaco:DualAtenuverter'
    2: '4msCompany:MultiLFO'
    3: '4msCompany:MultiLFO'
    4: 'Befaco:PonyVCO'
    5: 'Befaco:DualAtenuverter'
    6: '4msCompany:Pan'
    7: '4msCompany:MultiLFO'
    8: '4msCompany:MultiLFO'
    9: 'Befaco:PonyVCO'
    10: '4msCompany:Pan'
    11: 'Befaco:DualAtenuverter'
    12: '4msCompany:MultiLFO'
    13: '4msCompany:MultiLFO'
    14: 'Befaco:PonyVCO'
    15: '4msCompany:Pan'
    16: '4msCompany:StMix'
  int_cables:
    - out:
        module_id: 5
        jack_id: 1
      ins:
        - module_id: 10
          jack_id: 0
      color: 61865
    - out:
        module_id: 9
        jack_id: 0
      ins:
        - module_id: 10
          jack_id: 1
      color: 61865
    - out:
        module_id: 11
        jack_id: 1
      ins:
        - module_id: 15
          jack_id: 0
      color: 1453
    - out:
        module_id: 14
        jack_id: 0
      ins:
        - module_id: 15
          jack_id: 1
      color: 13501
    - out:
        module_id: 12
        jack_id: 3
      ins:
        - module_id: 11
          jack_id: 0
      color: 35419
    - out:
        module_id: 13
        jack_id: 3
      ins:
        - module_id: 11
          jack_id: 1
      color: 61865
    - out:
        module_id: 3
        jack_id: 3
      ins:
        - module_id: 5
          jack_id: 0
      color: 61865
    - out:
        module_id: 8
        jack_id: 3
      ins:
        - module_id: 5
          jack_id: 1
      color: 64934
    - out:
        module_id: 2
        jack_id: 3
      ins:
        - module_id: 1
          jack_id: 0
      color: 61865
    - out:
        module_id: 7
        jack_id: 3
      ins:
        - module_id: 1
          jack_id: 1
      color: 1453
    - out:
        module_id: 5
        jack_id: 0
      ins:
        - module_id: 6
          jack_id: 0
      color: 61865
    - out:
        module_id: 4
        jack_id: 0
      ins:
        - module_id: 6
          jack_id: 1
      color: 61865
    - out:
        module_id: 6
        jack_id: 0
      ins:
        - module_id: 16
          jack_id: 0
      color: 1453
    - out:
        module_id: 6
        jack_id: 1
      ins:
        - module_id: 16
          jack_id: 1
      color: 13501
    - out:
        module_id: 10
        jack_id: 0
      ins:
        - module_id: 16
          jack_id: 2
      color: 35419
    - out:
        module_id: 10
        jack_id: 1
      ins:
        - module_id: 16
          jack_id: 3
      color: 61865
    - out:
        module_id: 15
        jack_id: 0
      ins:
        - module_id: 16
          jack_id: 4
      color: 64934
    - out:
        module_id: 15
        jack_id: 1
      ins:
        - module_id: 16
          jack_id: 5
      color: 1453
    - out:
        module_id: 1
        jack_id: 0
      ins:
        - module_id: 4
          jack_id: 4
      color: 64934
    - out:
        module_id: 1
        jack_id: 1
      ins:
        - module_id: 9
          jack_id: 4
      color: 64934
    - out:
        module_id: 11
        jack_id: 0
      ins:
        - module_id: 14
          jack_id: 4
      color: 13501
  mapped_ins: []
  mapped_outs:
    - panel_jack_id: 0
      out:
        module_id: 16
        jack_id: 0
    - panel_jack_id: 1
      out:
        module_id: 16
        jack_id: 1
  static_knobs:
    - module_id: 4
      param_id: 0
      value: 0.393976
    - module_id: 4
      param_id: 1
      value: 0
    - module_id: 4
      param_id: 2
      value: 0
    - module_id: 4
      param_id: 3
      value: 0.666667
    - module_id: 4
      param_id: 4
      value: 0
    - module_id: 6
      param_id: 0
      value: 0.5
    - module_id: 16
      param_id: 0
      value: 0.212048
    - module_id: 16
      param_id: 1
      value: 0.212048
    - module_id: 16
      param_id: 2
      value: 0.212048
    - module_id: 16
      param_id: 3
      value: 0.0144576
    - module_id: 16
      param_id: 4
      value: 0.5
    - module_id: 16
      param_id: 5
      value: 0.5
    - module_id: 16
      param_id: 6
      value: 0.5
    - module_id: 16
      param_id: 7
      value: 0.5
    - module_id: 9
      param_id: 0
      value: 0.361446
    - module_id: 9
      param_id: 1
      value: 0
    - module_id: 9
      param_id: 2
      value: 0
    - module_id: 9
      param_id: 3
      value: 0.666667
    - module_id: 9
      param_id: 4
      value: 0
    - module_id: 10
      param_id: 0
      value: 0.5
    - module_id: 14
      param_id: 0
      value: 0.415662
    - module_id: 14
      param_id: 1
      value: 0
    - module_id: 14
      param_id: 2
      value: 0
    - module_id: 14
      param_id: 3
      value: 0.666667
    - module_id: 14
      param_id: 4
      value: 0
    - module_id: 15
      param_id: 0
      value: 0.5
    - module_id: 1
      param_id: 0
      value: 0.750602
    - module_id: 1
      param_id: 1
      value: 0.657229
    - module_id: 1
      param_id: 2
      value: 0.721687
    - module_id: 1
      param_id: 3
      value: 0.657229
    - module_id: 11
      param_id: 0
      value: 0.745783
    - module_id: 11
      param_id: 1
      value: 0.659285
    - module_id: 11
      param_id: 2
      value: 0.525301
    - module_id: 11
      param_id: 3
      value: 0.657914
    - module_id: 5
      param_id: 0
      value: 0.525301
    - module_id: 5
      param_id: 1
      value: 0.498
    - module_id: 5
      param_id: 2
      value: 0.525301
    - module_id: 5
      param_id: 3
      value: 0.503
    - module_id: 2
      param_id: 0
      value: 0.44
    - module_id: 2
      param_id: 1
      value: 0
    - module_id: 2
      param_id: 2
      value: 0.5
    - module_id: 2
      param_id: 3
      value: 1
    - module_id: 12
      param_id: 0
      value: 0.544
    - module_id: 12
      param_id: 1
      value: 0
    - module_id: 12
      param_id: 2
      value: 0.5
    - module_id: 12
      param_id: 3
      value: 1
    - module_id: 7
      param_id: 0
      value: 0.5
    - module_id: 7
      param_id: 1
      value: 0
    - module_id: 7
      param_id: 2
      value: 0.5
    - module_id: 7
      param_id: 3
      value: 1
    - module_id: 3
      param_id: 0
      value: 0
    - module_id: 3
      param_id: 1
      value: 0
    - module_id: 3
      param_id: 2
      value: 0.5
    - module_id: 3
      param_id: 3
      value: 1
    - module_id: 8
      param_id: 0
      value: 0
    - module_id: 8
      param_id: 1
      value: 0
    - module_id: 8
      param_id: 2
      value: 0.5
    - module_id: 8
      param_id: 3
      value: 1
    - module_id: 13
      param_id: 0
      value: 0
    - module_id: 13
      param_id: 1
      value: 0
    - module_id: 13
      param_id: 2
      value: 0.5
    - module_id: 13
      param_id: 3
      value: 1
  mapped_knobs:
    - name: ''
      set:
        - panel_knob_id: 0
          module_id: 4
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Pitch 1
        - panel_knob_id: 1
          module_id: 9
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Pitch 2
        - panel_knob_id: 2
          module_id: 14
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Pitch 3
        - panel_knob_id: 3
          module_id: 4
          param_id: 2
          curve_type: 0
          min: 0
          max: 1
          alias_name: Timbre
        - panel_knob_id: 3
          module_id: 9
          param_id: 2
          curve_type: 0
          min: 0
          max: 1
          alias_name: Timbre
        - panel_knob_id: 3
          module_id: 14
          param_id: 2
          curve_type: 0
          min: 0
          max: 1
          alias_name: Timbre
        - panel_knob_id: 4
          module_id: 2
          param_id: 0
          curve_type: 0
          min: 0.44
          max: 0.805
          alias_name: Vol LFO Rate
        - panel_knob_id: 4
          module_id: 12
          param_id: 0
          curve_type: 0
          min: 0.544
          max: 1
          alias_name: Vol LFO Rate
        - panel_knob_id: 4
          module_id: 7
          param_id: 0
          curve_type: 0
          min: 0.5
          max: 0.889
          alias_name: Vol LFO Rate
        - panel_knob_id: 5
          module_id: 3
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Pan LFO Rate
        - panel_knob_id: 5
          module_id: 8
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Pan LFO Rate
        - panel_knob_id: 5
          module_id: 13
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Pan LFO Rate
        - panel_knob_id: 6
          module_id: 1
          param_id: 1
          curve_type: 0
          min: 1
          max: 0.5
          alias_name: Amplitude LFO Depth
        - panel_knob_id: 6
          module_id: 1
          param_id: 3
          curve_type: 0
          min: 1
          max: 0.5
          alias_name: Amplitude LFO Depth
        - panel_knob_id: 6
          module_id: 11
          param_id: 3
          curve_type: 0
          min: 1
          max: 0.501
          alias_name: Amplitude LFO Depth
        - panel_knob_id: 6
          module_id: 11
          param_id: 1
          curve_type: 0
          min: 1
          max: 0.503
          alias_name: Amplitude LFO Depth
        - panel_knob_id: 7
          module_id: 5
          param_id: 0
          curve_type: 0
          min: 0.5
          max: 1
          alias_name: Pan LFO Depth
        - panel_knob_id: 7
          module_id: 5
          param_id: 2
          curve_type: 0
          min: 0.5
          max: 1
          alias_name: Pan LFO Depth
        - panel_knob_id: 7
          module_id: 11
          param_id: 2
          curve_type: 0
          min: 0.5
          max: 1
          alias_name: Pan LFO Depth
        - panel_knob_id: 8
          module_id: 16
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
          alias_name: Master Volume
        - panel_knob_id: 8
          module_id: 16
          param_id: 1
          curve_type: 0
          min: 0
          max: 1
          alias_name: Master Volume
        - panel_knob_id: 8
          module_id: 16
          param_id: 2
          curve_type: 0
          min: 0
          max: 1
          alias_name: Master Volume
    - name: ''
      set: []
    - name: ''
      set: []
    - name: ''
      set: []
    - name: ''
      set: []
    - name: ''
      set: []
    - name: ''
      set: []
    - name: ''
      set: []
  midi_maps:
    name: ''
    set: []
  midi_poly_num: 0
  midi_poly_mode: 0
  midi_pitchwheel_range: 1
  mapped_lights: []
  vcvModuleStates:
    - module_id: 4
      data: |-
        {"blockTZFMDC":true,"removePulseDC":true,"limitPW":true,"oversamplingIndex":1}
    - module_id: 9
      data: |-
        {"blockTZFMDC":true,"removePulseDC":true,"limitPW":true,"oversamplingIndex":1}
    - module_id: 14
      data: |-
        {"blockTZFMDC":true,"removePulseDC":true,"limitPW":true,"oversamplingIndex":1}
)";

