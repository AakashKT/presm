`default_nettype none

module CommandProcessor
(
    // Clock
    input _extern_clock,

    // Reset
    input _extern_async_reset,

    // Host input
    input wire [31:0] in_host,
    input wire in_en,

    // Host output
    output reg [31:0] out_host,
    output reg out_en
);

    localparam CP_IDLE = 0;
    localparam CP_INIT = 1;
    localparam CP_ACTIVE = 2;
    localparam CP_STOP = 3;

    reg [2:0] cp_state;

    reg [31:0] r1, r2;

    always @(posedge _extern_async_reset)
    begin
        cp_state <= CP_IDLE;
        out_en <= 0;
        out_host <= 0;

        r1 <= 0;
        r2 <= 0;
    end

    always @(posedge _extern_clock)
    begin
        case(cp_state)

            CP_IDLE:
            begin
                if(in_en == 1)
                begin
                    cp_state <= CP_INIT;
                end
            end

            CP_INIT:
            begin
                r1[15:0] = in_host[31:16];
                r2[15:0] = in_host[15:0];
                cp_state = CP_ACTIVE;
            end

            CP_ACTIVE:
            begin
                out_host = r1 + r2;
                cp_state = CP_STOP;
            end

            CP_STOP:
            begin
                out_en <= 1;
                if(in_en == 0)
                begin
                    out_en <= 0;
                    cp_state = CP_IDLE;
                end
            end

        endcase
    end

endmodule