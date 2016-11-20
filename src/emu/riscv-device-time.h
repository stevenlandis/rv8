//
//  riscv-device-time.h
//

#ifndef riscv_device_time_h
#define riscv_device_time_h

namespace riscv {

	/* TIME MMIO device */

	template <typename P>
	struct time_mmio_device : memory_segment<typename P::ux>
	{
		typedef typename P::ux UX;

		enum {
			total_size = sizeof(u64) * 2
		};

		P &proc;

		/* Time registers */

		u64 mtime;
		u64 mtimecmp;

		/* TODO implement TIME MMIO access and timer interrupts */

		time_mmio_device(P &proc, UX mpa) :
			memory_segment<UX>(mpa, /*uva*/0, /*size*/total_size,
				pma_type_io | pma_prot_read | pma_prot_write), proc(proc) {}

		void load_8 (UX va, u8  &val) { printf("time_mmio:0x%04llx\n", addr_t(va)); val = 0; }
		void load_16(UX va, u16 &val) { printf("time_mmio:0x%04llx\n", addr_t(va)); val = 0; }

		void load_32(UX va, u32 &val)
		{
			printf("time_mmio:0x%04llx\n", addr_t(va));
			if (va == 0) val = u32(mtime);
			else if (va == 4) val = u32(mtime >> 32);
			else if (va == 8) val = u32(mtimecmp);
			else if (va == 12) val = u32(mtimecmp >> 32);
		}

		void load_64(UX va, u64 &val)
		{
			printf("time_mmio:0x%04llx\n", addr_t(va));
			if (va == 0) val = mtime;
			else if (va == 8) val = mtimecmp;
		}

		void store_8 (UX va, u8  val) { printf("time_mmio:0x%04llx\n", addr_t(va)); }
		void store_16(UX va, u16 val) { printf("time_mmio:0x%04llx\n", addr_t(va)); }

		void store_32(UX va, u32 val)
		{
			printf("time_mmio:0x%04llx\n", addr_t(va));
			if (va == 0) mtime = (mtime & 0xffffffff00000000ULL) | val;
			else if (va == 4) mtime = (mtime & 0xffffffffULL) | (u64(val) << 32);
			else if (va == 8) mtimecmp = (mtimecmp & 0xffffffff00000000ULL) | val;
			else if (va == 12) mtimecmp = (mtimecmp & 0xffffffffULL) | (u64(val) << 32);
		}

		void store_64(UX va, u64 val)
		{
			printf("time_mmio:0x%04llx\n", addr_t(va));
			if (va == 0) mtime = val;
			else if (va == 8) mtimecmp = val;
		}
	};

}

#endif
