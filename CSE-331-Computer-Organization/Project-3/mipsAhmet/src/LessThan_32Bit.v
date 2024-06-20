module LessThan_32Bit(input [31:0] A, input [31:0] B, output LessThan);
    wire [31:0] Result;
    wire Cout;

    Substract_32Bit sub(
        .A(A),
        .B(B),
        .Result(Result),
        .Cout(Cout)
        );

    or and1 (LessThan, 0, Result[31]);

endmodule
