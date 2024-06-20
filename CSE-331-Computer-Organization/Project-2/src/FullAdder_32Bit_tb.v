// iverilog -o FullAdder_32Bit FullAdder_32Bit.v FullAdder_32Bit_tb.v FullAdder_16Bit_CLA.v FullAdder_4Bit_CLA.v
// vvp FullAdder_32Bit
module FullAdder_32Bit_tb;
    reg [31:0] A, B;
    reg Cin;
    wire [31:0] Sum;
    wire Cout;

    FullAdder_32Bit dut (
        .a(A),
        .b(B),
        .cin(Cin),
        .sum(Sum),
        .carry(Cout)
    );

    initial begin

        A = 299999; // Example input A
        B = 899999; // Example input B
        Cin = 1'b0;  // Example input Cin

        #10; 

        $finish; // End simulation
    end

    initial begin
        $monitor("A = %b, B = %b, Cin = %b, Sum = %b, Cout = %b", A, B, Cin, Sum, Cout);
    end
endmodule
