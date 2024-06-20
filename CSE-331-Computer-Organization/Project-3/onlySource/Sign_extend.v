module Sign_extend (
    output [31:0] sign_ext_imm,
    input [15:0] imm
);

   // Use AND gates for sign extension
    or o1(sign_ext_imm[31], imm[15], 0);
    or o2(sign_ext_imm[30], imm[15], 0);
    or o3(sign_ext_imm[29], imm[15], 0);
    or o4(sign_ext_imm[28], imm[15], 0);
    or o5(sign_ext_imm[27], imm[15], 0);
    or o6(sign_ext_imm[26], imm[15], 0);
    or o7(sign_ext_imm[25], imm[15], 0);
    or o8(sign_ext_imm[24], imm[15], 0);
    or o9(sign_ext_imm[23], imm[15], 0);
    or o10(sign_ext_imm[22], imm[15], 0);
    or o11(sign_ext_imm[21], imm[15], 0);
    or o12(sign_ext_imm[20], imm[15], 0);
    or o13(sign_ext_imm[19], imm[15], 0);
    or o14(sign_ext_imm[18], imm[15], 0);
    or o15(sign_ext_imm[17], imm[15], 0);
    or o16(sign_ext_imm[16], imm[15], 0);
    or o17(sign_ext_imm[15], imm[15], 0);
    or o18(sign_ext_imm[14], imm[14], 1'b0);
    or o19(sign_ext_imm[13], imm[13], 1'b0);
    or o20(sign_ext_imm[12], imm[12], 1'b0);
    or o21(sign_ext_imm[11], imm[11], 1'b0);
    or o22(sign_ext_imm[10], imm[10], 1'b0);
    or o23(sign_ext_imm[9], imm[9], 1'b0);
    or o24(sign_ext_imm[8], imm[8], 1'b0);
    or o25(sign_ext_imm[7], imm[7], 1'b0);
    or o26(sign_ext_imm[6], imm[6], 1'b0);
    or o27(sign_ext_imm[5], imm[5], 1'b0);
    or o28(sign_ext_imm[4], imm[4], 1'b0);
    or o29(sign_ext_imm[3], imm[3], 1'b0);
    or o30(sign_ext_imm[2], imm[2], 1'b0);
    or o31(sign_ext_imm[1], imm[1], 1'b0);
    or o32(sign_ext_imm[0], imm[0], 1'b0);

endmodule
