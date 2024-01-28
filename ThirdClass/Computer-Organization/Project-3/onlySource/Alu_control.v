module Alu_control (
    output reg [2:0] alu_ctr,
    input [5:0] function_code,
    input [2:0] ALUop
);
/*
    and a1 (isRtype, ALUop[0],  ALUop[1], ALUop[2]);
    
    // decision of alu_ctr
    wire [2:0] rTypeDecision;
    wire [2:0] notRTypeDecision;
    wire [0:0] fc2, fc1, fc0, res1, res2;

    not not1 (fc2[0], function_code[2]);
    not not2 (fc1[0], function_code[1]);
    not not3 (fc0[0], function_code[0]);

    and a2 (rTypeDecision[2], isRtype, function_code[1]);
    and a3 (rTypeDecision[1], isRtype, fc2[0], function_code[1], function_code[0]);

    // 010
    and a4 (res1[0], isRtype, fc2[0], function_code[1], fc0[0]);
    // 101
    and a5 (res2[0], isRtype, function_code[2], fc1[0], function_code[0]);

    or o1 (rTypeDecision[0], res1[0], res2[0]);

    not n1 (notRtype, isRtype);
    and a8 (notRTypeDecision[0], notRtype, ALUop[0]);
    and a6 (notRTypeDecision[1], notRtype, ALUop[1]);
    and a7 (notRTypeDecision[2], notRtype, ALUop[2]);

    or o2 (alu_ctr[0], rTypeDecision[0], notRTypeDecision[0]);
    or o3 (alu_ctr[1], rTypeDecision[1], notRTypeDecision[1]);
    or o4 (alu_ctr[2], rTypeDecision[2], notRTypeDecision[2]);


*/
    always @ (function_code, ALUop) begin
        if (ALUop == 3'b111) begin
            case (function_code)
                6'b000010: begin
                    alu_ctr = 3'b101;
                end
                6'b000011: begin
                    alu_ctr = 3'b110;
                end
                6'b000100: begin
                    alu_ctr = 3'b000;
                end
                6'b000101: begin
                    alu_ctr = 3'b001;
                end
                6'b000111: begin
                    alu_ctr = 3'b100;
                end
                default: begin
                    alu_ctr = 3'b000;
                end
            endcase
        end
        else begin
            alu_ctr = ALUop;
        end
    end
  
endmodule