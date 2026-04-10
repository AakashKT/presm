`default_nettype none

module CommandProcessor
(
    // Clock
    input extern_clock,

    // Reset
    input async_reset,

    // Host input
    input wire [7:0] in_host,
    input wire in_en,

    // Host output
    output reg [7:0] out_host,
    output reg out_en
);

    localparam CP_IDLE = 0;
    localparam CP_ADD_CMD_PACKET_WAIT = 1;
    localparam CP_ADD_CMD_PACKET_READ = 2;

    reg [2:0] cp_state;

    reg [32:0] addr1;
    reg [32:0] addr1_m;
    reg [32:0] addr1_n;

    reg [32:0] addr2;
    reg [32:0] addr2_m;
    reg [32:0] addr2_n;

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            cp_state <= CP_IDLE;
            out_host <= 0;
            out_en <= 0;
        end
        else
        begin
            case(cp_state)

                CP_IDLE:
                begin
                    if(in_en == 1)
                    begin
                        if(in_host == 8'h61)
                        begin
                            cp_state <= CP_ADD_CMD_PACKET_WAIT;
                        end
                    end
                end

                CP_ADD_CMD_PACKET_WAIT:
                begin
                    if(in_en == 0)
                    begin
                        cp_state <= CP_ADD_CMD_PACKET_READ;
                    end
                end

                CP_ADD_CMD_PACKET_READ:
                begin
                    if(in_en == 1)
                    begin
                        out_host <= in_host;
                        out_en <= 1;
                        cp_state <= CP_IDLE;
                    end
                end

            endcase
        end
    end

endmodule