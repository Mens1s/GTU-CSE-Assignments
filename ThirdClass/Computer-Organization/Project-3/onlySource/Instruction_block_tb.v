// iverilog -o Instruction_block Instruction_block.v Instruction_block_tb.v 
`timescale 1ns/1ns  // Adjust the timescale as needed

module Instruction_block_tb;

    reg [31:0] pc;
    wire [31:0] instruction;

    // Instantiate the module under test
    Instruction_block uut (
        .instruction(instruction),
        .pc(pc)
    );

    // Initial stimulus
    initial begin

        // Test case 1
        pc = 0;
        #10; // Wait for a few simulation cycles
        // write
        $display("pc = %b, instruction = %b", pc, instruction);
        // Test case 2
        pc = 4;
        #10; // Wait for a few simulation cycles
        $display("pc = %b, instruction = %b", pc, instruction);

        // Add more test cases as needed

        // Terminate simulation
        $finish;
    end

endmodule
