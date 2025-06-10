<Qucs Schematic 24.4.1>
<Properties>
  <View=-186,-2,2261,1559,1.00322,0,309>
  <Grid=10,10,1>
  <DataSet=codec_in.dat>
  <DataDisplay=codec_in.dpl>
  <OpenDisplay=0>
  <Script=codec_in.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <GND * 1 970 800 0 0 0 0>
  <Vac V4 1 700 730 18 -26 0 1 "1 V" 1 "1 kHz" 0 "0" 0 "0" 0 "0" 0 "0" 0>
  <R R1 1 970 610 15 -26 0 1 "10 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "US" 0>
  <.TR TR1 1 180 290 0 70 0 0 "lin" 1 "0 ms" 1 "10 ms" 1 "1001" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
  <R R4 1 620 580 -26 -55 0 2 "boof_R_ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <C C1 1 860 580 -26 17 0 0 "bypass_caps_F" 1 "" 0 "neutral" 0>
  <GND * 1 700 800 0 0 0 0>
  <GND * 1 550 800 0 0 0 0>
  <GND * 1 1490 800 0 0 0 0>
  <Vdc V3 1 550 730 18 -26 0 1 "1 V" 1>
  <Vdc V5 1 1490 730 18 -26 0 1 "1 V" 1>
  <R R2 1 970 730 15 -26 0 1 "10 kOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "US" 0>
  <R R6 1 1310 670 -26 15 0 0 "codec_input_Z" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <C C3 1 700 640 17 -26 0 1 "AC_cap_f" 1 "" 0 "neutral" 0>
  <C C4 1 1150 670 -26 17 0 0 "bypass_caps_F" 1 "1 V" 1 "neutral" 0>
  <SpicePar Parameters 1 170 750 -28 18 0 0 "bypass_caps_F=1e-6 " 1 "codec_input_Z=29e3" 1 "AC_cap_f=10e-6 " 1 "boof_R_ohm=10e3" 1>
  <NutmegEq Sim_calcs 1 160 500 -30 18 0 0 "TR1" 1 "offset=2" 1 "AC_diff=tran.v(ac_out) -tran.v(dc_removed)" 1 "input_bias_diff=tran.v(input) - tran.v(ac_out) - 1" 1 "div_difference=tran.v(dc_removed) - tran.v(div)*2" 1 "output_bias_diff=tran.v(output)- 1- tran.v(div)" 1>
  <.AC AC1 1 160 920 0 41 0 0 "log" 1 "20 Hz" 1 "20 kHz" 1 "2020" 1 "no" 0>
  <NutmegEq NutmegEq1 1 -110 960 -30 18 0 0 "AC1" 1 "test=ac.v(ac_out)/ac.v(dc_removed)" 1>
</Components>
<Wires>
  <890 580 970 580 "dc_removed" 960 540 56 "">
  <650 580 700 580 "" 0 0 0 "">
  <700 760 700 800 "" 0 0 0 "">
  <550 760 550 800 "" 0 0 0 "">
  <550 580 590 580 "" 0 0 0 "">
  <550 580 550 700 "" 0 0 0 "">
  <1490 760 1490 800 "" 0 0 0 "">
  <970 760 970 800 "" 0 0 0 "">
  <970 640 970 670 "" 0 0 0 "">
  <1340 670 1490 670 "" 0 0 0 "">
  <1490 670 1490 700 "" 0 0 0 "">
  <970 670 970 700 "" 0 0 0 "">
  <1180 670 1280 670 "output" 1320 610 100 "">
  <970 670 1120 670 "div" 1010 650 12 "">
  <700 580 830 580 "input" 800 520 90 "">
  <700 580 700 610 "" 0 0 0 "">
  <700 670 700 700 "" 0 0 0 "">
  <700 700 700 700 "AC_out" 760 690 0 "">
</Wires>
<Diagrams>
  <Rect 440 450 240 160 3 #c0c0c0 1 00 1 0 0.0005 0.002 1 -1.20496 1 1.25483 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.v(input)" #0000ff 0 3 0 0 0>
	  <Mkr 0.0020028 173 -200 3 0 0>
	<"ngspice/tran.v(ac_out)" #ff0000 0 3 0 0 0>
  </Rect>
  <Rect 440 210 240 160 3 #c0c0c0 1 00 1 0 0.0005 0.002 1 -0.00522535 0.002 0.000632135 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.ac_diff" #ff0000 0 3 0 0 0>
  </Rect>
  <Rect 760 210 240 160 3 #c0c0c0 1 00 1 0 0.002 0.01 1 -0.006 0.002 0.00128219 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.input_bias_diff" #ff0000 0 3 0 0 0>
  </Rect>
  <Rect 760 440 240 160 3 #c0c0c0 1 00 1 0 0.0005 0.002 1 -1.2176 1 1.2016 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.v(ac_out)" #0000ff 0 3 0 0 0>
	<"ngspice/tran.v(dc_removed)" #ff0000 0 3 0 0 0>
  </Rect>
  <Rect 1110 440 240 160 3 #c0c0c0 1 00 1 0 0.0005 0.002 1 -1.64423 5 5.96808 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.v(div)" #0000ff 0 3 0 0 0>
	  <Mkr 0.0020028 170 36 3 0 0>
	<"ngspice/tran.v(dc_removed)" #ff0000 0 3 0 0 0>
	  <Mkr 0.0020028 249 -176 3 0 0>
  </Rect>
  <Rect 1110 210 240 160 3 #c0c0c0 1 00 1 0 0.002 0.01 1 -0.2 0.2 0.2 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.div_difference" #0000ff 0 3 0 0 0>
  </Rect>
  <Rect 1550 200 240 160 3 #c0c0c0 1 00 1 0 0.001 0.005 1 -0.595694 0.5 0.604701 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.output_bias_diff" #ff00ff 0 3 0 0 0>
	  <Mkr 0.0020028 244 -200 3 0 0>
  </Rect>
  <Rect 1550 480 240 160 3 #c0c0c0 1 00 1 0 0.002 0.01 1 -1 1 1.69857 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/tran.v(div)" #0000ff 0 3 0 0 0>
	  <Mkr 0.0062528 279 -116 3 0 0>
	<"ngspice/tran.v(output)" #ff0000 0 3 0 0 0>
	  <Mkr 0.0062528 269 -207 3 0 0>
	<"ngspice/tran.output_bias_diff" #ff00ff 0 3 0 0 0>
  </Rect>
  <Rect 130 1280 240 160 3 #c0c0c0 1 10 1 10 1 100000 1 6298.25 200 7000 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/ac.v(z)" #0000ff 0 3 0 0 0>
  </Rect>
  <Rect 510 1440 240 160 3 #c0c0c0 1 10 1 10 1 100000 1 0.986313 0.1 1.15056 1 -1 1 1 315 0 225 1 0 0 "" "" "">
	<"ngspice/ac.test" #0000ff 0 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
  <Text 1800 780 11 #000000 0 "29 kΩ is from the datasheet here: \nhttps://www.nxp.com/docs/en/data-sheet/SGTL5000.pdf#page=7">
</Paintings>
