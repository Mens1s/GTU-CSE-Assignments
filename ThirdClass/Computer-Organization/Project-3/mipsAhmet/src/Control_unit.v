module Control_unit(
	 output regDst,
	 output branch,
	 output memToReg,
	 output memWrite,
	 output [2:0] ALUop,
	 output ALUsrc,
	 output regWrite,
	 output jump,
	 output byteOperations, 
	 output move,
	 input [5:0] opcode);
	 
    not not22 (op0_not, opcode[0]);
    not not23 (op1_not, opcode[1]);
    not not24 (op2_not, opcode[2]);
    not not25 (op3_not, opcode[3]);
    not not26 (op4_not, opcode[4]);
    not not27 (op5_not, opcode[5]);
    
    and and99 (isRtype, op5_not, op4_not, op3_not, op2_not, op1_not, op0_not);
    and and30 (isSlti, op5_not, op4_not, op3_not, opcode[2], opcode[1], opcode[0]);
    and and31 (isAddi, op5_not, op4_not, op3_not, op2_not, opcode[1], op0_not);
    and and332 (isOri, op5_not, op4_not, op3_not, opcode[2], op1_not, opcode[0]);

    and and33 (aluOpTwoV, op5_not, op4_not, op3_not, op1_not);
    not not34 (resultFromFirst, aluOpTwoV);
    or or38 (ALUop[2], isRtype, resultFromFirst);

    not not41 (notSLI, isSlti);
    and and35 (resultFromSecond, op4_not, op3_not, opcode[1], opcode[0], notSLI);
    or or39 (ALUop[1], isRtype, resultFromSecond);

    xor xor36 (aluOpZeroX, opcode[4], opcode[3]);
    or or37 (ALUop[0], aluOpZeroX, isAddi, isOri, move, isRtype);

    /*
    always @* begin 
        $display("opcode : %b", opcode);
        $display("aluop : %b", ALUop);
        $display("aluop0 : %b", ALUop[0]);
        $display("aluop1 : %b", ALUop[1]);
        $display("aluop2 : %b", ALUop[2]);
        $display("opcode0 : %b", opcode[0]);
        $display("opcode1 : %b", opcode[1]);
        $display("opcode2 : %b", opcode[2]);
        $display("opcode3 : %b", opcode[3]);
        $display("opcode4 : %b", opcode[4]);
        $display("opcode5 : %b", opcode[5]);
    end
    */

    nor nor1 (nor1_result, opcode[4], opcode[0]);
    and and18 (is_move, opcode[5], nor1_result);
	 
	 not not7 (is_move_not, is_move);
	
	 // move
    and and014 (and14_result, op4_not, op3_not, op2_not, op1_not, op0_not);
    and and103 (move, opcode[5], and14_result);

    // branch signal
    and and3 (branch, opcode[5], opcode[0]);
    not not8 (branch_not, branch);
    
    //alusrc signal
    not not91 (notRtype, isRtype);
    or or30 (ALUsrcResult, notRtype, is_move);
    and and92 (ALUsrc, ALUsrcResult, branch_not);

    // regdst signal
    and and4 (and4_result, op1_not, op2_not);
    and and5 (and5_result, op3_not, op4_not);
    and and6 (and6_result, and4_result, and5_result);
    and and20 (regDst, and6_result, is_move_not);
    
    // memtoreg signal
    and and7 (memToReg, op4_not, opcode[3]);
     
    // memwrite signal
    and and56 (memWrite, op3_not, opcode[4]);	 
    
	 // byteoperations
    xor xor73 (xor3_result, opcode[4], opcode[3]);
    and and122(byteOperations, xor3_result, opcode[0]);
	 
    // regwrite signal
    or or98 (notRegWrite, opcode[5], opcode[4]);
    not not79 (regWriteRes, notRegWrite);
    or or10 (regWrite, regWriteRes, move);
	 
    // jump	signal
    and and999 (jump, opcode[5], opcode[4], opcode[3]);
    
    
    
    
endmodule
/*module Control_unit (
    output  regDst,
    output  branch,
    output  memToReg, // Can also be used as memToReg
    output  memWrite,
    output  [2:0] ALUop, // Should be [2:0]
    output  ALUsrc,
    output  regWrite,
    output  jump,
    output  byteOperations, // Optional signal for lb and sb operations
    output  move, // Optional signal for MOVE operation
    input [5:0] opcode
);
// decision of r type
    and a1 (isRtype, ~opcode[0],  ~opcode[1], ~opcode[2], ~opcode[3], ~opcode[4],  ~opcode[5]);
    and a2 (regDst, isRtype, 1'b1);
    and a3 (regWrite, isRtype, 1'b1);
    and a4 (ALUsrc, ~isRtype, 1'b1);
    and a5 (jump, ~isRtype, 1'b1);
    and a6 (byteOperations, ~isRtype, 1'b1);
    and a7 (move, ~isRtype, 1'b1);
    and a8 (ALUop[0], isRtype, 1'b1);
    and a9 (ALUop[1], isRtype, 1'b1);
    and a10 (ALUop[2], isRtype, 1'b1);

    // decision of branch
    and a11 (branch, opcode[5],opcode[5]);

    // decison of memToReg 
    and a12 (memToReg, ~opcode[5], ~opcode[4], ~opcode[3]);

    // decision of memWrite
    and a13 (memWrite, opcode[4], opcode[4]);

    // decision of regWrite
    and n1 (regWrite, memToReg, 1'b1);

    // decision of Jump
    and a14 (jump, opcode[0],  opcode[1], opcode[2], ~opcode[3], ~opcode[4],  ~opcode[5]);

    // decision of byteOperations
    and a15 (byteOperationsLoad, ~opcode[0], ~opcode[1], opcode[2], ~opcode[3], ~opcode[4],  opcode[5]);
    and a16 (byteOperationsStore, ~opcode[0], opcode[1], ~opcode[2], ~opcode[3], ~opcode[4],  opcode[5]);
    or o1 (byteOperations, byteOperationsLoad, byteOperationsStore);

endmodule

always @* begin
        // Default values
        regDst = 0;
        branch = 0;
        memToReg = 0;
        memWrite = 0;
        ALUop = 3'b000; // Default ALUop value
        ALUsrc = 0;
        regWrite = 0;
        jump = 0;
        byteOperations = 0;
        move = 0;
        
        ALUop = 3'b111;

        // Decode opcode to determine control signals
        case (opcode)
            6'b000000: begin // R-type instruction
                regDst = 1;
                regWrite = 1;
                ALUsrc = 0;
                jump = 0;
                byteOperations = 0;
                move = 0;
            end
            6'b100011: begin // lw (load word) instruction
                memToReg = 1;
                ALUsrc = 1;
                regWrite = 1;
            end
            6'b101011: begin // sw (store word) instruction
                memWrite = 1;
                ALUsrc = 1;
            end
            6'b001000: begin // andi instruction
                ALUop = 3'b000;
                regWrite = 1;
            end
            6'b001101: begin // ori instruction
                ALUop = 3'b001;
                regWrite = 1;
            end
            6'b001010: begin // slti instruction
                ALUop = 3'b100;
                regWrite = 1;
            end
            6'b001100, 6'b100011, 6'b101011, 6'b100001, 6'b100000: begin // addi, lb, sb, lw, sw
                ALUop = 3'b101;
                regWrite = 1;
            end
            6'b001110, 6'b000100, 6'b000101, 6'b000000, 6'b000110: begin // subi, beq, bne, R-type instructions
                ALUop = 3'b111;
            end
            6'b000111: begin // MOVE instruction
                move = 1;
            end
        endcase

    end*/