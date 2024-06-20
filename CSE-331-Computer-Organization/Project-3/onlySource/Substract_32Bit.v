module Substract_32Bit(input [31:0] A, input [31:0] B, output [31:0] Result, output Cout);
    
    wire temp;
    wire [31:0] B_inverted;
    Inverter inv(B, B_inverted);

    FullAdder_32Bit fa(
        .a(A),
        .b(B_inverted),
        .cin(1'b1),
        .sum(Result),
        .carry(temp)
        );

    xor x1(Cout, temp, 1'b1);
    // carry ou varsa eksi sayidir ve twos complementi alınabilir
endmodule   



module Inverter(input [31:0] inverter, output [31:0] inverted);

    xor xo1 (inverted[0], inverter[0], 1'b1);
    xor xo2 (inverted[1], inverter[1], 1'b1);
    xor xo3 (inverted[2], inverter[2], 1'b1);
    xor xo4 (inverted[3], inverter[3], 1'b1);
    xor xo5 (inverted[4], inverter[4], 1'b1);
    xor xo6 (inverted[5], inverter[5], 1'b1);
    xor xo7 (inverted[6], inverter[6], 1'b1);
    xor xo8 (inverted[7], inverter[7], 1'b1);
    xor xo9 (inverted[8], inverter[8], 1'b1);
    xor xo10 (inverted[9], inverter[9], 1'b1);
    xor xo11 (inverted[10], inverter[10], 1'b1);
    xor xo12 (inverted[11], inverter[11], 1'b1);
    xor xo13 (inverted[12], inverter[12], 1'b1);
    xor xo14 (inverted[13], inverter[13], 1'b1);
    xor xo15 (inverted[14], inverter[14], 1'b1);
    xor xo16 (inverted[15], inverter[15], 1'b1);
    xor xo17 (inverted[16], inverter[16], 1'b1);
    xor xo18 (inverted[17], inverter[17], 1'b1);
    xor xo19 (inverted[18], inverter[18], 1'b1);
    xor xo20 (inverted[19], inverter[19], 1'b1);
    xor xo21 (inverted[20], inverter[20], 1'b1);
    xor xo22 (inverted[21], inverter[21], 1'b1);
    xor xo23 (inverted[22], inverter[22], 1'b1);
    xor xo24 (inverted[23], inverter[23], 1'b1);
    xor xo25 (inverted[24], inverter[24], 1'b1);
    xor xo26 (inverted[25], inverter[25], 1'b1);
    xor xo27 (inverted[26], inverter[26], 1'b1);
    xor xo28 (inverted[27], inverter[27], 1'b1);
    xor xo29 (inverted[28], inverter[28], 1'b1);
    xor xo30 (inverted[29], inverter[29], 1'b1);
    xor xo31 (inverted[30], inverter[30], 1'b1);
    xor xo32 (inverted[31], inverter[31], 1'b1);

endmodule