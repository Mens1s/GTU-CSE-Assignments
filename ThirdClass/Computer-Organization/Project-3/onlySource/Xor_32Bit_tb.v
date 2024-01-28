module Xor_32Bit_tb;

    reg [31:0] a;
    reg [31:0] b;
    wire [31:0] out;

    Xor_32Bit dut(out, a, b);

    initial begin
        a = 32'hFFFFFFFF; 
        b = 32'h00000000;
        #10;

        a = 32'h12311111;
        b = 32'h00100000;
        #10;

        a = 32'h10000100;
        b = 32'h11000010;
        #10;

        a = 32'h11111000;
        b = 32'h00001111;
        #10;

        $finish;
    end

    initial begin
        $monitor("Time: %2d %b\txor\t%b\t=\t%b",$time, a, b, out);
    end

endmodule