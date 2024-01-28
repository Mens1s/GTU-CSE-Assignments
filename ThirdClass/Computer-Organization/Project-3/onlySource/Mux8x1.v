module Mux8x1(
    output reg [31:0] result,
    input [31:0] and32,
    input [31:0] or32,
    input [31:0] xor32,
    input [31:0] nor32,
    input [31:0] LessThan,
    input [31:0] Adder32,
    input [31:0] Subs32,
    input [2:0] sel
);
/*
    wire [0:0] isAnd, isOr, isXor, isNor, isLessThan, isAdder, isSubs;
    wire [31:0] isAnd32, isOr32, isXor32, isNor32, isLessThan32, isAdder32, isSubs32, lessThan, isSubss32;
    wire [31:0] result1, result2, result3, result4, result5, result6, result7, result8, result9, result10, result11, result12, result13, result14;

    OneBit_To_32Bit less1to32(.result(lessThan),.wire1(isLessThan));

    not not1(sel2Not, sel[2]);
    not not2(sel1Not, sel[1]);
    not not3(sel0Not, sel[0]);

    and and01(isSubsS, sel2Not, sel[1], sel[0]);
    and and1(isAnd, sel2Not, sel1Not, sel0Not);
    and and2(isOr, sel2Not, sel1Not, sel[0]);
    and and3(isXor, sel2Not, sel[1], sel0Not);
    and and4(isNor, sel2Not, sel[1], sel[0]);
    and and5(isLessThan, sel[2], sel1Not, sel0Not);
    and and6(isAdder, sel[2], sel1Not, sel[0]);
    and and7(isSubs, sel[2], sel[1], sel0Not);

    OneBit_To_32Bit subs2to32(.result(isSubss32),.wire1(isSubss));
    OneBit_To_32Bit and1to32(.result(isAnd32),.wire1(isAnd));
    OneBit_To_32Bit or1to32(.result(isOr32),.wire1(isOr));
    OneBit_To_32Bit xor1to32(.result(isXor32),.wire1(isXor));
    OneBit_To_32Bit nor1to32(.result(isNor32),.wire1(isNor));
    OneBit_To_32Bit lessThan1to32(.result(isLessThan32),.wire1(isLessThan));
    OneBit_To_32Bit adder1to32(.result(isAdder32),.wire1(isAdder));
    OneBit_To_32Bit subs1to32(.result(isSubs32),.wire1(isSubs));

    And_32Bit subsRess(.out(result14),.a(isSubss32),.b(Subs32));
    And_32Bit andRes(.out(result1),.a(isAnd32),.b(and32));
    And_32Bit orRes(.out(result2),.a(isOr32),.b(or32));
    And_32Bit xorRes(.out(result3),.a(isXor32),.b(xor32));
    And_32Bit norRes(.out(result4),.a(isNor32),.b(nor32));
    And_32Bit lessThanRes(.out(result5),.a(isLessThan32),.b(lessThan));
    And_32Bit adderRes(.out(result6),.a(isAdder32),.b(Adder32));
    And_32Bit subsRes(.out(result7),.a(isSubs32),.b(Subs32));

    Or_32Bit orResult2(.out(result8),.a(result1),.b(result2));
    Or_32Bit orResult3(.out(result9),.a(result3),.b(result4));
    Or_32Bit orResult4(.out(result10),.a(result5),.b(result6));
    Or_32Bit orResult5(.out(result11),.a(result7),.b(result14));

    Or_32Bit orResult6(.out(result12),.a(result8),.b(result9));
    Or_32Bit orResult7(.out(result13),.a(result10),.b(result11));

    Or_32Bit orResult8(.out(result),.a(result12),.b(result13));
*/
    
    always @(*)
    begin
        case(sel)
            3'b000: result = and32;       // Select result1[0] when sel is 000
            3'b001: result = or32;       // Select result1[1] when sel is 001
            3'b010: result = Adder32;         // Select result1[2] when sel is 010
            3'b011: result = Subs32;         // Select result1[3] when sel is 011
            3'b100: result = LessThan;      // Select result1[4] when sel is 100
            3'b101: result = Adder32;       // Select result1[5] when sel is 101
            3'b110: result = Subs32;       // Select result1[5] when sel is 101
            3'b111: result = Adder32;        // Select result1[6] when sel is 110
            default: result = Subs32;        // Default to 0 if sel is invalid
        endcase
    end

endmodule
