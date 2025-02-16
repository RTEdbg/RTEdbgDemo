#ifndef RTE_SIMPLE_DEMO_FMT_H
#define RTE_SIMPLE_DEMO_FMT_H
/* simple_demo_fmt.h */

// MSG0_SIMPLE_TEST_START "simple_demo() function started"

// EXT_MSG1_3_DEMO_EXT_MSG "data1 = %u, sequence = %u (%[-32:32u]{standby|start|stop|iddle}Y)"

// MSG3_COMBINED_DEMO "data1 = %#10X, data2 = %g, data3 = %g"

// MEMO(M_TICK)
// FILTER(F_BATTERY_DATA)

// FILTER(F_SINCOS_DEMO)

// OUT_FILE(BATTERY_DATA, "Battery_data.csv", "w", "Ubat[V];Ibat [A];Charged Ah;discharged Ah;Temperature #1; Temperature #2; SOC[%]\n")
// OUT_FILE(SIN_COS, "sin_cos.csv", "w", "MSG_No;Signal1;Signal2;Calc_time[us]\n")

// MSG0_START_SIN_COS "Sin_cos_demo() function started"

// MSG2_SINCOS_DEMO "%|Sin|g, %|Cos|g, %[t-MSG0_START_SIN_COS](*1e6)|Time used to calculate sin/cos [us]|g us"
// >SIN_COS "%[N]u;%g;%g; %[t-MSG0_START_SIN_COS](*1e6)g\n"

// MSGN3_BATT_DEMO "Hex: %4H"
// "\n  Battery voltage = %[16u](*.01).2f V, current = %[16i](*.01).2f, "
// "\n  Charged Ah = %[16](*.1).1f, discharged Ah = %[16](*0.1).1f, "
// "\n  Temperatures: %[8u](-100*.5).1f, %[8u](-100*.5).1f, SOC: %[16](*.1).1f"
// >BATTERY_DATA "%[16u](*.01).2f;%[16i](*0.01).2f;%[16](*.1).1f;%[16](*0.1).1f;"
// >BATTERY_DATA "%[8u](-100*.5).1f;%[8u](-100*.5).1f;%[16](*.1).1f\n"

// MSG1_UWTICK "Tick %<M_TICK>u"

// MSG3_COMBINED "Combined values demo: \n"
//                "   value1=%g, value2=%g, value3=%[16i]d, value4=%[6u]u, value5=%[10i]d"

// MSG0_TRIGGER_SYS_ERROR
//  "\n  --> [Triggering a system error]-[触发系统错误]\n"

#endif
