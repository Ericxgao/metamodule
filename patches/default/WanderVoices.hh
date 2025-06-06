static const char WanderVoices_patch[] =
	R"(
PatchData:
  patch_name: Wander Voices
  description: Patch Description
  module_slugs:
    0: HubMedium
    1: Plaits
    2: EnOsc
    3: StMix
    4: DualAtenuverter
    5: Kickall
    6: DEV
    7: 4Seq
    8: 4Seq
    9: QCD
    10: MultiLFO
  int_cables:
    - out:
        module_id: 5
        jack_id: 0
      ins:
        - module_id: 3
          jack_id: 4
      color: 13501
    - out:
        module_id: 2
        jack_id: 0
      ins:
        - module_id: 6
          jack_id: 10
      color: 13501
    - out:
        module_id: 6
        jack_id: 6
      ins:
        - module_id: 3
          jack_id: 2
      color: 1453
    - out:
        module_id: 9
        jack_id: 7
      ins:
        - module_id: 9
          jack_id: 5
      color: 13501
    - out:
        module_id: 9
        jack_id: 2
      ins:
        - module_id: 7
          jack_id: 0
        - module_id: 6
          jack_id: 3
      color: 35921
    - out:
        module_id: 9
        jack_id: 6
      ins:
        - module_id: 8
          jack_id: 0
      color: 35419
    - out:
        module_id: 8
        jack_id: 0
      ins:
        - module_id: 6
          jack_id: 6
      color: 64934
    - out:
        module_id: 8
        jack_id: 5
      ins:
        - module_id: 2
          jack_id: 5
      color: 61865
    - out:
        module_id: 9
        jack_id: 8
      ins:
        - module_id: 9
          jack_id: 9
      color: 64934
    - out:
        module_id: 9
        jack_id: 3
      ins:
        - module_id: 5
          jack_id: 0
      color: 35921
    - out:
        module_id: 8
        jack_id: 2
      ins:
        - module_id: 7
          jack_id: 1
      color: 35419
    - out:
        module_id: 1
        jack_id: 0
      ins:
        - module_id: 6
          jack_id: 7
      color: 35921
    - out:
        module_id: 6
        jack_id: 3
      ins:
        - module_id: 3
          jack_id: 0
      color: 35419
    - out:
        module_id: 7
        jack_id: 5
      ins:
        - module_id: 1
          jack_id: 7
      color: 1453
    - out:
        module_id: 7
        jack_id: 1
      ins:
        - module_id: 8
          jack_id: 1
      color: 13501
    - out:
        module_id: 7
        jack_id: 3
      ins:
        - module_id: 9
          jack_id: 1
      color: 35921
    - out:
        module_id: 8
        jack_id: 3
      ins:
        - module_id: 6
          jack_id: 2
      color: 35419
    - out:
        module_id: 6
        jack_id: 2
      ins:
        - module_id: 4
          jack_id: 1
      color: 13501
    - out:
        module_id: 4
        jack_id: 1
      ins:
        - module_id: 2
          jack_id: 8
      color: 61865
    - out:
        module_id: 1
        jack_id: 1
      ins:
        - module_id: 4
          jack_id: 0
      color: 64934
    - out:
        module_id: 4
        jack_id: 0
      ins:
        - module_id: 5
          jack_id: 2
      color: 1453
    - out:
        module_id: 10
        jack_id: 1
      ins:
        - module_id: 9
          jack_id: 2
      color: 61865
  mapped_ins: []
  mapped_outs:
    - panel_jack_id: 0
      out:
        module_id: 3
        jack_id: 0
    - panel_jack_id: 1
      out:
        module_id: 3
        jack_id: 1
    - panel_jack_id: 4
      out:
        module_id: 10
        jack_id: 1
  static_knobs:
    - module_id: 1
      param_id: 0
      value: 0
    - module_id: 1
      param_id: 1
      value: 0.391566
    - module_id: 1
      param_id: 2
      value: 0.375
    - module_id: 1
      param_id: 3
      value: 0.606024
    - module_id: 1
      param_id: 4
      value: 0.26747
    - module_id: 1
      param_id: 5
      value: 0.291567
    - module_id: 1
      param_id: 6
      value: 0.53012
    - module_id: 1
      param_id: 7
      value: 0.5
    - module_id: 1
      param_id: 8
      value: 0.825333
    - module_id: 1
      param_id: 9
      value: 0.5
    - module_id: 1
      param_id: 10
      value: 0.5
    - module_id: 2
      param_id: 0
      value: 0.139759
    - module_id: 2
      param_id: 1
      value: 0.147518
    - module_id: 2
      param_id: 2
      value: 0.5
    - module_id: 2
      param_id: 3
      value: 0.392771
    - module_id: 2
      param_id: 4
      value: 0.33735
    - module_id: 2
      param_id: 5
      value: 0
    - module_id: 2
      param_id: 6
      value: 0.0373496
    - module_id: 2
      param_id: 7
      value: 0
    - module_id: 2
      param_id: 8
      value: 0.149398
    - module_id: 2
      param_id: 9
      value: 1
    - module_id: 2
      param_id: 10
      value: 1
    - module_id: 2
      param_id: 11
      value: 0
    - module_id: 2
      param_id: 12
      value: 1
    - module_id: 2
      param_id: 13
      value: 0
    - module_id: 2
      param_id: 14
      value: 0
    - module_id: 2
      param_id: 15
      value: 0.511999
    - module_id: 2
      param_id: 16
      value: 1
    - module_id: 2
      param_id: 17
      value: 0.5
    - module_id: 2
      param_id: 18
      value: 0
    - module_id: 2
      param_id: 19
      value: 0
    - module_id: 3
      param_id: 0
      value: 0.840964
    - module_id: 3
      param_id: 1
      value: 0.885542
    - module_id: 3
      param_id: 2
      value: 0.449397
    - module_id: 3
      param_id: 3
      value: 0.5
    - module_id: 3
      param_id: 4
      value: 0.366265
    - module_id: 3
      param_id: 5
      value: 0.604819
    - module_id: 3
      param_id: 6
      value: 0.490362
    - module_id: 3
      param_id: 7
      value: 0.5
    - module_id: 4
      param_id: 0
      value: 0.501001
    - module_id: 4
      param_id: 1
      value: 0.5
    - module_id: 4
      param_id: 2
      value: 0.494
    - module_id: 4
      param_id: 3
      value: 0.5
    - module_id: 5
      param_id: 0
      value: 0.5
    - module_id: 5
      param_id: 1
      value: 0
    - module_id: 5
      param_id: 2
      value: 0.204819
    - module_id: 5
      param_id: 3
      value: 0.389157
    - module_id: 5
      param_id: 4
      value: 0.226611
    - module_id: 5
      param_id: 5
      value: 0.17875
    - module_id: 6
      param_id: 0
      value: 0
    - module_id: 6
      param_id: 1
      value: 0.26747
    - module_id: 6
      param_id: 2
      value: 0
    - module_id: 6
      param_id: 3
      value: 0.5
    - module_id: 6
      param_id: 4
      value: 0.589157
    - module_id: 6
      param_id: 5
      value: 0.43253
    - module_id: 6
      param_id: 6
      value: 0.5
    - module_id: 6
      param_id: 7
      value: 0.5
    - module_id: 6
      param_id: 8
      value: 0.5
    - module_id: 6
      param_id: 9
      value: 0.5
    - module_id: 6
      param_id: 10
      value: 0.53494
    - module_id: 6
      param_id: 11
      value: 0.595181
    - module_id: 6
      param_id: 12
      value: 0.5
    - module_id: 6
      param_id: 13
      value: 0.5
    - module_id: 6
      param_id: 14
      value: 0.5
    - module_id: 6
      param_id: 15
      value: 0.5
    - module_id: 6
      param_id: 16
      value: 0
    - module_id: 6
      param_id: 17
      value: 0
    - module_id: 7
      param_id: 0
      value: 0.2
    - module_id: 7
      param_id: 1
      value: 0.26696
    - module_id: 7
      param_id: 2
      value: 0
    - module_id: 7
      param_id: 3
      value: 0.23784
    - module_id: 8
      param_id: 0
      value: 0.424096
    - module_id: 8
      param_id: 1
      value: 0.616868
    - module_id: 8
      param_id: 2
      value: 0.342169
    - module_id: 8
      param_id: 3
      value: 0.403615
    - module_id: 9
      param_id: 0
      value: 0.5
    - module_id: 9
      param_id: 1
      value: 0.5
    - module_id: 9
      param_id: 2
      value: 0.446988
    - module_id: 9
      param_id: 3
      value: 0.512048
    - module_id: 9
      param_id: 4
      value: 0.5
    - module_id: 9
      param_id: 5
      value: 0.5
    - module_id: 9
      param_id: 6
      value: 0.603615
    - module_id: 9
      param_id: 7
      value: 0.398795
    - module_id: 9
      param_id: 8
      value: 0.5
    - module_id: 9
      param_id: 9
      value: 0.5
    - module_id: 9
      param_id: 10
      value: 0.439759
    - module_id: 9
      param_id: 11
      value: 0.545783
    - module_id: 9
      param_id: 12
      value: 0.5
    - module_id: 9
      param_id: 13
      value: 0.50241
    - module_id: 9
      param_id: 14
      value: 0.5
    - module_id: 9
      param_id: 15
      value: 0.295181
    - module_id: 9
      param_id: 16
      value: 0
    - module_id: 9
      param_id: 17
      value: 0
    - module_id: 9
      param_id: 18
      value: 1
    - module_id: 9
      param_id: 19
      value: 0.5
    - module_id: 9
      param_id: 20
      value: 0
    - module_id: 10
      param_id: 0
      value: 0.231518
    - module_id: 10
      param_id: 1
      value: 0.5
    - module_id: 10
      param_id: 2
      value: 0.500141
  mapped_knobs:
    - name: Voices
      set:
        - panel_knob_id: 0
          module_id: 1
          param_id: 4
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 0
          module_id: 6
          param_id: 1
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 1
          module_id: 4
          param_id: 2
          curve_type: 0
          min: 0.494
          max: 1
        - panel_knob_id: 2
          module_id: 2
          param_id: 1
          curve_type: 0
          min: 0.061
          max: 0.491
        - panel_knob_id: 3
          module_id: 2
          param_id: 5
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 4
          module_id: 1
          param_id: 5
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 5
          module_id: 5
          param_id: 3
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 6
          module_id: 10
          param_id: 0
          curve_type: 0
          min: 0.063
          max: 0.418
        - panel_knob_id: 11
          module_id: 4
          param_id: 1
          curve_type: 0
          min: 0.5
          max: 0.796
        - panel_knob_id: 11
          module_id: 4
          param_id: 0
          curve_type: 0
          min: 0.501001
          max: 1
    - name: Levels
      set:
        - panel_knob_id: 0
          module_id: 3
          param_id: 0
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 1
          module_id: 3
          param_id: 1
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 2
          module_id: 3
          param_id: 2
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 3
          module_id: 3
          param_id: 4
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 4
          module_id: 3
          param_id: 5
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 5
          module_id: 3
          param_id: 6
          curve_type: 0
          min: 0
          max: 1
        - panel_knob_id: 6
          module_id: 10
          param_id: 0
          curve_type: 0
          min: 0
          max: 0.418
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
  vcvModuleStates:
    - module_id: 1
      data: |-
        {"lowCpu":false,"model":7}

)";
