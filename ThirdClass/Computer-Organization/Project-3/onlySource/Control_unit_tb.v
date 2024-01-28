// iverilog -o Control_unit Control_unit.v Control_unit_tb.v 
module Control_unit_tb;

  reg [5:0] opcode;
  wire regDst, branch, memToReg, memWrite, ALUsrc, regWrite, jump, byteOperations, move;
  wire [2:0] ALUop;

  // Instantiate the Control_unit module
  Control_unit myControlUnit (
    .regDst(regDst),
    .branch(branch),
    .memToReg(memToReg),
    .memWrite(memWrite),
    .ALUop(ALUop),
    .ALUsrc(ALUsrc),
    .regWrite(regWrite),
    .jump(jump),
    .byteOperations(byteOperations),
    .move(move),
    .opcode(opcode)
  );

  // Initial block for test cases
  initial begin
    // Test case 1: R-type instruction
    opcode = 6'b000000;
    #10;
    $display("Test Case 1: R-type instruction");
    printSignals();

    // Test case 2: lw (load word) instruction
    opcode = 6'b100011;
    #10;
    $display("Test Case 2: lw (load word) instruction");
    printSignals();

    // Add more test cases as needed

    // Terminate simulation
    $finish;
  end

  // Helper task to print signals
  task printSignals;
    $display("regDst=%b, branch=%b, memToReg=%b, memWrite=%b, ALUop=%b, ALUsrc=%b, regWrite=%b, jump=%b, byteOperations=%b, move=%b",
      regDst, branch, memToReg, memWrite, ALUop, ALUsrc, regWrite, jump, byteOperations, move);
  endtask

endmodule
