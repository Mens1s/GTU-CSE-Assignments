module FullAdder_4Bit_CLA_tb;
    reg [3:0] A, B;
    reg Cin;
    wire [3:0] Sum;
    wire Cout;

    FullAdder_4Bit_CLA dut (
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );

    initial begin

        A = 4'd15; // Example input A
        B = 4'd15; // Example input B
        Cin = 1'b0;  // Example input Cin

        #10; 

        $finish; // End simulation
    end

    initial begin
        $monitor("A = %b, B = %b, Cin = %b, Sum = %b, Cout = %b", A, B, Cin, Sum, Cout);
    end
endmodule
