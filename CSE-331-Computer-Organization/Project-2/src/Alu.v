module Alu (
    output reg [31:0] result,
    input [31:0] a,
    input [31:0] b,
    input [2:0] sel,
    input c_in,
    output c_out,
    input start,
    input reset,
    input clk,
    output done);

    reg [7:0] results;
    wire [31:0] and_result, or_result, xor_result, nor_result, lt_result, add_result ,sub_result, mod_result;
    wire [31:0] mux_result;

    And_32Bit and1 (and_result, a, b);
    Or_32Bit or1 (or_result, a, b);
    Xor_32Bit xor1 (xor_result, a, b);
    Nor_32Bit nor1 (nor_result, a, b);
    LessThan_32Bit lt1 (.LessThan(lt_result), .A(a), .B(b));
    FullAdder_32Bit fa1 (.sum(add_result), .carry(c_out), .a(a), .b(b), .cin(c_in));
    Substract_32Bit sub1 (.Result(sub_result), .A(a), .B(b), .Cout(c_outt));

    Mod_32Bit mod1 (
        .clk(clk), 
        .rst(reset), 
        .start(start),
        .a(a),
        .b(b),
        .result(mod_result),
        .done(done) 
    );

    always @(*)
    begin
        result = mux_result;
    end

	Mux8x1 mux8x1 (.result(mux_result),
        .and32(and_result),
        .or32(or_result),
        .xor32(xor_result),
        .nor32(nor_result),
        .LessThan(lt_result),
        .Adder32(add_result),
        .Subs32(sub_result),
        .Mod(mod_result),
        .sel(sel)
    );
endmodule
