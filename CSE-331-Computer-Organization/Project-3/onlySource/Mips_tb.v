// iverilog -o Mips Mips.v Mips_tb.v Mux2x1_5byte.v Mux2x1.v Sign_extend.v Shift_left_2.v Register_block.v Memory_block.v Instruction_block.v Control_unit.v Alu_control.v Alu.v Add_32Bit.v And_32Bit.v LessThan_32Bit.v Mux8x1.v Nor_32Bit.v Or_32Bit.v Substract_32Bit.v Xor_32Bit.v FullAdder_32Bit.v FullAdder_16Bit_CLA.v FullAdder_4Bit_CLA.v Bus_for_jump.v Control_for_Jr.v OneBit_To_32Bit.v
`timescale 1ns/1ps

module Mips_tb;

  reg clock;
  reg [31:0] instruction;
  
  // Instantiate MIPS module
  Mips dut (
    .clock(clock)
  );

  // Clock generation
  always begin
    #5 clock = ~clock;
  end

  // Initial block
  initial begin
    // Initialize clock
    clock = 0;

    // Apply reset
    // Add any additional initializations if needed

    // Wait for a few clock cycles
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;
    #5;



    // Test case 1: Load a value into a register, add 5, and check the result



    $finish;
  end

endmodule
