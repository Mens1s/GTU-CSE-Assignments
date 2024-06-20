transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Alu_control.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Alu.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/And_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Bus_for_jump.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Control_for_Jr.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Control_unit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/FullAdder_4Bit_CLA.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/FullAdder_16Bit_CLA.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/FullAdder_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/LessThan_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Mux2x1_5byte.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Mux2x1.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Mux8x1.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Nor_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Or_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Shift_left_2.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Sign_extend.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Substract_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Xor_32Bit.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Mips.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Instruction_block.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Memory_block.v}
vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Register_block.v}

vlog -vlog01compat -work work +incdir+C:/mipsAhmet/src {C:/mipsAhmet/src/Mips_tb.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  Mips_tb

add wave *
view structure
view signals
run -all
