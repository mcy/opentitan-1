// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// Exclude below TL toggle coverage as these pins won't be changed in the comportable IPs
INSTANCE: tb.dut
Toggle tl_o.d_opcode [1] "logic tl_o.d_opcode[2:0]"
Toggle tl_o.d_opcode [2] "logic tl_o.d_opcode[2:0]"
Toggle tl_o.d_param "logic tl_o.d_param[2:0]"
Toggle tl_o.d_sink "logic tl_o.d_sink[0:0]"
Toggle tl_i.a_param "logic tl_i.a_param[2:0]"
