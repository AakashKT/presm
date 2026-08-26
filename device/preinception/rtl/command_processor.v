`default_nettype none

module CommandProcessor
(
    // Clock
    input extern_clock,

    // Reset
    input extern_reset,

    // Host Communication
    input wire [63:0] rx_packet,
    input wire rx_packet_ready,

    output reg [63:0] tx_packet,
    output reg tx_packet_ready
);

    localparam CP_IDLE = 0;
    localparam CP_HANDSHAKE_PREP = 1;
    localparam CP_HANDSHAKE = 2;
    localparam CP_STOP = 3;

    reg [2:0] cp_state;

    always @(posedge extern_clock or posedge extern_reset)
    begin
        if(extern_reset)
        begin
            cp_state <= CP_IDLE;
        end
        else
        begin
            case(cp_state)
                CP_IDLE:
                begin
                    if(rx_packet_ready == 1)
                    begin
                        if(rx_packet[23:16] == 8'd0)
                        begin
                            cp_state <= CP_HANDSHAKE_PREP;
                        end
                    end
                end

                CP_HANDSHAKE_PREP:
                begin
                    tx_packet_ready <= 0;
                    cp_state <= CP_HANDSHAKE;
                end

                CP_HANDSHAKE:
                begin
                    tx_packet[7:0] <= 1;
                    tx_packet[15:8] <= 0;
                    tx_packet[23:16] <= 8'd2;
                    tx_packet[31:24] <= 8'd1;
                    tx_packet_ready <= 1;

                    cp_state <= CP_STOP;
                end

                CP_STOP:
                begin
                    if(rx_packet_ready == 0)
                    begin
                        cp_state <= CP_IDLE;
                    end
                end
            endcase 
        end
    end

endmodule