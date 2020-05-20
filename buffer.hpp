#include <iostream>
#include <vector>

#include "packet.hpp"

#define NUM_THREAD 64
#define SIZE_RING NUM_THREAD * 2
#define NUM_MOD (SIZE_RING - 1)
#define SIZE_POOL SIZE_RING * 2

static uint32_t nums[NUM_THREAD + 1];

enum rsource {
	CLT = 0,
	SRV = 1,
};

enum dstatus {
	INIT,
	PUSH,
	PULL,
};

class desc {
public:
	dstatus status;
	uint16_t id;
	uint16_t len;
	bool is_used;
	packet *entry;

	desc();
	desc(dstatus);

	void set_param(packet, uint16_t, dstatus);
	void delete_info(dstatus);
};

class ring {
#if 1
private:
	std::mutex pool_mtx;
#endif
public:
	uint16_t rsrv_idx[NUM_THREAD];
	uint16_t recv_idx[NUM_THREAD];
	uint16_t proc_idx[NUM_THREAD];
	uint16_t size;
	desc descs[SIZE_RING];

	ring();
	ring(const ring&);
	ring(ring&&);
	ring& operator=(const ring&);
	ring&& operator=(ring&&);

	uint16_t get_index(packet*, rsource, short);
	bool push(packet, packet*, rsource, short);
	bool dinit(short);
	packet pull(packet*, short);
	void init_descs();
};

void set_packet_nums(uint32_t*);
