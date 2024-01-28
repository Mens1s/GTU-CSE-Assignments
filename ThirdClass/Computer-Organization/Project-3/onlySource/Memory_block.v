module Memory_block (
    output reg [31:0] read_data,
    input byteOperations,
    //input [17:0] address,
    input [7:0] address,
    input [31:0] write_data,
    input memRead,
    input memWrite
);

    reg [31:0] memory [0:31];  // Assuming a memory size of
    // read data from memory.mem
    initial begin
        $readmemb("memory.mem", memory);
    end

    // Read port
    always @* begin
         // $display("Reading : %b memRead : %b memWrite : %b byte : %b address : %b", read_data, memRead, memWrite, byteOperations, address);

        if (memRead) begin
            if (byteOperations) begin
                // Read a byte from memory
                /*
                case (address[1:0])
                    2'b00: read_data = memory[address][7:0];
                    2'b01: read_data = memory[address][15:8];
                    2'b10: read_data = memory[address][23:16];
                    2'b11: read_data = memory[address][31:24];
                endcase
                */
                // Fill read_data1 and read_data2 with zeros
                read_data = 32'b0;

                // Extract the lowest byte from the specified registers
                read_data[7:0] = memory[address][7:0];
            end else begin
                // Read a full word from memory
                read_data = memory[address];
            end
        end else begin
            // Default value when not reading
            read_data = 32'b0;
        end

        if (memWrite) begin
            if (byteOperations) begin
                // Write a byte to memory
                /*
                case (address[1:0])
                    2'b00: memory[address][7:0] = write_data[7:0];
                    2'b01: memory[address][15:8] = write_data[7:0];
                    2'b10: memory[address][23:16] = write_data[7:0];
                    2'b11: memory[address][31:24] = write_data[7:0];
                endcase
                */
                memory[address][7:0] = write_data[7:0];
            end else begin
                // Write a full word to memory
                memory[address] = write_data;
            end
            // Write data to memory.mem
            $writememb("memory.mem", memory);
        end
    end

endmodule
