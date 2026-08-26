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

    output wire [63:0] rx_packet_flat,
    output reg rx_packet_ready,

    input wire [63:0] tx_packet_flat,
    input wire tx_packet_ready
);

    reg [7:0] rx_packet[8];
    assign rx_packet_flat[7:0] = rx_packet[0];
    assign rx_packet_flat[15:8] = rx_packet[1];
    assign rx_packet_flat[23:16] = rx_packet[2];
    assign rx_packet_flat[31:24] = rx_packet[3];
    assign rx_packet_flat[39:32] = rx_packet[4];
    assign rx_packet_flat[47:40] = rx_packet[5];
    assign rx_packet_flat[55:48] = rx_packet[6];
    assign rx_packet_flat[63:56] = rx_packet[7];

    wire [7:0] tx_packet[8];
    assign tx_packet[0] = tx_packet_flat[7:0];
    assign tx_packet[1] = tx_packet_flat[15:8];
    assign tx_packet[2] = tx_packet_flat[23:16];
    assign tx_packet[3] = tx_packet_flat[31:24];
    assign tx_packet[4] = tx_packet_flat[39:32];
    assign tx_packet[5] = tx_packet_flat[47:40];
    assign tx_packet[6] = tx_packet_flat[55:48];
    assign tx_packet[7] = tx_packet_flat[63:56];

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
                    if(rx_data_en == 0)
                    begin
                        rx_state <= RX_HEADER_RECEIVE;
                        rx_packet_idx <= 0;
                    end
                end

                RX_HEADER_RECEIVE_WAIT:
                begin
                    if(rx_packet_idx == 4)
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
                    if(rx_packet_idx == rx_packet[3])
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

    reg [2:0] tx_state;
    reg [4:0] tx_packet_idx;

    localparam TX_IDLE = 0;
    localparam TX_SEND = 1;
    localparam TX_SEND_WAIT = 2;
    localparam TX_END = 3;

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            tx_data_en <= 0;

            tx_state <= TX_IDLE;
            tx_packet_idx <= 0;
        end
        else
        begin
            case(tx_state)
                
                TX_IDLE:
                begin
                    if(tx_packet_ready == 1)
                    begin
                        tx_packet_idx <= 0;
                        tx_state <= TX_SEND;
                    end
                end

                TX_SEND:
                begin
                    if(tx_data_sent == 0)
                    begin
                        tx_data <= tx_packet[tx_packet_idx];
                        tx_data_en <= 1;
                        tx_packet_idx <= tx_packet_idx + 1;

                        tx_state <= TX_SEND_WAIT;
                    end
                    else
                    begin
                       tx_data_en <= 0; 
                    end
                end

                TX_SEND_WAIT:
                begin
                    if(tx_data_sent == 1)
                    begin
                        if(tx_packet_idx == tx_packet[3] + 4)
                        begin
                            tx_state <= TX_END;
                        end
                        else
                        begin
                            tx_data_en <= 0;
                            tx_state <= TX_SEND;
                        end
                    end
                end

                TX_END:
                begin
                    if(tx_packet_ready == 0)
                    begin
                        tx_state <= TX_IDLE;
                    end
                end
    
            endcase
        end
    end
    
endmodule