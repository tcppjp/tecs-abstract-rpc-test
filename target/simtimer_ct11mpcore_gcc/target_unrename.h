/* This file is generated from target_rename.def by genrename. */

/* This file is included only when target_rename.h has been included. */
#ifdef TOPPERS_TARGET_RENAME_H
#undef TOPPERS_TARGET_RENAME_H

/*
 *  target_kernel_impl.c
 */
#undef target_initialize
#undef target_exit

/*
 *  sim_timer.c
 */
#undef target_timer_initialize
#undef target_timer_terminate
#undef target_hrt_get_current
#undef target_hrt_set_event
#undef target_hrt_raise_event
#undef target_hrt_handler
#undef target_twdtimer_start
#undef target_twdtimer_stop
#undef target_twdtimer_get_current
#undef target_twdtimer_handler
#undef target_ovrtimer_start
#undef target_ovrtimer_stop
#undef target_ovrtimer_get_current
#undef target_ovrtimer_handler
#undef target_custom_idle
#undef simtim_advance
#undef simtim_add
#undef hook_hrt_set_event
#undef hook_hrt_raise_event


#include "core_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
