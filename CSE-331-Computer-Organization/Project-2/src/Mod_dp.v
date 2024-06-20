module Mod_dp (
  input clk,
  input rst,
  input [31:0] a,
  input [31:0] b,
  input [1:0] state
);

parameter INITIALIZE = 2'd0;
parameter SUBTRACT = 2'd1;
parameter COMPARE = 2'd2;
parameter DONE = 2'd3;

reg [31:0] temp;
reg [31:0] result;

initial begin
  result <= 32'b0;
end

always @(posedge clk or posedge rst) begin
  if (rst) begin
    temp <= 32'b0;
  end 
  else begin
    case (state)
      INITIALIZE: begin
        temp <= a;
      end
      SUBTRACT: begin
        temp <= temp - b;
      end
      COMPARE, DONE: begin
      end
      default: begin
        // Reset to zero by default
        temp <= 32'b0;
        result <= 32'b0;
      end
    endcase
  end
end


endmodule
