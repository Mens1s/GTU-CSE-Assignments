module Bus_for_jump(
    output [31:0] jumpAddress,
    input [31:0] shiftedJumpAddress,
    input [31:0] program_counter 
);

    and a1(jumpAddress[31], shiftedJumpAddress[31], program_counter[31]);
    and a2(jumpAddress[30], shiftedJumpAddress[30], program_counter[30]);
    and a3(jumpAddress[29], shiftedJumpAddress[29], program_counter[29]);
    and a4(jumpAddress[28], shiftedJumpAddress[28], program_counter[28]);
    and a5(jumpAddress[27], shiftedJumpAddress[27], shiftedJumpAddress[27]);
    and a6(jumpAddress[26], shiftedJumpAddress[26], shiftedJumpAddress[26]);
    and a7(jumpAddress[25], shiftedJumpAddress[25], shiftedJumpAddress[25]);
    and a8(jumpAddress[24], shiftedJumpAddress[24], shiftedJumpAddress[24]);
    and a9(jumpAddress[23], shiftedJumpAddress[23], shiftedJumpAddress[23]);
    and a10(jumpAddress[22], shiftedJumpAddress[22], shiftedJumpAddress[22]);
    and a11(jumpAddress[21], shiftedJumpAddress[21], shiftedJumpAddress[21]);
    and a12(jumpAddress[20], shiftedJumpAddress[20], shiftedJumpAddress[20]);
    and a13(jumpAddress[19], shiftedJumpAddress[19], shiftedJumpAddress[19]);
    and a14(jumpAddress[18], shiftedJumpAddress[18], shiftedJumpAddress[18]);
    and a15(jumpAddress[17], shiftedJumpAddress[17], shiftedJumpAddress[17]);
    and a16(jumpAddress[16], shiftedJumpAddress[16], shiftedJumpAddress[16]);
    and a17(jumpAddress[15], shiftedJumpAddress[15], shiftedJumpAddress[15]);
    and a18(jumpAddress[14], shiftedJumpAddress[14], shiftedJumpAddress[14]);
    and a19(jumpAddress[13], shiftedJumpAddress[13], shiftedJumpAddress[13]);
    and a20(jumpAddress[12], shiftedJumpAddress[12], shiftedJumpAddress[12]);
    and a21(jumpAddress[11], shiftedJumpAddress[11], shiftedJumpAddress[11]);
    and a22(jumpAddress[10], shiftedJumpAddress[10], shiftedJumpAddress[10]);
    and a23(jumpAddress[9], shiftedJumpAddress[9], shiftedJumpAddress[9]);
    and a24(jumpAddress[8], shiftedJumpAddress[8], shiftedJumpAddress[8]);
    and a25(jumpAddress[7], shiftedJumpAddress[7], shiftedJumpAddress[7]);
    and a26(jumpAddress[6], shiftedJumpAddress[6], shiftedJumpAddress[6]);
    and a27(jumpAddress[5], shiftedJumpAddress[5], shiftedJumpAddress[5]);
    and a28(jumpAddress[4], shiftedJumpAddress[4], shiftedJumpAddress[4]);
    and a29(jumpAddress[3], shiftedJumpAddress[3], shiftedJumpAddress[3]);
    and a30(jumpAddress[2], shiftedJumpAddress[2], shiftedJumpAddress[2]);
    and a31(jumpAddress[1], shiftedJumpAddress[1], shiftedJumpAddress[1]);
    and a32(jumpAddress[0], shiftedJumpAddress[0], shiftedJumpAddress[0]);

endmodule