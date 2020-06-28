/*
 * Z80CPU.cc
 */

#include <sstream>
#include <iomanip>
#include <iostream>
#include "z80.h"

static bool par_even(uint8_t v)
{
	uint8_t res = v;
	res ^= (res >> 4);
	res ^= (res >> 2);
	res ^= (res >> 1);
	return (res&1) == 0;
}

void Z80::tick() {
	uint8_t f1, f2, f3;
	uint16_t t1;
	uint8_t b1;
	uint8_t c1;
	int8_t sb1;

	if (_state == 0) { //������� ���������� ��� ��������
		//������� ����:
		_instruction = _bus.read(_regs.pc);	//����� ��������� ���������� ������ � _regs.pc
		if (unsigned(_instruction ) == (0xCB || 0xED || 0xDD || 0xFD || 0xDDCB || 0xFDCB))
			std::cout << "�������" << std::endl;
		//TODO: ���� ��� �������, �� �������� � ���������� ����
		//������ ����
		_state = 2;
	} else if (_state == 1) { //������� ���������� � ���������
		_wait += 4;

		_state = 2;
	} else if (_state == 2) { //������� ���������, ���� ����, � ���� �� ��� ����������
		f1 = (_instruction >> 6) & 0x03;
		f2 = (_instruction >> 3) & 0x07;
		f3 = (_instruction >> 0) & 0x07;

		switch (_instruction) {
		case 0x00: //NOP           �.�. 4 �����
			_wait = 2;
			_regs.pc++; //�� � �������� �����
			break;

		case 0x08:  //EX AF, AF'
			_wait = 2;
			_regs.pc++;

			t1 = _regs.af;
			_regs.af = _shadow_regs.af;
			_shadow_regs.af = t1;
			break;

		//!!!
		/*case 0x10: //DJNZ d
			_wait = 11; //FIXME: 8 ������
			sb1 = static_cast<int8_t>(_bus.read(_regs.pc + 1));
			//_regs.pc++;
			_regs.b -= 1;
			if (_regs.b != 0) _regs.pc += sb1;
			break;
*/
		case 0x18:  //JR d
			_wait = 10;
			sb1 = static_cast<int8_t>(_bus.read(_regs.pc + 1)); //������������ � �������� ����������
			_regs.pc += sb1; // ���������, ����� ��� 2 ��������� (������ JR d)
			break;

		case 0x32:  //LD [nn], A
			_wait = 11;
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;

			_bus.write(t1, _regs.a);
			break;

		case 0x61: { //LD DDD, SSS 01 100 001
			_wait = 4; //FIXME: ��� [HL] 15
			_regs.pc++;

			b1 = get_reg(f2);
			c1 = get_reg(f3);
			_bus.write(get_reg(_regs.h), _regs.c); //FIXME: get_reg(_regs.h) - �������� ��������
		}
			break;

		case 0x6d: { //LD DDD, SSS 01 101 101
			_wait = 4; //FIXME: ��� [HL] 15
			_regs.pc++;

			b1 = get_reg(f2);
			c1 = get_reg(f3);
			_bus.write(get_reg(_regs.h), _regs.c); //FIXME: get_reg(_regs.h) - �������� ��������
		}
			break;

		case 0x70 ... 0x75: { //LD [HL], SSS
			_wait = 7; //FIXME: ��� [HL] 15
			_regs.pc++;

			b1 = get_reg(f3);
			_bus.write(b1, _regs.hl);
		} break;

		case 0xa8 ... 0xaf: { //�������� XOR A, SSS - �������� ���������
			_wait = 2; //FIXME: ��� [HL], [IX+d] � [IY+d] ������ ��������
			_regs.pc++;

			b1 = get_reg(f3);
			_regs.a ^= b1; //����������� ���
			// ����� �����
			_regs.f = _regs.a & 0xa8;			// ����� S, 5 � 3, ��������� 0
			_regs.f |= (_regs.a == 0) ? 0x40 : 0x00;	// ���� Z
			_regs.f |= par_even(_regs.a) ? 0x04 : 0x00;	// ���� P/V

			break;
		}
		/*case 0xc2: // 11 000 010	JP NZ, (nn)
			_wait = 8;
			b1 = get_reg(f2);
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;
			if (f2) { //FIXME: ��������� ���
				_regs.pc = t1;
			}

			break;
		case 0xca: // 11 001 010	JP Z, (nn)
			_wait = 8;
			b1 = get_reg(f2);
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;
			if (f2) { //FIXME: ��������� ���
				_regs.pc = t1;
			}

			break;
		case 0xd2: // 11 010 010	JP NC, (nn)
			_wait = 8;
			b1 = get_reg(f2);
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;
			if (f2) { //FIXME: ��������� ���
				_regs.pc = t1;
			}
			break;
		case 0xda: // 11 011 010	JP C, (nn)
			_wait = 8;
			b1 = get_reg(f2);
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;
			if (f2) { //FIXME: ��������� ���
				_regs.pc = t1;
			}
			break;

		case 0xe2: // 11 100 010	JP PO, (nn)
			_wait = 8;
			b1 = get_reg(f2);
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;
			if (f2) { //FIXME: ��������� ���
				_regs.pc = t1;
			}
			break;

		case 0xea: // 11 101 010	JP PE, (nn)
			_wait = 8;
			b1 = get_reg(f2);
			t1 = _bus.read16(_regs.pc + 1);
			_regs.pc += 3;
			if (f2) { //FIXME: ��������� ���
				_regs.pc = t1;
			}
			break;
*/
		//!!!
		case 0xf2: { // 11 110 010	JP P, (nn)
			_wait = 8;
			t1 = _bus.read16(_regs.pc + 1);
			if(unsigned((_regs.f & 0x0f) >> 2)){
					_regs.pc = t1;
			}
			_regs.pc += 3;
			//std::cout << std::hex << unsigned(_regs.f) << "_" << unsigned(_regs.f & 0x0f)<< "_" << unsigned((_regs.f & 0x0f) >> 2);
			}
			break;
/*
		case 0xfa: // 11 111 010	JP M, (nn)
*/
		case 0xf3:  //DI
			_wait = 2;
			_regs.pc++; //������� �������� ����
			iff1 = false;
			iff2 = false;
			break;

		default: {
			std::stringstream ss;
			ss << "Unknown instruction " << std::hex << std::setfill('0')
					<< std::setw(2) << unsigned(_instruction) << " @ "
					<< std::setw(4) << unsigned(_regs.pc);
			throw std::runtime_error(ss.str());
		}
		}

		_state = 3; //� ��������� 3 ���������� ��������� ������
	} else { //�������� )����� �������� ��������������� ��������� Z80
		--_wait;
		if (_wait == 0)
			_state = 0;
	}

}

uint8_t Z80::get_reg(uint8_t reg) const {
	switch (reg) {
	case 0:
		return _regs.b;
		break; //���������� b (������� �����)
	case 1:
		return _regs.c;
		break; //���������� c (������� �����)
	case 2:
		return _regs.d;
		break; //���������� d
	case 3:
		return _regs.e;
		break; //���������� e
	case 4:
		return _regs.h;
		break; //���������� h ��� ix ��� iy FIXME: ��������
	case 5:
		return _regs.l;
		break; //���������� l ��� ix ��� iy FIXME: ��������
	case 6:
		return _bus.read(_regs.hl);
		break; //���������� FIXME: ��������
	case 7:
		return _regs.a;
		break; //���������� a
	default:
		throw std::runtime_error("Unknown register reference!");
	}
}

void Z80::reset() {
	_regs.pc = 0; //TODO: ����������� ������ ������ ����������
}

