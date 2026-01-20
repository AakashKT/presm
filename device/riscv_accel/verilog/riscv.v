
`default_nettype none

`include "../../verilog/uart.v"

module riscv_accel
#(
    parameter DELAY_FRAMES = 234 // 27,000,000 (27Mhz) / 115200 Baud rate
)
(
    input _extern_27mhz_clock,
    input _extern_uart_rx,
    input _extern_reset,
    output _extern_uart_tx,
    output [5:0] _extern_led
);

    reg [7:0] dataIn;
    wire byte_ready;
    UART uart_comm(
        _extern_27mhz_clock,
        _extern_uart_rx,
        _extern_reset,
        _extern_uart_tx,
        dataIn,
        byte_ready
    );

    always @(posedge _extern_27mhz_clock) begin
        if (byte_ready) begin
            _extern_led <= ~dataIn[7:2];
        end
    end






    // localparam WAIT_TIME = 13500000;

    // reg [23:0] clockCounter = 0;

    // wire [5:0] ledRegisterIn;
    // wire ledRegisterEn;
    // wire ledRegisterClear;
    // wire ledRegisterReset;

    // RegisterDFF #(3'd6) ledRegister(
    //     _extern_27mhz_clock,
    //     ledRegisterIn,
    //     ledRegisterEn,
    //     ledRegisterClear,
    //     ledRegisterReset,
    //     _extern_led
    // );

    // always @(posedge _extern_27mhz_clock) 
    // begin
    //     if(clockCounter == 0)
    //     begin
    //         ledRegisterIn = 5'd16;
    //         ledRegisterClear = 1'b1;
    //         ledRegisterReset = 1'b0;
    //         ledRegisterEn = 1'b1;
    //     end

    //     clockCounter <= clockCounter + 1;
    // end

endmodule
