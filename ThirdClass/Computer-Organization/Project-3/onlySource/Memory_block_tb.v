// iverilog -o Memory_block Memory_block.v Memory_block_tb.v 
`timescale 1ns/1ns

module Memory_block_tb;

    reg byteOperations, memRead, memWrite;
    reg [7:0] address;
    reg [31:0] write_data;
    wire [31:0] read_data;

    // Instantiate the module under test
    Memory_block uut (
        .read_data(read_data),
        .byteOperations(byteOperations),
        .address(address),
        .write_data(write_data),
        .memRead(memRead),
        .memWrite(memWrite)
    );


    // Testbench stimulus
    initial begin
        // Test case 1: Write a word to memory
        memRead = 0;
        memWrite = 1;
        byteOperations = 1;
        address = 8'h00;
        write_data = 32'b00010000000000000000000000000010;
        $display("Writing : %b memRead : %b memWrite : %b byteOp : %b", write_data, memRead, memWrite, byteOperations);
        #10;

        // Test case 2: Read a word from memory
        memRead = 1;
        memWrite = 0;
        byteOperations = 0;
        address = 8'h00;
        $display("Writing : %b memRead : %b memWrite : %b byteOp : %b", write_data, memRead, memWrite, byteOperations);
        #10;

        // Test case 3: Write a byte to memory
        memRead = 0;
        memWrite = 1;
        byteOperations = 1;
        address = 8'h01;
        write_data = 8'h77;
        $display("Writing : %b memRead : %b memWrite : %b byteOp : %b", write_data, memRead, memWrite, byteOperations);
        #10;

        // Test case 4: Read a byte from memory
        memRead = 1;
        memWrite = 0;
        byteOperations = 1;
        address = 8'h01;
        $display("Writing : %b memRead : %b memWrite : %b byteOp : %b", write_data, memRead, memWrite, byteOperations);
        #10;

        // Terminate simulation
        #10;
        $finish;
    end

endmodule
