module Mux8x1(
    output reg [31:0] result,
    input [31:0] and32,
    input [31:0] or32,
    input [31:0] xor32,
    input [31:0] nor32,
    input [31:0] LessThan,
    input [31:0] Adder32,
    input [31:0] Subs32,
    input [31:0] Mod,
    input [2:0] sel
);

    always @(*)
    begin
        case(sel)
            3'b000: result = and32; // Select result1[0] when sel is 000
            3'b001: result = or32; // Select result1[1] when sel is 001
            3'b010: result = xor32; // Select result1[2] when sel is 010
            3'b011: result = nor32; // Select result1[3] when sel is 011
            3'b100: result = LessThan; // Select result1[4] when sel is 100
            3'b101: result = Adder32; // Select result1[5] when sel is 101
            3'b110: result = Subs32; // Select result1[6] when sel is 110
            3'b111: result = Mod; // Select result1[7] when sel is 111
            default: result = 32'b0; // Default to 0 if sel is invalid
        endcase
    end

endmodule
