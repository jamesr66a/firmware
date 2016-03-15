
/*****************************************************************************
 *                   AUTOMATICALLY GENERATED FILE. DO NOT EDIT.              *
 ****************************************************************************/

#include "application.h"
#include "debug.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t siphash24(const void *src, unsigned long src_sz, const char key[16]);

#define BIG_ENDIAN 0

#if (!BIG_ENDIAN)
static uint32_t swap_32(uint32_t op) {
  return ((op >> 24) & 0xff) | ((op << 8) & 0xff0000) | ((op >> 8) & 0xff00) |
         ((op << 24) & 0xff000000);
}
#endif

static uint32_t ntohl(uint32_t netlong) {
#ifdef NETWORK_BIG_ENDIAN
#if BIG_ENDIAN
  return netlong;
#else
  return swap_32(netlong);
#endif
#else
  return netlong;
#endif
}

static uint32_t htonl(uint32_t hostlong) {
#ifdef NETWORK_BIG_ENDIAN
#if BIG_ENDIAN
  return hostlong;
#else
  return swap_32(hostlong);
#endif
#else
  return hostlong;
#endif
}

#if (!BIG_ENDIAN)
static uint16_t swap_16(uint16_t op) { return (op >> 8) | (op << 8); }
#endif

static uint16_t ntohs(uint16_t netshort) {
#ifdef NETWORK_BIG_ENDIAN
#if BIG_ENDIAN
  return netshort;
#else
  return swap_16(netshort);
#endif
#else
  return netshort;
#endif
}

static uint16_t htons(uint16_t hostshort) {
#ifdef NETWORK_BIG_ENDIAN
#if BIG_ENDIAN
  return hostshort;
#else
  return swap_16(hostshort);
#endif
#else
  return hostshort;
#endif
}

#include "RSSIData.pbo.h"

static bool has_field(const RSSIData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  return ((unsigned)msg->has_field[byte_index] & (1u << (i % CHAR_BIT - 1)));
}

static void set_field_present(RSSIData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] |= (1u << (i % CHAR_BIT - 1));
}

static void set_field_absent(RSSIData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] &= ~(1u << (i % CHAR_BIT - 1));
}

static void check_magic(const RSSIData *msg) {
  if (msg->magic != htonl(0xDEADBEEFu)) {
    errorCheck();
  }
}

void RSSIData_init(RSSIData *msg) {
  memset(msg, 0, sizeof(*msg));
  // TODO: generate this from a hash at compile time
  msg->magic = htonl(0xDEADBEEF);
}
/*
 * string bssid
 */
bool RSSIData_has_bssid(const RSSIData *msg) {
  check_magic(msg);
  return has_field(msg, 1);
}
size_t RSSIData_bssid(const RSSIData *msg, char *dest, size_t lim) {
  if (has_field(msg, 1)) {
    size_t char_count = (lim - 1 < msg->bssid_size ? lim - 1 : msg->bssid_size);
    memmove(dest, msg->bssid, char_count);
    dest[char_count] = '\0';
    return char_count;
  } else {
  }
}
void RSSIData_set_bssid(RSSIData *msg, const char *src, size_t src_len) {
  size_t char_count = (100 > src_len ? src_len : 100);
  memmove((void *)msg->bssid, src, char_count);
  msg->bssid_size = htonl(char_count);
  set_field_present(msg, 1);
}
void RSSIData_clear_bssid(RSSIData *msg) {
  if (has_field(msg, 1)) {
    set_field_absent(msg, 1);
    msg->bssid_size = 0;
  }
}
/*
 * int32 rssi
 */
bool RSSIData_has_rssi(const RSSIData *msg) {
  check_magic(msg);
  return has_field(msg, 2);
}
int16_t RSSIData_rssi(const RSSIData *msg) {
  check_magic(msg);
  if (has_field(msg, 2)) {
    return ntohs(msg->rssi);
  } else {
  }
}
void RSSIData_set_rssi(RSSIData *msg, int16_t value) {
  check_magic(msg);
  msg->rssi = htons(value);
  set_field_present(msg, 2);
}
void RSSIData_clear_rssi(RSSIData *msg) {
  check_magic(msg);
  msg->rssi = 0;
  set_field_absent(msg, 2);
}
static const char key[] = "scary spooky skeletons";
void RSSIData_to_bytes(RSSIData *msg, char *buf, uint32_t seq) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  msg->seq = seq;
  msg->siphash = siphash24((char *)msg + offset, sizeof(*msg) - offset,
                           key);
  if ((char*)msg != buf) {
    memmove((void *)buf, (void *)msg, sizeof(*msg));
  }
}

bool RSSIData_from_bytes(RSSIData *msg, const char *buf, uint32_t *seq_out) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  memmove((void *)msg, (void *)buf, sizeof(*msg));
  *seq_out = msg->seq;
  return (siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                    key) == msg->siphash);
}
