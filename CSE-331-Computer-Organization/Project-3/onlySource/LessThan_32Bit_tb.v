// derleme kodu : iverilog -o LessThan_32Bit LessThan_32Bit.v LessThan_32Bit_tb.v Substract_32Bit.v FullAdder_32Bit.v FullAdder_4Bit_CLA.v
module LessThan_32Bit_tb;

    reg [31:0] a;
    reg [31:0] b;
    wire LessThan;

    LessThan_32Bit dut(.LessThan(LessThan), .A(a), .B(b));

    initial begin
        a = 32'hFFFFFFFF; 
        b = 32'hFFFFFFFF;
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
        $monitor("Time: %2d %b\t<\t%b\t=\t%b",$time, a, b, LessThan);
    end

endmodule