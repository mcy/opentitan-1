// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// SECDED encoder generated by util/design/secded_gen.py

module prim_secded_hamming_72_64_enc (
  input        [63:0] in,
  output logic [71:0] out
);

  always_comb begin : p_encode
    out = 72'(in);
    out[64] = ^(out & 72'h00AB55555556AAAD5B);
    out[65] = ^(out & 72'h00CD9999999B33366D);
    out[66] = ^(out & 72'h00F1E1E1E1E3C3C78E);
    out[67] = ^(out & 72'h0001FE01FE03FC07F0);
    out[68] = ^(out & 72'h0001FFFE0003FFF800);
    out[69] = ^(out & 72'h0001FFFFFFFC000000);
    out[70] = ^(out & 72'h00FE00000000000000);
    out[71] = ^(out & 72'h7FFFFFFFFFFFFFFFFF);
  end

endmodule : prim_secded_hamming_72_64_enc
