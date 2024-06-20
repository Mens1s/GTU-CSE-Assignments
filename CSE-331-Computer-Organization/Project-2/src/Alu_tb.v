// iverilog -o Alu Alu.v Alu_tb.v Add_32Bit.v And_32Bit.v LessThan_32Bit.v Mod_cu.v Mux8x1.v Nor_32Bit.v Or_32Bit.v Substract_32Bit.v Xor_32Bit.v FullAdder_32Bit.v FullAdder_16Bit_CLA.v FullAdder_4Bit_CLA.v Mod_32Bit.v Mod_dp.v
// vvp Alu
module Alu_tb;

    reg [31:0] a;
    reg [31:0] b;
    reg [2:0] sel;
    reg c_in;
    wire c_out;
    reg clk;
    reg reset;
    reg start;
    
    wire [31:0] result;
    wire done;

    Alu alu_inst (
        .result(result),
        .a(a),
        .b(b),
        .sel(sel),
        .c_in(c_in),
        .c_out(c_out),
        .start(start),
        .reset(reset),
        .clk(clk),
        .done(done)
    );
    always #10 clk = ~clk; // Clock generation

    initial begin
        // Initialize inputs
        a = 100;
        b = 60;
        sel = 1'b0;
        c_in = 1'b0;
        clk = 1'b0;
        reset = 1'b1;
        start = 1'b0;

        sel = 3'b000;
        #10;

        sel = 3'b001;
        #10;

        sel = 3'b010;
        #10;

        sel = 3'b011;
        #10;

        sel = 3'b100;
        #10;

        sel = 3'b101;
        #10;

        sel = 3'b110;
        #10000;

        // Reset the module
        
        sel = 3'b111;
        #100
        #100reset = 1'b0;
        #100reset = 1'b1;
        #200;
        a = 100;
        b = 6;
        start = 1;
        #1000;
        start = 0;
        #1000;
        $finish;
    end
    
    initial begin
        $monitor("Selection = %b A = %b, B = %b, Cin = %b, Result = %b, Cout = %b",sel , a, b, c_in, result, c_out);
    end
    
endmodule
