`default_nettype none

module CommandProcessor
(
    // Clock
    input _extern_clock,

    // Reset
    input async_reset,

    // Host input
    input wire [7:0] in_host,
    input wire in_en,

    // Host output
    output reg [7:0] out_host,
    output reg out_en
);

    // Convenient definitions
    localparam CP_IDLE = 0;
    localparam CP_INIT = 1;
    localparam CP_ACTIVE = 2;
    localparam CP_STOP = 3;

    // Registers
    reg [2:0] cp_state;
    reg [7:0] r1;

    // CP state machine
    always @(posedge _extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            cp_state <= CP_IDLE;
        end
        else
        begin
            case(cp_state)

                CP_IDLE:
                begin
                    if(in_en == 1)
                    begin
                        r1 <= 0;
                        out_host <= 0;
                        out_en <= 0;

                        cp_state <= CP_INIT;
                    end
                end

                CP_INIT:
                begin
                    cp_state <= CP_ACTIVE;
                    r1 <= in_host;
                end

                CP_ACTIVE:
                begin
                    out_host = r1;
                    cp_state <= CP_STOP;
                end

                CP_STOP:
                begin
                    out_en <= 1;
                    if(in_en == 0)
                    begin
                        out_en <= 0;
                        cp_state <= CP_IDLE;
                    end
                end

            endcase
        end
    end

endmodule