module FullAdder_4Bit_CLA(
    input [3:0] A,
    input [3:0] B,
    input Cin,
    output [3:0] Sum,
    output p,
    output g);
    
    wire [3:0] Gen, Propagate, Carry;
    wire [32:0] temp;

    // Generate and Propagate for each bit
    xor xo0(Propagate[0], A[0], B[0]);
    xor xo1(Propagate[1], A[1], B[1]);
    xor xo2(Propagate[2], A[2], B[2]);
    xor xo3(Propagate[3], A[3], B[3]);

    and a0(Gen[0], A[0], B[0]);
    and a1(Gen[1], A[1], B[1]);
    and a2(Gen[2], A[2], B[2]);
    and a3(Gen[3], A[3], B[3]);
    
    // Generate and Propagate for zero bit
    or o4(Carry[0], Cin, 0);
    
    // fill carry 1
    and a4(temp[0], Propagate[0], Carry[0]);
    or o5(Carry[1], Gen[0], temp[0]);

    // fill carry 2
    and a5(temp[1], Propagate[0], Carry[0]);
    and a6(temp[2], temp[1], Propagate[1]);

    and a7(temp[3], Gen[0], Propagate[1]);

    or o6(temp[4], temp[2], temp[3]);
    or o7(Carry[2], temp[4], Gen[1]);

    // fill carry 3
    and a8(temp[5], Propagate[0], Carry[0]);
    and a9(temp[6], temp[5], Propagate[1]);
    and a10(temp[7], temp[6], Propagate[2]);

    and a11(temp[8], Gen[0], Propagate[1]);
    and a12(temp[9], temp[8], Propagate[2]);

    and a13(temp[10], Gen[1], Propagate[2]);

    or o8(temp[11], temp[7], temp[9]);    
    or o9(temp[12], temp[10], temp[11]);
    or o10(Carry[3], temp[12], Gen[2]);

    // fill cout
    and a14(temp[13], Propagate[0], Carry[0]);
    and a15(temp[14], temp[13], Propagate[1]);
    and a16(temp[15], temp[14], Propagate[2]);
    and a17(temp[16], temp[15], Propagate[3]);

    and a18(temp[17], Gen[0], Propagate[1]);
    and a19(temp[18], temp[17], Propagate[2]);
    and a20(temp[19], temp[18], Propagate[3]);

    and a21(temp[20], Gen[1], Propagate[2]);
    and a22(temp[21], temp[20], Propagate[3]);

    and a23(temp[22], Gen[2], Propagate[3]);

    or o11(temp[23], temp[16], temp[19]);
    or o12(temp[24], temp[21], temp[22]);
    or o13(temp[25], temp[23], temp[24]);
    or o14(Cout, temp[25], Gen[3]);

    // Propagate xor Carrry
    xor xo4(Sum[0], Propagate[0], Carry[0]);
    xor xo5(Sum[1], Propagate[1], Carry[1]);
    xor xo6(Sum[2], Propagate[2], Carry[2]);
    xor xo7(Sum[3], Propagate[3], Carry[3]);

    // Generate
    and a24(p, Propagate[0], Propagate[1], Propagate[2], Propagate[3]);

    // Generate xor Carry
    and a25(temp[26], Propagate[1], Propagate[2], Propagate[3], Gen[0]);
    and a26(temp[27], Propagate[3], Propagate[2], Gen[1]);
    and a27(temp[28], Propagate[3], Gen[2]);

    or o15(g, Gen[3], temp[26], temp[27], temp[28]);
endmodule