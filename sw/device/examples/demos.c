// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/examples/demos.h"

#include <stdbool.h>

#include "sw/device/lib/arch/device.h"
#include "sw/device/lib/dif/dif_gpio.h"
#include "sw/device/lib/log.h"
#include "sw/device/lib/runtime/hart.h"
#include "sw/device/lib/spi_device.h"
#include "sw/device/lib/uart.h"

void gpio_startup_demo(dif_gpio_t *gpio) {
  LOG_INFO("Watch the LEDs!\n");

  // Give a LED pattern as startup indicator for 5 seconds.
  dif_gpio_all_write(gpio, 0xff00);
  for (int i = 0; i < 32; ++i) {
    usleep(100 * 1000);  // 100 ms
    dif_gpio_pin_write(gpio, 8 + (i % 8), (i / 8) % 2);
  }
  dif_gpio_all_write(gpio, 0x0000);  // All LEDs off.
}

/**
 * Mask for "valid" GPIO bits. The first byte represents switch inputs,
 * while byte 16 represents the FTDI bit.
 */
static const uint32_t kGpioMask = 0x100ff;

/**
 * Mask for the FTDI bit among the GPIO bits.
 */
static const uint32_t kFtdiMask = 0x10000;

uint32_t gpio_to_log_echo(dif_gpio_t *gpio, uint32_t prev_gpio_state) {
  uint32_t gpio_state;
  dif_gpio_all_read(gpio, &gpio_state);
  gpio_state &= kGpioMask;

  uint32_t state_delta = prev_gpio_state ^ gpio_state;
  for (int bit_idx = 0; bit_idx < 8; ++bit_idx) {
    bool changed = ((state_delta >> bit_idx) & 0x1) != 0;
    bool is_currently_set = ((gpio_state >> bit_idx) & 0x1) != 0;
    if (changed) {
      LOG_INFO("GPIO switch #%d changed to %d\n", bit_idx, is_currently_set);
    }
  }

  if ((state_delta & kFtdiMask) != 0) {
    if ((gpio_state & kFtdiMask) != 0) {
      LOG_INFO("FTDI control changed. Enable JTAG.\r\n");
    } else {
      LOG_INFO("FTDI control changed. Enable JTAG.\r\n");
    }
  }

  return gpio_state;
}

/**
 * Copies characters out of |bytes| to |out|, replacing non-printable characters
 * with a replacement value.
 */
static void copy_printable(const char *bytes, size_t bytes_len,
                           char replacement, char *out) {
  for (int i = 0; i < bytes_len; ++i) {
    char c = bytes[i];
    if (c < ' ' || c > '~') {
      c = replacement;
    }
    out[i] = c;
  }
  out[bytes_len] = '\0';
}

void spi_to_log_echo(void) {
  uint32_t spi_buf[8];
  size_t spi_len = spid_read_nb(spi_buf, sizeof(spi_buf));
  if (spi_len > 0) {
    uint32_t echo_word = spi_buf[0] ^ 0x01010101;
    spid_send(&echo_word, sizeof(echo_word));

    char spi_printable[sizeof(spi_buf) + 1];
    copy_printable((char *)spi_buf, sizeof(spi_buf), '?', spi_printable);
    LOG_INFO("SPI: %s\r\n", spi_printable);
  }
}

void uart_to_uart_and_gpio_echo(dif_gpio_t *gpio) {
  char rcv_char;
  while (uart_rcv_char(&rcv_char) != -1) {
    uart_send_char(rcv_char);
    dif_gpio_all_write(gpio, rcv_char << 8);
  }
}