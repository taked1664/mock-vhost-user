#include <iostream>
#include <vector>

#include "packet.hpp"

#define INFO_CPU 2
#define SIZE_BATCH 32
#define SIZE_RING SIZE_BATCH

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

constexpr int_fast32_t SIZE_POOL = SIZE_RING * 2;
constexpr int_fast32_t NUM_MOD = SIZE_RING - 1;

enum rsource : uint_fast8_t {
	CLT,
	SRV,
};

enum dstatus : uint_fast8_t {
	INIT,
	PUSH,
	PULL,
};

class desc {
public:
	dstatus status;
	uint_fast8_t id;
	uint_fast8_t len;
	packet *entry;

	desc();

	void set_param(uint_fast8_t, dstatus);
	void delete_info(dstatus);
};

class ring {
public:
	uint_fast8_t rsrv_idx;
	uint_fast8_t recv_idx;
	uint_fast8_t proc_idx;
	uint_fast8_t size;
	desc descs[SIZE_RING];

	ring();
	void operator=(ring&&);

	void ipush(packet[SIZE_BATCH], packet[SIZE_BATCH], rsource, uint_fast8_t);
	void pull(packet[SIZE_BATCH], packet[SIZE_BATCH], uint_fast8_t);
	void init_descs();
};

#include "cbuffer.hpp"
