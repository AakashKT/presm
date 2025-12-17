module playground
(
    input in_1,
    input in_2,
    input in_3,

    output out_1
);
    assign out_1 = in_3 ? in_2 : in_1;

endmodule