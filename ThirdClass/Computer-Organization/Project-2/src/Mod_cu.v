module Mod_cu(
  input clk,
  input rst,
  input start,
  input [31:0] a,
  input [31:0] b,
  output wire [31:0] result,
  output wire done
);

parameter INITIALIZE = 2'd0;
parameter SUBTRACT = 2'd1;
parameter COMPARE = 2'd2;
parameter DONE = 2'd3;

reg [1:0] state, next_state;
reg [31:0] temp;

always @(posedge clk) begin
  if (rst) begin
    state <= INITIALIZE;
  end 
  else begin
    state <= next_state;
  end
end

initial begin
  temp <= 32'b0;
end

always @(negedge clk) begin
  if (start) begin
    case (next_state)
      INITIALIZE: begin
        temp <= a;
        next_state <= COMPARE;
      end
      SUBTRACT: begin
        temp <= temp - b;
        next_state <= COMPARE;
      end
      COMPARE: begin
        if (temp < b) begin
          next_state <= DONE;
        end else begin
          next_state <= SUBTRACT;
        end
      end
      DONE: begin
        next_state <= DONE;
      end
      default: begin
        next_state <= INITIALIZE;
      end
    endcase
  end
end

assign result = temp;
assign done = (next_state == DONE);

endmodule
