module riscv_accel
(
    input clk,
    output [5:0] led
);

localparam WAIT_TIME = 13500000;
reg [5:0] ledCounter = 0;

reg [23:0] clockCounter = 0;

always @(posedge clk) begin
    clockCounter <= clockCounter + 1;
    if(clockCounter == WAIT_TIME) begin
        ledCounter <= ledCounter + 1;
        clockCounter <= 0;
    end
end

assign led = ledCounter;

endmodule