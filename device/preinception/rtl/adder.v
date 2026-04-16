`default_nettype none

module Adder
(
    // Clock
    input extern_clock,

    // Reset
    input async_reset,

    input wire rdy,

    input wire signed [31:0] ra,
    input wire signed [31:0] rb,

    output reg signed [31:0] rc,
    output reg rc_rdy
);

    localparam ADDER_IDLE = 0;
    localparam ADDER_COMPUTE = 1;
    localparam ADDER_END = 2;

    reg [1:0] adder_state;

    always @(posedge extern_clock or posedge async_reset)
    begin
        if(async_reset)
        begin
            rc <= 0;
            rc_rdy <= 0;

            adder_state <= ADDER_IDLE;
        end
        else
        begin
            case(adder_state)

                ADDER_IDLE:
                begin
                    if(rdy == 1)
                    begin
                        rc_rdy <= 0;
                        adder_state <= ADDER_COMPUTE;
                    end
                end

                ADDER_COMPUTE:
                begin
                    rc <= ra + rb;
                    rc_rdy <= 1;
                    adder_state <= ADDER_END;
                end

                ADDER_END:
                begin
                    if(rdy == 0)
                    begin
                        adder_state <= ADDER_IDLE;
                    end
                end

            endcase
        end
    end

endmodule