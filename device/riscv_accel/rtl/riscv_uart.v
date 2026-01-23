
`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"

module RiscvUart
#(
    parameter DELAY_FRAMES = 234 // 27,000,000 (27Mhz) / 115200 Baud rate
)
(
    input _extern_clock,
    input _extern_uart_rx,
    output _extern_uart_tx,
    output [5:0] _extern_led,
);

    reg [7:0] dataIn;
    reg dataInReady;

    reg [7:0] dataOut = 8'b01100001;
    reg dataOutReady = 0;
    reg [31:0] dataOutReadyCount = 0;

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
        dataOutReady
    );

    always @(posedge _extern_clock) 
    begin
        dataOutReady <= dataInReady;
        dataOut <= dataIn;
    end

endmodule