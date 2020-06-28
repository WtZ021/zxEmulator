/*
 * ZX80CPU.h
 *
 *  Created on: 28 ���. 2020 �.
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
			uint8_t f; //�������
			uint8_t a; //������� �����
		};
	};
	union{
			uint16_t bc;
			struct {
				uint8_t c; //�������
				uint8_t b; //������� �����
			};
		};
	union{
			uint16_t de;
				struct {
					uint8_t e; //�������
					uint8_t d; //������� �����
				};
			};
	union{
			uint16_t hl;
					struct {
						uint8_t l; //�������
						uint8_t h; //������� �����
					};
				};
	union{
		uint16_t ix;
		struct {
			uint8_t ixl; //�������
			uint8_t ixh; //������� �����
		};
	};
	union{
		uint16_t iy;
		struct {
			uint8_t iyl; //�������
			uint8_t iyh; //������� �����
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
			uint8_t f; //�������
			uint8_t a; //������� �����
		};
	};
	union{
			uint16_t bc;
			struct {
				uint8_t c; //�������
				uint8_t b; //������� �����
			};
		};
	union{
			uint16_t de;
				struct {
					uint8_t e; //�������
					uint8_t d; //������� �����
				};
			};
	union{
			uint16_t hl;
					struct {
						uint8_t l; //�������
						uint8_t h; //������� �����
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


	unsigned _ticks {0}; //������� �����
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
