module Mux2x1_5byte(
    output reg [4:0] result,
    input [4:0] wire1,
    input [4:0] wire2,
    input [0:0] sel
);
/*
    // gate level implementation
    wire [4:0] result1;
    wire [4:0] result2;
    wire [0:0] notSel;

    not not1(notSel, sel);

    and and28(result1[4], wire1[4], notSel);
    and and29(result1[3], wire1[3], notSel);
    and and30(result1[2], wire1[2], notSel);
    and and31(result1[1], wire1[1], notSel);
    and and32(result1[0], wire1[0], notSel);

    and and60(result2[4], wire2[4], sel);
    and and61(result2[3], wire2[3], sel);
    and and62(result2[2], wire2[2], sel);
    and and63(result2[1], wire2[1], sel);
    and and64(result2[0], wire2[0], sel);

    or or1(result[4], result1[4], result2[4]);
    or or2(result[3], result1[3], result2[3]);
    or or3(result[2], result1[2], result2[2]);
    or or4(result[1], result1[1], result2[1]);
    or or5(result[0], result1[0], result2[0]);
    */
    always @(*)
    begin
        case(sel)
            1'b0: result = wire1;     // Select result1[0] when sel is 000
            1'b1: result = wire2;      // Select result1[1] when sel is 001
            default: result = 5'b0;    // Default to 0 if sel is invalid   
        endcase
    end
    

endmodule
