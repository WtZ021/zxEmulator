/*
 * Z80CPU.h
 */

#ifndef Z80CPU_H_
#define Z80CPU_H_
#include "Memory.h"

//#include <libspectrum.h>

#include "libz80\z80.h"

byte In_memo(void* par, ushort address);
void Out_memo(void* par, ushort address, byte data);
byte In_inpout(void* par, ushort address);
void Out_inpout(void* par, ushort address, byte data);

class Z80CPU
{
protected:
	AddressSpace & _bus;
	Z80Context _context { };
	friend byte In_memo(void* par, ushort address);
	friend void Out_memo(void* par, ushort address, byte data);
	friend byte In_inpout(void* par, ushort address);
	friend void Out_inpout(void* par, ushort address, byte data);

public:
	Z80CPU(AddressSpace & bus): _bus(bus) {
		_context.memRead = In_memo;
		_context.memWrite = Out_memo;
		_context.ioRead = In_inpout;
		_context.ioWrite = Out_inpout;
		_context.ioParam = this;
		_context.memParam = this;
	}

	void tick() {
		Z80Execute (&_context);
	}
	void ticks(unsigned ticks) {
		Z80ExecuteTStates(&_context, ticks);
	}

	void reset() {
		Z80RESET(&_context);;
	}

	void intr(byte value) {
		Z80INT(&_context, value);
	}

	void nmi() {
		Z80NMI(&_context);
	}

	void save_state_sna(const char * filename);
	void load_state_sna(const char * filename);
	void load_state_z80(const char * filename);

	void load_state_sna_libspectrum(const char * filename);
	void load_state_z80_libspectrum(const char * filename);

};

#endif /* Z80CPU_H_ */
