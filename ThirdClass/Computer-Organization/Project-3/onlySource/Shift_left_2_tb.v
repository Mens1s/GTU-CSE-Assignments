// derleme kodu : iverilog -o Shift_left_2 Shift_left_2.v Shift_left_2_tb.v And_32Bit.v
module Shift_left_2_tb;

   reg [31:0] address;
   reg clk = 0;
   wire [31:0] shifted_address;

   // Instantiate the shift_left_2 module
   Shift_left_2 uut (
      .shifted_address(shifted_address),
      .address(address)
   );

   // Clock generation
   always #5 clk = ~clk;

   // Test stimulus
   initial begin

      // Test case 1
      address = 32'h12345678;
      #10;
    $display("address = %h, shifted_address = %h", address, shifted_address);

      // Test case 2
      address = 32'h00000001;
      #10;

      // Add more test cases as needed

    // print
    $display("address = %h, shifted_address = %h", address, shifted_address);
      $finish;
   end

endmodule
