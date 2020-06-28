/*
 * ZX80CPU.h
 *
 *  Created on: 28 июн. 2020 г.
 *      Author: Wertiz
 */

#ifndef Z80_H_
#define Z80_H_

#include "Memory.h"

#pragma pack(push, 1)
struct Z80Regs1
{
	union{
		uint16_t af;
		struct {
			uint8_t f; //младшая
			uint8_t a; //старшая часть
		};
	};
	union{
			uint16_t bc;
			struct {
				uint8_t c; //младшая
				uint8_t b; //старшая часть
			};
		};
	union{
			uint16_t de;
				struct {
					uint8_t e; //младшая
					uint8_t d; //старшая часть
				};
			};
	union{
			uint16_t hl;
					struct {
						uint8_t l; //младшая
						uint8_t h; //старшая часть
					};
				};
	union{
		uint16_t ix;
		struct {
			uint8_t ixl; //младшая
			uint8_t ixh; //старшая часть
		};
	};
	union{
		uint16_t iy;
		struct {
			uint8_t iyl; //младшая
			uint8_t iyh; //старшая часть
		};
	};

	uint16_t pc;
	uint16_t sp;

	uint8_t i;
	uint8_t r;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Z80Regs2
{
	union{
		uint16_t af;
		struct {
			uint8_t f; //младшая
			uint8_t a; //старшая часть
		};
	};
	union{
			uint16_t bc;
			struct {
				uint8_t c; //младшая
				uint8_t b; //старшая часть
			};
		};
	union{
			uint16_t de;
				struct {
					uint8_t e; //младшая
					uint8_t d; //старшая часть
				};
			};
	union{
			uint16_t hl;
					struct {
						uint8_t l; //младшая
						uint8_t h; //старшая часть
					};
				};
};
#pragma pack(pop)

class Z80
{
protected:
	AddressSpace & _bus;
	Z80Regs1 _regs {};
	Z80Regs2 _shadow_regs {};
	bool iff1 {false};
	bool iff2 {false};


	unsigned _ticks {0}; //счетчик тиков
	unsigned _state {0};

	uint8_t _prefix {0};
	uint8_t _instruction {0};
	uint8_t _wait {0};

	uint8_t get_reg(uint8_t) const;

public:
	Z80(AddressSpace & bus): _bus(bus) {}

	void tick();
	void reset();
};



#endif /* Z80_H_ */
