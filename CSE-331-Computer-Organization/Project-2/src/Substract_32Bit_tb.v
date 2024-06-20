// derleme kodu : iverilog -o Substract_32Bit Substract_32Bit.v Substract_32Bit_tb.v FullAdder_32Bit.v FullAdder_4Bit_CLA.v
module Substract_32Bit_tb;
    reg [31:0] A, B;
    reg Cin;
    wire [31:0] Result;
    wire Cout;

    Substract_32Bit dut(
        .A(A),
        .B(B),
        .Result(Result),
        .Cout(Cout)
        );

    initial begin

        A = 110000; // Example input A
        B = 120000; // Example input B
        Cin = 1'b0;  // Example input Cin

        #10; 

        $finish; // End simulation
    end

    initial begin
        $monitor("A = %b, B = %b, Cin = %b, Result = %b, Cout = %b", A, B, Cin, Result, Cout);
    end

endmodule