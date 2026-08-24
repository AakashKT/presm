`default_nettype none

`include "uart_rx.v"
`include "uart_tx.v"

module UARTPacket 
(
    // Clock
    input extern_clock,

    // UART rx & tx
    input wire extern_uart_rx,
    output wire extern_uart_tx,

    // Hardware reset signal
    input extern_reset,

    output reg [7:0] rx_packet[36],
    output reg rx_packet_ready,

    input wire [7:0] tx_packet[36],
    input wire tx_packet_ready
);

    wire [7:0] rx_data;
    wire rx_data_en;

    UARTRx #(.DELAY_WAIT(234)) receiver(
        extern_clock,
        extern_reset,
        extern_uart_rx,
        rx_data,
        rx_data_en
    );

    reg [2:0] rx_state;
    reg [4:0] rx_packet_idx;

    localparam RX_IDLE = 0;
    localparam RX_HEADER_RECEIVE_WAIT = 1;
    localparam RX_HEADER_RECEIVE = 2;
    localparam RX_BODY_RECEIVE_WAIT = 3;
    localparam RX_BODY_RECEIVE = 4;
    localparam RX_END = 5;

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            rx_packet_ready <= 0;

            rx_state <= RX_IDLE;
            rx_packet_idx <= 0;
        end
        else
        begin
            case(rx_state)
                RX_IDLE:
                begin
                    if(rx_data_en == 1)
                    begin
                        if(rx_data == 8'd1)
                        begin
                            rx_state <= RX_HEADER_RECEIVE_WAIT;

                            rx_packet[0] <= 1;
                            rx_packet_idx <= 1;
                        end
                    end
                end

                RX_HEADER_RECEIVE_WAIT:
                begin
                    if(rx_packet_idx == 3'd4)
                    begin
                        rx_packet_idx <= 0;
                        rx_state <= RX_BODY_RECEIVE_WAIT;
                    end
                    else if(rx_data_en == 0)
                    begin
                        rx_state <= RX_HEADER_RECEIVE;
                    end
                end

                RX_HEADER_RECEIVE:
                begin
                    if(rx_data_en == 1)
                    begin
                        rx_packet[rx_packet_idx] <= rx_data;
                        rx_packet_idx <= rx_packet_idx + 1;

                        rx_state <= RX_HEADER_RECEIVE_WAIT;
                    end
                end

                RX_BODY_RECEIVE_WAIT:
                begin
                    if(rx_packet_idx == rx_packet[1])
                    begin
                        rx_state <= RX_END;
                    end
                    else if(rx_data_en == 0)
                    begin
                        rx_state <= RX_BODY_RECEIVE;
                    end
                end

                RX_BODY_RECEIVE:
                begin
                    if(rx_data_en == 1)
                    begin
                        rx_packet[rx_packet_idx + 4] <= rx_data;
                        rx_packet_idx <= rx_packet_idx + 1;

                        rx_state <= RX_BODY_RECEIVE_WAIT;
                    end
                end

                RX_END:
                begin
                    rx_packet_ready <= 1;

                    if(rx_data_en == 0)
                    begin
                        rx_packet_ready <= 0;
                        rx_state <= RX_IDLE;
                    end
                end
            endcase
        end
    end

    reg [7:0] tx_data;
    reg tx_data_en;
    wire tx_data_sent;

    UARTTx #(.DELAY_WAIT(234)) transmitter(
        extern_clock,
        extern_reset,
        extern_uart_tx,
        tx_data,
        tx_data_en,
        tx_data_sent
    );

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            rx_packet_ready <= 0;

            rx_state <= RX_IDLE;
            rx_packet_idx <= 0;
        end
    end
    
endmodule