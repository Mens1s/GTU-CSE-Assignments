// iverilog -o Mod_cu Mod_cu.v Mod_cu_tb.v
// vvp Mod_cu
module Mod_cu_tb;

    reg clk;
    reg start;
    reg restart;
    reg [31:0] A;
    reg [31:0] B;
    wire [31:0] result;
    wire done;
    
    // Instantiate the mod module
    Mod_cu uut (
        .clk(clk),
        .start(start),
        .rst(restart),
        .a(A),
        .b(B),
        .result(result),
        .done(done)
    );

    // Clock generation
    always #5 clk = ~clk;

    // Testbench procedure
    initial begin
        
        clk = 0;
        restart = 1;
        start = 0;
        A = 0;
        B = 0;

        // Reset the module
        #10 restart = 0;
        #10 restart = 1;


        // Test case 3: 87%6
        #20;
        A = 10;
        B = 4;
        start = 1;
        #10000;
        start = 0;

        // End simulation after test cases
        $finish;
    end

    // Monitor result and done signals
    always @ (posedge clk) begin
        if (done)
            $display("Time: %0t A: %d, B: %d, Result: %d", $time, A, B, result);
    end

endmodule
