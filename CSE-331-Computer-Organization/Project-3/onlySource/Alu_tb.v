// iverilog -o Alu Alu.v Alu_tb.v Add_32Bit.v And_32Bit.v LessThan_32Bit.v Mux8x1.v Nor_32Bit.v Or_32Bit.v Substract_32Bit.v Xor_32Bit.v FullAdder_32Bit.v FullAdder_16Bit_CLA.v FullAdder_4Bit_CLA.v 
// vvp Alu
`timescale 1ns/1ns

module Alu_tb;

    reg [31:0] alu_src1, alu_src2;
    reg [2:0] alu_ctr;
    wire [31:0] alu_result;
    wire zero_bit;

    // Instantiate the module under test
    Alu uut (
        .alu_result(alu_result),
        .alu_src1(alu_src1),
        .alu_src2(alu_src2),
        .alu_ctr(alu_ctr),
        .zero_bit(zero_bit)
    );

    // Clock generation
    reg clk;
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // Testbench stimulus
    initial begin
        // Test case 1: AND operation
        alu_src1 = 32'hAAAA5555;
        alu_src2 = 32'h00FF00FF;
        alu_ctr = 3'b000;
        #10;
        $display("%b\n%b\n%b\n-----",alu_src1, alu_src2, alu_result);

        // Test case 2: OR operation
        alu_src1 = 32'hAAAA5555;
        alu_src2 = 32'h00FF00FF;
        alu_ctr = 3'b001;
        #10;
        $display("%b\n%b\n%b\n-----",alu_src1, alu_src2, alu_result);

        // Test case 3: XOR operation
        alu_src1 = 32'hAAAA5555;
        alu_src2 = 32'h00FF00FF;
        alu_ctr = 3'b010;
        #10;
        $display("%b\n%b\n%b\n-----",alu_src1, alu_src2, alu_result);

        // Add more test cases based on your specific operations
        // Test case 4: FullAdder operation

        alu_src1 = 32'h11000011;
        alu_src2 = 32'h10000011;
        alu_ctr = 3'b101;
        #10;
        $display("%b\n%b\n%b\n-----",alu_src1, alu_src2, alu_result);


        // Terminate simulation

        #10;
        $finish;
    end

endmodule
