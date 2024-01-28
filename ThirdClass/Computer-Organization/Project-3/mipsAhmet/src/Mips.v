
module Mips (
  input wire clock
);
    reg [31:0] program_counter;
    wire [31:0] read_data1, read_data2;
    wire [31:0] instruction;

    wire byteOperations, ALUsrc,regWrite,jump, move;
    wire regDst, branch, memToReg, memWrite;
    wire [2:0] ALUop;
    wire [31:0] sign_ext_imm;
    wire [2:0] alu_ctr;
    wire [31:0] alu_result;
    wire zero_bit;
    wire [31:0] read_data;

    wire [31:0] jumpAddress;
    wire [31:0] shiftedJumpAddress;

    wire [31:0] write_data; 
    wire [31:0] shifted_address;

    wire [4:0] tempReg;
    wire [31:0] tempReg2, tempReg3, tempReg4, tempReg5, tempReg6, tempReg7, tempReg8;

    reg jumpAndLink;
    reg [31:0] registers [0:31];

    initial begin
        program_counter = 32'h00000000;
        $readmemb("registers.mem", registers);
    end

    Instruction_block instBlock (
        .instruction(instruction),
        .pc(program_counter)  // Using program_counter as the program counter (example)
    );

    // Instantiate modules
    Control_unit ctrlUnit (
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
        .opcode(instruction[31:26])
    );

    Mux2x1_5byte muxRegDst (
        .result(tempReg),
        .wire1(instruction[20:16]),
        .wire2(instruction[15:11]),
        .sel(regDst)
    );

    Register_block regBlock (
        .read_data1(read_data1),
        .read_data2(read_data2),
        .byteOperations(byteOperations),
        .write_data(write_data),
        .read_reg1(instruction[25:21]),
        .read_reg2(instruction[20:16]), // mux lazim
        .write_reg(tempReg),
        .regWrite(regWrite)
    );

    Sign_extend signExt (
        .sign_ext_imm(sign_ext_imm),
        .imm(instruction[15:0])  // Using lower bits of instruction as immediate (example)
    );

    Alu_control aluCtrl (
        .alu_ctr(alu_ctr),
        .function_code(instruction[5:0]),
        .ALUop(ALUop)
    );

    Mux2x1 muxALUSrc (
        .result(tempReg2),
        .wire1(read_data2),
        .wire2(sign_ext_imm),
        .sel(ALUsrc)
    );

    Alu aluUnit (
        .alu_result(alu_result),
        .zero_bit(zero_bit),
        .alu_src1(read_data1),
        .alu_src2(tempReg2), // mux lazım
        .alu_ctr(alu_ctr)
    );

    Memory_block memBlock (
        .read_data(read_data),
        .byteOperations(byteOperations),
        .address(alu_result), // 32-8 bit uyusmazlıgı
        .write_data(read_data2),
        .memRead(memToReg),
        .memWrite(memWrite)
    );

    Mux2x1 muxMemToReg (
        .result(write_data),
        .wire1(alu_result),
        .wire2(read_data),
        .sel(memToReg)
    );

    FullAdder_32Bit adder (
        .sum(tempReg4),
        .carry(_),
        .cin(1'b0),
        .a(program_counter),
        .b(32'h0004)
    );
    
    Shift_left_2 shiftLeft2 (
        .shifted_address(shifted_address),
        .address(sign_ext_imm)
    );

    FullAdder_32Bit adder2 (
        .sum(tempReg5),
        .carry(_),
        .cin(1'b0),
        .a(shifted_address),
        .b(tempReg4)
    );
    xor xor1 (xorResult, instruction[28], zero_bit);

    and a1(andResult, branch, xorResult);

    Mux2x1 muxBranch (
        .result(tempReg6),
        .wire1(tempReg4),
        .wire2(tempReg5),
        .sel(andResult)
    );

    Shift_left_2 shiftLeft2_2 (
        .shifted_address(shiftedJumpAddress),
        .address(instruction)
    );

    Bus_for_jump bfJump(
        .jumpAddress(jumpAddress),
        .shiftedJumpAddress(shiftedJumpAddress),
        .program_counter(program_counter)
    );

    Mux2x1 muxJump (
        .result(tempReg7),
        .wire1(tempReg6),
        .wire2(jumpAddress),
        .sel(jump)
    );

    and a41(isJR, ~instruction[31], ~instruction[30], ~instruction[29], ~instruction[28], ~instruction[27], ~instruction[26], instruction[3]);

    Control_for_Jr crJr(
        .goToAddress(tempReg8),
        .read_data1(instruction[25:0]),
        .jumpAddress(tempReg7),
        .isJR(isJR)
    );
    /*
    Mux2x1 jrMux (
        .result(tempReg8),
        .wire1(tempReg7),
        .wire2(read_data1),
        .sel(isJR)
    );
    */
    // program counter = tempReg6
    always @(posedge clock) begin
        program_counter <= tempReg8;

        jumpAndLink = jump & instruction[26];

        if(jumpAndLink) begin
            registers[31] = tempReg4;
            $writememb("registers.mem", registers);
        end
    
    end

endmodule
