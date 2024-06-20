// iverilog -o Mod_32Bit Mod_32Bit.v Mod_32Bit_tb.v Mod_cu.v Mod_dp.v
module Mod_32Bit_tb;

  reg clk;
  reg rst;
  reg start;
  reg [31:0] a;
  reg [31:0] b;
  wire [31:0] result;
  wire done;

  // Instantiate the DUT (Design Under Test)
  Mod_32Bit dut (
    .clk(clk),
    .rst(rst),
    .start(start),
    .a(a),
    .b(b),
    .result(result),
    .done(done)
  );

  // Clock generation
  always #5 clk = ~clk;

  // Stimulus
  initial begin
    clk = 0;
    rst = 1;
    start = 0;
    a = 50; // Set initial values for a and b
    b = 13;

    #10 rst = 0; // Deassert reset

    // Test scenario
    // You can add multiple test cases here by changing inputs and checking outputs
    #20 start = 1; // Initiate operation
    #100; // Wait for computation to complete

    // Add more test scenarios as needed

    $stop; // Stop simulation
  end

  // Monitor outputs
  always @(posedge clk) begin
    if (done) begin
      $display("Result: %d", result);
      $display("Computation Done!");
      // You can add more actions or checks based on the output here
    end
  end

endmodule
