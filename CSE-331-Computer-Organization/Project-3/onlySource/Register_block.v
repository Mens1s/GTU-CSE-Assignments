module Register_block (
    output reg [31:0] read_data1,
    output reg [31:0] read_data2,
    input byteOperations,
    input [31:0] write_data,
    input [4:0] read_reg1,
    input [4:0] read_reg2,
    input [4:0] write_reg,
    input regWrite
);

    reg [31:0] registers [0:31];

    // read data
    initial begin
        $readmemb("registers.mem", registers);
    end 

    
    // Read ports
    always @* begin
        
        //$display("write_data: %b", write_data);
        if (byteOperations) begin
            // Fill read_data1 and read_data2 with zeros
            read_data1 = 32'b0;
            read_data2 = 32'b0;

            // Extract the lowest byte from the specified registers
            read_data1[7:0] = registers[read_reg1][7:0];
            read_data2[7:0] = registers[read_reg2][7:0];
        end else begin
            // Read the full words from the specified registers
            read_data1 = registers[read_reg1];
            read_data2 = registers[read_reg2];
            //  display
        end

        if (regWrite) begin
            #2;
            // Write data based on byte operations (if applicable)
            if (byteOperations) begin
                registers[write_reg][7:0] = write_data[7:0];
            end 
            else begin
                registers[write_reg] = write_data;
            end
            $writememb("registers.mem", registers);
        
        end
    end



    // Write port with optional byte operations
    /*always @* begin

        if (regWrite) begin
            // Write data based on byte operations (if applicable)
            if (byteOperations) begin
                registers[write_reg][7:0] = write_data[7:0];
            end 
            else begin
                registers[write_reg] = write_data;
            end
            $writememb("registers.mem", registers);
            $display("write_data: %b", write_data);
        end
    end*/

endmodule
