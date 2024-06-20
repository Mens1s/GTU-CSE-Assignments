// derleme kodu     : iverilog -o Add_32Bit_tb.vvp Add_32Bit.v Add_32Bit_tb.v FullAdder_32Bit.v FullAdder_4Bit_CLA.V 
module Add_32Bit_tb;
    reg [31:0] A, B;
    reg Cin;
    wire [31:0] Sum;
    wire Cout;

    Add_32Bit dut (
        .A(A),
        .B(B),
        .Sum(Sum),
        .Cout(Cout)
    );

    initial begin

        A = 992139129; // Example input A
        B = 233123124; // Example input B
        Cin = 1'b0;  // Example input Cin

        #10; 

        $finish; // End simulation
    end

    initial begin
        $monitor("A = %b, B = %b, Cin = %b, Sum = %b, Cout = %b", A, B, Cin, Sum, Cout);
    end
endmodule
