dtmc

module main
    s : [-1..3] init -1;
    [] true -> 0.5:(s'=max(-1,s-1)) + 0.5:(s'=min(3,s+1));
endmodule

