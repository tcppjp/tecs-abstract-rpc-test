/* This file is generated from target_rename.def by genrename. */

#ifndef TOPPERS_TARGET_RENAME_H
#define TOPPERS_TARGET_RENAME_H

/*
 *  target_kernel_impl.c
 */
#define target_initialize			_kernel_target_initialize
#define target_exit					_kernel_target_exit

/*
 *  sim_timer.c
 */
#define target_timer_initialize		_kernel_target_timer_initialize
#define target_timer_terminate		_kernel_target_timer_terminate
#define target_hrt_get_current		_kernel_target_hrt_get_current
#define target_hrt_set_event		_kernel_target_hrt_set_event
#define target_hrt_raise_event		_kernel_target_hrt_raise_event
#define target_hrt_handler			_kernel_target_hrt_handler
#define target_twdtimer_start		_kernel_target_twdtimer_start
#define target_twdtimer_stop		_kernel_target_twdtimer_stop
#define target_twdtimer_get_current	_kernel_target_twdtimer_get_current
#define target_twdtimer_handler		_kernel_target_twdtimer_handler
#define target_ovrtimer_start		_kernel_target_ovrtimer_start
#define target_ovrtimer_stop		_kernel_target_ovrtimer_stop
#define target_ovrtimer_get_current	_kernel_target_ovrtimer_get_current
#define target_ovrtimer_handler		_kernel_target_ovrtimer_handler
#define target_custom_idle			_kernel_target_custom_idle
#define simtim_advance				_kernel_simtim_advance
#define simtim_add					_kernel_simtim_add
#define hook_hrt_set_event			_kernel_hook_hrt_set_event
#define hook_hrt_raise_event		_kernel_hook_hrt_raise_event


#include "core_rename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
