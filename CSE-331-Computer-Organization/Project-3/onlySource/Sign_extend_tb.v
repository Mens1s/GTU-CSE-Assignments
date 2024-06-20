// derleme kodu : iverilog -o Sign_extend Sign_extend.v Sign_extend_tb.v 

module Sign_extend_tb;

   reg [15:0] imm;
   wire [31:0] sign_ext_imm;

   // Instantiate the Sign_extend module
   Sign_extend uut (
      .sign_ext_imm(sign_ext_imm),
      .imm(imm)
   );

   // Clock generation
   reg clk = 0;
   always #5 clk = ~clk;

   // Test stimulus
   initial begin
      // Test case 1: Positive value
      imm = 16'h1234;
      #10;
        $display("imm = %h, sign_ext_imm = %h", imm, sign_ext_imm);

      // Test case 2: Negative value
      imm = 16'hFEDC;
      #10;

        // pring 
        $display("imm = %h, sign_ext_imm = %h", imm, sign_ext_imm);
      // Add more test cases as needed

      $finish;
   end

endmodule
