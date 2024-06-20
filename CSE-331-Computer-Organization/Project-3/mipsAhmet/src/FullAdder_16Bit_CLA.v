module FullAdder_16Bit_CLA(
    input [15:0] A,
    input [15:0] B,
    input Cin,
    output [15:0] Sum,
    output [3:0] C);

    wire [16:0] temp;
    wire [3:0] p, g;

    FullAdder_4Bit_CLA u0(A[3:0], B[3:0], Cin, Sum[3:0], p[0], g[0]);
    FullAdder_4Bit_CLA u1(A[7:4], B[7:4], C[0], Sum[7:4], p[1], g[1]);
    FullAdder_4Bit_CLA u2(A[11:8], B[11:8], C[1], Sum[11:8], p[2], g[2]);
    FullAdder_4Bit_CLA u3(A[15:12], B[15:12], C[2], Sum[15:12], p[3], g[3]);

    and gate6(temp[0], p[0], Cin);
    or gate7(C[0], g[0], temp[0]);

    and gate8(temp[1], p[1], g[0]);
    and gate9(temp[2], p[1], p[0], Cin);
    or gate10(C[1], g[1], temp[1], temp[2]);

    and gate11(temp[3], p[2], g[1]);
    and gate12(temp[4], p[2], p[1], g[0]);
    and gate13(temp[5], p[2], p[1], p[0], Cin);
    or gate14(C[2], g[2], temp[3], temp[4], temp[5]);

    and gate15(temp[6], p[3], g[2]);
    and gate16(temp[7], p[3], p[2], g[1]);
    and gate17(temp[8], p[3], p[2], p[1], g[0]);
    and gate18(temp[9], p[3], p[2], p[1], p[0], Cin);
    or gate19(C[3], g[3], temp[6], temp[7], temp[8], temp[9]);

endmodule