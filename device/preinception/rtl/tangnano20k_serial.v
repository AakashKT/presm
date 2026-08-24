`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"
`include "command_processor.v"

module Tangnano20kUart
(
    // Clock
    input extern_clock,

    // UART rx & tx
    input wire extern_uart_rx,
    output wire extern_uart_tx,

    // Hardware reset signal
    input extern_reset,

    output reg [5:0] extern_led
);

    wire [7:0] rx_data;
    wire rx_data_en;

    reg [7:0] rx_packet [4];

    reg [7:0] tx_data;
    reg tx_data_en;
    wire tx_data_sent;

    // Submodules
    UARTRx #(.DELAY_WAIT(234)) receiver(
        extern_clock,
        extern_reset,
        extern_uart_rx,
        rx_data,
        rx_data_en
    );

    UARTTx #(.DELAY_WAIT(234)) transmitter(
        extern_clock,
        extern_reset,
        extern_uart_tx,
        tx_data,
        tx_data_en,
        tx_data_sent
    );

    reg [2:0] rx_state;
    reg [2:0] packet_idx;

    localparam IDLE = 0;
    localparam RECEIVE_WAIT = 1;
    localparam RECEIVE = 2;
    localparam PROCESS = 3;
    localparam PROCESS_WAIT = 4;

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            rx_state <= IDLE;
            packet_idx <= 0;
            tx_data_en <= 0;

            extern_led <= 0;
        end
        else
        begin
            case(rx_state)
                IDLE:
                begin
                    if(rx_data_en == 1)
                    begin
                        if(rx_data == 8'd1)
                        begin
                            rx_state <= RECEIVE_WAIT;
                            tx_data_en <= 0;
                            packet_idx <= 0;
                        end
                    end
                end

                RECEIVE_WAIT:
                begin
                    if(packet_idx == 3'd4)
                    begin
                        rx_state <= PROCESS;
                    end
                    else if(rx_data_en == 0)
                    begin
                        rx_state <= RECEIVE;
                    end
                end

                RECEIVE:
                begin
                    if(rx_data_en == 1)
                    begin
                        rx_packet[packet_idx] <= rx_data;
                        packet_idx <= packet_idx + 1;

                        rx_state <= RECEIVE_WAIT;
                    end
                end

                PROCESS:
                begin
                    if(rx_packet[2] == 8'd1) // Identity
                    begin
                        extern_led <= PROCESS;

                        tx_data <= 8'd3;
                        tx_data_en <= 1;

                        rx_state <= PROCESS_WAIT;
                    end
                end

                PROCESS_WAIT:
                begin
                    if(tx_data_sent == 1 && rx_data_en == 0)
                    begin
                        tx_data_en <= 0;
                        rx_state <= IDLE;
                    end
                end
            endcase
        end
    end

endmodule