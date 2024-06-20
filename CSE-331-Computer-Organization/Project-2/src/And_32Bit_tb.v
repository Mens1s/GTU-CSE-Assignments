module And_32Bit_tb;

    reg [31:0] a;
    reg [31:0] b;
    wire [31:0] out;

    And_32Bit dut(out, a, b);

    initial begin
        a = 32'hFFFFFFFF; 
        b = 32'hFFFFFFFF;
        #10;

        a = 32'b0;
        b = 32'b1;
        #10;

        a = 32'b1;
        b = 32'b0;
        #10;

        a = 32'b1;
        b = 32'b1;
        #10;

        $finish;
    end

    initial begin
        $monitor("Time: %2d %b\tand\t%b\t=\t%b",$time, a, b, out);
    end

endmodule