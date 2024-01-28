module FullAdder_32Bit(input [31:0] a, input [31:0] b, input cin ,output [31:0] sum, output carry); 
// it uses full adder 4 bit cla so it contains 2-Level Carry Look Ahead Adder 
    wire [7:0] c; // Carry signals from each stage

    FullAdder_16Bit_CLA u0(a[15:0], b[15:0], cin, sum[15:0], c[3:0]);
    FullAdder_16Bit_CLA u1(a[31:16], b[31:16], c[3], sum[31:16], c[7:4]);
    
    or or0(carry, c[7], 0);

endmodule