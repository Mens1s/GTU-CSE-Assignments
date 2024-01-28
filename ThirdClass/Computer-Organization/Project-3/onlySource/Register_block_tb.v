`timescale 1ns/1ns

module Register_block_tb;

    reg clk;
    reg byteOperations;
    reg [31:0] write_data;
    reg [4:0] read_reg1, read_reg2, write_reg;
    reg regWrite;

    wire [31:0] read_data1, read_data2;

    // Instantiate the module under test
    Register_block uut (
        .read_data1(read_data1),
        .read_data2(read_data2),
        .byteOperations(byteOperations),
        .write_data(write_data),
        .read_reg1(read_reg1),
        .read_reg2(read_reg2),
        .write_reg(write_reg),
        .regWrite(regWrite)
    );

    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    // Set initial values for testing
    initial begin
        byteOperations = 0;
        write_data = 8'b11111111;
        read_reg1 = 5'b00000;  // Register 0
        read_reg2 = 5'b00001;  // Register 1
        write_reg = 5'b00010;  // Register 2
        regWrite = 1;

        // Monitor for display
        $monitor("Time=%0t: byteOperations=%b, read_data1=%b, read_data2=%b", $time, byteOperations, read_data1, read_data2);
        
        // Test case 1: Write data to register
        #10;
        byteOperations = 1;
        regWrite = 0;

        // Test case 2: Write data to register
        //#10;
        byteOperations = 1;
        regWrite = 1;

        #10
        regWrite = 0;

        //#10;
        write_reg = 5'b00111;  // Register 2
        write_data = 32'b00001111;
        byteOperations = 0;
        regWrite = 1;

        // Terminate simulation
        #10;
        $finish;
    end

endmodule
