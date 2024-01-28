module Mux2x1(
    output reg [31:0] result,
    input [31:0] wire1,
    input [31:0] wire2,
    input [0:0] sel
);
/*
    // gate-level implementation
    wire [31:0] result1;
    wire [31:0] result2;
    wire [0:0] notSel;

    not not1(notSel, sel);

    and and1(result1[31], wire1[31], notSel);
    and and2(result1[30], wire1[30], notSel);
    and and3(result1[29], wire1[29], notSel);
    and and4(result1[28], wire1[28], notSel);
    and and5(result1[27], wire1[27], notSel);
    and and6(result1[26], wire1[26], notSel);
    and and7(result1[25], wire1[25], notSel);
    and and8(result1[24], wire1[24], notSel);
    and and9(result1[23], wire1[23], notSel);
    and and10(result1[22], wire1[22], notSel);
    and and11(result1[21], wire1[21], notSel);
    and and12(result1[20], wire1[20], notSel);
    and and13(result1[19], wire1[19], notSel);
    and and14(result1[18], wire1[18], notSel);
    and and15(result1[17], wire1[17], notSel);
    and and16(result1[16], wire1[16], notSel);
    and and17(result1[15], wire1[15], notSel);
    and and18(result1[14], wire1[14], notSel);
    and and19(result1[13], wire1[13], notSel);
    and and20(result1[12], wire1[12], notSel);
    and and21(result1[11], wire1[11], notSel);
    and and22(result1[10], wire1[10], notSel);
    and and23(result1[9], wire1[9], notSel);
    and and24(result1[8], wire1[8], notSel);
    and and25(result1[7], wire1[7], notSel);
    and and26(result1[6], wire1[6], notSel);
    and and27(result1[5], wire1[5], notSel);
    and and28(result1[4], wire1[4], notSel);
    and and29(result1[3], wire1[3], notSel);
    and and30(result1[2], wire1[2], notSel);
    and and31(result1[1], wire1[1], notSel);
    and and32(result1[0], wire1[0], notSel);

    and and33(result2[31], wire2[31], sel);
    and and34(result2[30], wire2[30], sel);
    and and35(result2[29], wire2[29], sel);
    and and36(result2[28], wire2[28], sel);
    and and37(result2[27], wire2[27], sel);
    and and38(result2[26], wire2[26], sel);
    and and39(result2[25], wire2[25], sel);
    and and40(result2[24], wire2[24], sel);
    and and41(result2[23], wire2[23], sel);
    and and42(result2[22], wire2[22], sel);
    and and43(result2[21], wire2[21], sel);
    and and44(result2[20], wire2[20], sel);
    and and45(result2[19], wire2[19], sel);
    and and46(result2[18], wire2[18], sel);
    and and47(result2[17], wire2[17], sel);
    and and48(result2[16], wire2[16], sel);
    and and49(result2[15], wire2[15], sel);
    and and50(result2[14], wire2[14], sel);
    and and51(result2[13], wire2[13], sel);
    and and52(result2[12], wire2[12], sel);
    and and53(result2[11], wire2[11], sel);
    and and54(result2[10], wire2[10], sel);
    and and55(result2[9], wire2[9], sel);
    and and56(result2[8], wire2[8], sel);
    and and57(result2[7], wire2[7], sel);
    and and58(result2[6], wire2[6], sel);
    and and59(result2[5], wire2[5], sel);
    and and60(result2[4], wire2[4], sel);
    and and61(result2[3], wire2[3], sel);
    and and62(result2[2], wire2[2], sel);
    and and63(result2[1], wire2[1], sel);
    and and64(result2[0], wire2[0], sel);

    Or_32Bit or1(result, result1, result2);

    */
    always @(*)
    begin
        case(sel)
            1'b0: result = wire1;     // Select result1[0] when sel is 000
            1'b1: result = wire2;      // Select result1[1] when sel is 001
            default: result = 32'b0;    // Default to 0 if sel is invalid   
        endcase
    end
    

endmodule
