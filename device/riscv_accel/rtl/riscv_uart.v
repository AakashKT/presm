
`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"

module riscv_uart
#(
    parameter DELAY_FRAMES = 234 // 27,000,000 (27Mhz) / 115200 Baud rate
)
(
    input _extern_clock,
    input _extern_uart_rx,
    output _extern_uart_tx,
    output [5:0] _extern_led,
    input _extern_reset
);

    reg [7:0] dataIn;
    reg dataInReady;

    reg [7:0] dataOut = 8'b01100001;
    reg dataOutReady = 1;

    assign _extern_led = ~dataIn[5:0];

    UARTRx uart_rx(
        _extern_clock,
        _extern_uart_rx,
        dataIn,
        dataInReady
    );

    UARTTx uart_tx(
        _extern_clock,
        _extern_uart_tx,
        dataOut,
        ~_extern_reset
    );

    // always @(posedge _extern_clock) begin
    //     if (dataInReady) begin
    //         _extern_led <= ~dataIn[5:0];
    //     end
    // end

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