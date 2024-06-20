module Add_32Bit(input [31:0] A, input [31:0] B, output [31:0] Sum, output Cout);

    FullAdder_32Bit fa(
        .a(A),
        .b(B),
        .cin(1'b0),
        .sum(Sum),
        .carry(Cout)
        );

endmodule