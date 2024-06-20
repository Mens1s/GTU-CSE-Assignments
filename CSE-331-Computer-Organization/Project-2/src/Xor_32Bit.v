module Xor_32Bit(output [31:0] out, input [31:0] a, input[31:0] b);

    xor xo0(out[0], b[0], a[0]);
    xor xo1(out[1], b[1], a[1]);
    xor xo2(out[2], b[2], a[2]); 
    xor xo3(out[3], b[3], a[3]);
    xor xo4(out[4], b[4], a[4]);
    xor xo5(out[5], b[5], a[5]);
    xor xo6(out[6], b[6], a[6]);
    xor xo7(out[7], b[7], a[7]);
    xor xo8(out[8], b[8], a[8]);
    xor xo9(out[9], b[9], a[9]);
    xor xo10(out[10], b[10], a[10]);
    xor xo11(out[11], b[11], a[11]);
    xor xo12(out[12], b[12], a[12]);
    xor xo13(out[13], b[13], a[13]);
    xor xo14(out[14], b[14], a[14]);
    xor xo15(out[15], b[15], a[15]);
    xor xo16(out[16], b[16], a[16]);
    xor xo17(out[17], b[17], a[17]);
    xor xo18(out[18], b[18], a[18]);
    xor xo19(out[19], b[19], a[19]);
    xor xo20(out[20], b[20], a[20]);
    xor xo21(out[21], b[21], a[21]);
    xor xo22(out[22], b[22], a[22]);
    xor xo23(out[23], b[23], a[23]);
    xor xo24(out[24], b[24], a[24]);
    xor xo25(out[25], b[25], a[25]);
    xor xo26(out[26], b[26], a[26]);
    xor xo27(out[27], b[27], a[27]);
    xor xo28(out[28], b[28], a[28]);
    xor xo29(out[29], b[29], a[29]);
    xor xo30(out[30], b[30], a[30]);
    xor xo31(out[31], b[31], a[31]);

endmodule