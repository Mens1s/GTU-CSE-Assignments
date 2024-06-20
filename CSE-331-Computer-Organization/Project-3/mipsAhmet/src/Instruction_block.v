module Instruction_block (
    output reg [31:0] instruction,
    input [31:0] pc
);
    
    reg [31:0] instruction_memory [0:31];

    initial begin
        $readmemb("instructions.mem", instruction_memory);
    end

    always @(*) begin
        instruction = instruction_memory[pc >> 2];    
    end
    
endmodule