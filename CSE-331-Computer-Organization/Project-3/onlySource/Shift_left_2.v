module Shift_left_2 (
    output [31:0] shifted_address,
    input [31:0] address
);

    // Use or gates for the shift operation
    or o1(shifted_address[31], address[29], address[29]);
    or o2(shifted_address[30], address[28], address[28]);
    or o3(shifted_address[29], address[27], address[27]);
    or o4(shifted_address[28], address[26], address[26]);
    or o5(shifted_address[27], address[25], address[25]);
    or o6(shifted_address[26], address[24], address[24]);
    or o7(shifted_address[25], address[23], address[23]);
    or o8(shifted_address[24], address[22], address[22]);
    or o9(shifted_address[23], address[21], address[21]);
    or o10(shifted_address[22], address[20], address[20]);
    or o11(shifted_address[21], address[19], address[19]);
    or o12(shifted_address[20], address[18], address[18]);
    or o13(shifted_address[19], address[17], address[17]);
    or o14(shifted_address[18], address[16], address[16]);
    or o15(shifted_address[17], address[15], address[15]);
    or o16(shifted_address[16], address[14], address[14]);
    or o17(shifted_address[15], address[13], address[13]);
    or o18(shifted_address[14], address[12], address[12]);
    or o19(shifted_address[13], address[11], address[11]);
    or o20(shifted_address[12], address[10], address[10]);
    or o21(shifted_address[11], address[9], address[9]);
    or o22(shifted_address[10], address[8], address[8]);
    or o23(shifted_address[9], address[7], address[7]);
    or o24(shifted_address[8], address[6], address[6]);
    or o25(shifted_address[7], address[5], address[5]);
    or o26(shifted_address[6], address[4], address[4]);
    or o27(shifted_address[5], address[3], address[3]);
    or o28(shifted_address[4], address[2], address[2]);
    or o29(shifted_address[3], address[1], address[1]);
    or o30(shifted_address[2], address[0], address[0]);
    or o31(shifted_address[1], 1'b0, 1'b0);
    or o32(shifted_address[0], 1'b0, 1'b0);


endmodule
