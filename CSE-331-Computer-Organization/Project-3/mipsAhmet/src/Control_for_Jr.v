module Control_for_Jr(
    output reg [31:0] goToAddress,
    input [31:0] read_data1,
    input [31:0] jumpAddress,
    input [0:0] isJR
);

/*
    // gate-level implementation
    wire [31:0] result1;
    wire [31:0] result2;
    wire [0:0] notSel;

    not not1(notSel, sel);

    and and1(result1[31], jumpAddress[31], notSel);
    and and2(result1[30], jumpAddress[30], notSel);
    and and3(result1[29], jumpAddress[29], notSel);
    and and4(result1[28], jumpAddress[28], notSel);
    and and5(result1[27], jumpAddress[27], notSel);
    and and6(result1[26], jumpAddress[26], notSel);
    and and7(result1[25], jumpAddress[25], notSel);
    and and8(result1[24], jumpAddress[24], notSel);
    and and9(result1[23], jumpAddress[23], notSel);
    and and10(result1[22], jumpAddress[22], notSel);
    and and11(result1[21], jumpAddress[21], notSel);
    and and12(result1[20], jumpAddress[20], notSel);
    and and13(result1[19], jumpAddress[19], notSel);
    and and14(result1[18], jumpAddress[18], notSel);
    and and15(result1[17], jumpAddress[17], notSel);
    and and16(result1[16], jumpAddress[16], notSel);
    and and17(result1[15], jumpAddress[15], notSel);
    and and18(result1[14], jumpAddress[14], notSel);
    and and19(result1[13], jumpAddress[13], notSel);
    and and20(result1[12], jumpAddress[12], notSel);
    and and21(result1[11], jumpAddress[11], notSel);
    and and22(result1[10], jumpAddress[10], notSel);
    and and23(result1[9], jumpAddress[9], notSel);
    and and24(result1[8], jumpAddress[8], notSel);
    and and25(result1[7], jumpAddress[7], notSel);
    and and26(result1[6], jumpAddress[6], notSel);
    and and27(result1[5], jumpAddress[5], notSel);
    and and28(result1[4], jumpAddress[4], notSel);
    and and29(result1[3], jumpAddress[3], notSel);
    and and30(result1[2], jumpAddress[2], notSel);
    and and31(result1[1], jumpAddress[1], notSel);
    and and32(result1[0], jumpAddress[0], notSel);

    and and33(result2[31], read_data1[31], sel);
    and and34(result2[30], read_data1[30], sel);
    and and35(result2[29], read_data1[29], sel);
    and and36(result2[28], read_data1[28], sel);
    and and37(result2[27], read_data1[27], sel);
    and and38(result2[26], read_data1[26], sel);
    and and39(result2[25], read_data1[25], sel);
    and and40(result2[24], read_data1[24], sel);
    and and41(result2[23], read_data1[23], sel);
    and and42(result2[22], read_data1[22], sel);
    and and43(result2[21], read_data1[21], sel);
    and and44(result2[20], read_data1[20], sel);
    and and45(result2[19], read_data1[19], sel);
    and and46(result2[18], read_data1[18], sel);
    and and47(result2[17], read_data1[17], sel);
    and and48(result2[16], read_data1[16], sel);
    and and49(result2[15], read_data1[15], sel);
    and and50(result2[14], read_data1[14], sel);
    and and51(result2[13], read_data1[13], sel);
    and and52(result2[12], read_data1[12], sel);
    and and53(result2[11], read_data1[11], sel);
    and and54(result2[10], read_data1[10], sel);
    and and55(result2[9], read_data1[9], sel);
    and and56(result2[8], read_data1[8], sel);
    and and57(result2[7], read_data1[7], sel);
    and and58(result2[6], read_data1[6], sel);
    and and59(result2[5], read_data1[5], sel);
    and and60(result2[4], read_data1[4], sel);
    and and61(result2[3], read_data1[3], sel);
    and and62(result2[2], read_data1[2], sel);
    and and63(result2[1], read_data1[1], sel);
    and and64(result2[0], read_data1[0], sel);

    Or_32Bit or1(goToAddress, result1, result2);

    */
    always @(*)
    begin
        if(isJR == 1'b1)
        begin
            goToAddress = read_data1;
        end
        else
        begin
            goToAddress = jumpAddress;
        end
    end


endmodule