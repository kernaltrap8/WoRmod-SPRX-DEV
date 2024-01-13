

#include <cellstatus.h>
#include <sys/prx.h>


#include "main.h"
#include "printf.h"
#include "syscalls.h"

#include "detour\Detour.h"
#include "scripting/script.h"

SYS_MODULE_INFO( wor_tests, 0, 1, 1);
SYS_MODULE_START( _wor_tests_prx_entry );
SYS_MODULE_STOP(_wor_tests_prx_stop);

SYS_LIB_DECLARE_WITH_STUB( LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME );
SYS_LIB_EXPORT( _wor_tests_export_function, LIBNAME );

sys_ppu_thread_t gWORTthreadID = SYS_PPU_THREAD_ID_INVALID;

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _wor_tests_export_function(void)
{
	
    return CELL_OK;
}

void wor_test_main_thread(uint64_t args) {
	for (;;) {
		printf("i am injected\n");
		_sys_timer_sleep(1);
	}
	
	_sys_ppu_thread_exit(0);
}


extern "C" uint64_t strlen(const char *s) {
	uint64_t r = 0;
	while (*s) {
		r++;
		s++;
	}
	return r;
}

extern "C" int _wor_tests_prx_entry(void)
{
	// enable_button_cheats | for debug menu
	Script::CSymbolTableEntry* enable_button_cheats_symbol = Script::Resolve(720971780);
	printf("enable_button_cheats symbol: %p\n", enable_button_cheats_symbol);

	if (enable_button_cheats_symbol) {
		printf("enable_button_cheats symbol data: %p %d %d\n", enable_button_cheats_symbol->union_type, enable_button_cheats_symbol->type, enable_button_cheats_symbol->sourceFileNameChecksum);
		enable_button_cheats_symbol->union_type = 1;
	}
	// debug_use_screen_noise | post processing FX disable
	Script::CSymbolTableEntry* debug_use_screen_noise_symbol = Script::Resolve(3786639802);
	printf("debug_use_screen_noise symbol: %p\n", debug_use_screen_noise_symbol);
	
	if (debug_use_screen_noise_symbol) {
		printf("debug_use_screen_noise symbol data: %p %d %d\n", debug_use_screen_noise_symbol->union_type, debug_use_screen_noise_symbol->type, debug_use_screen_noise_symbol->sourceFileNameChecksum);
		debug_use_screen_noise_symbol->union_type = 0;
	}
	// debug_use_motion_blur | post processing FX disable
	Script::CSymbolTableEntry* debug_use_motion_blur_symbol = Script::Resolve(42529484);
	printf("debug_use_motion_blur symbol: %p\n", debug_use_motion_blur_symbol);

	if (debug_use_motion_blur_symbol) {
		printf("debug_use_motion_blur symbol data: %p %d %d\n", debug_use_motion_blur_symbol->union_type, debug_use_motion_blur_symbol->type, debug_use_motion_blur_symbol->sourceFileNameChecksum);
		debug_use_motion_blur_symbol->union_type = 0;
	}
	// apply changes to qsymbols
	CFuncs::RegisterCFuncs();

	// Exit thread using directly the syscall and not the user mode library or else we will crash
	_sys_ppu_thread_exit(0);
    return SYS_PRX_RESIDENT;
}


extern "C" int _wor_tests_prx_stop(void)
{
	//uint64_t retVal;
	//sys_ppu_thread_join(gWORTthreadID, &retVal);

	// Exit thread using directly the syscall and not the user mode library or else we will crash
	_sys_ppu_thread_exit(0);

	return SYS_PRX_RESIDENT;
}
