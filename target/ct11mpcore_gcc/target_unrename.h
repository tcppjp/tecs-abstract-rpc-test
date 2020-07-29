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
 *  target_timer.c
 */
#undef target_totimer_initialize
#undef target_totimer_terminate
#undef target_twdtimer_start
#undef target_twdtimer_stop
#undef target_twdtimer_get_current
#undef target_ovrtimer_start
#undef target_ovrtimer_stop
#undef target_ovrtimer_get_current
#undef target_totimer_handler


#include "core_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
