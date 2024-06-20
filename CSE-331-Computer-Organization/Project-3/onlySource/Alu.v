module Alu (
    output reg [31:0] alu_result,
    input [31:0] alu_src1,
    input [31:0] alu_src2,
    input [2:0] alu_ctr,
    output reg zero_bit
    );    
    
    wire c_out;
    reg [7:0] results;
    wire [31:0] and_result, or_result, xor_result, nor_result, lt_result, add_result ,sub_result;
    wire [31:0] mux_result;

    And_32Bit and1 (and_result, alu_src1, alu_src2);
    Or_32Bit or1 (or_result, alu_src1, alu_src2);
    Xor_32Bit xor1 (xor_result, alu_src1, alu_src2);
    Nor_32Bit nor1 (nor_result, alu_src1, alu_src2);
    LessThan_32Bit lt1 (.LessThan(lt_result), .A(alu_src1), .B(alu_src2));
    FullAdder_32Bit fa1 (.sum(add_result), .carry(c_out), .a(alu_src1), .b(alu_src2), .cin(1'b0));
    Substract_32Bit sub1 (.Result(sub_result), .A(alu_src1), .B(alu_src2), .Cout(c_out));


	Mux8x1 mux8x1 (.result(mux_result),
        .and32(and_result),
        .or32(or_result),
        .xor32(xor_result),
        .nor32(nor_result),
        .LessThan(lt_result),
        .Adder32(add_result),
        .Subs32(sub_result),
        .sel(alu_ctr)
    );


    always @*
    
        begin
            alu_result = mux_result;
            zero_bit = (sub_result == 32'b0);
        end
endmodule
