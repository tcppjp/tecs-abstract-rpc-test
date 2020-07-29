/*
 * This file was automatically generated by tecsgen.
 * Move and rename like below before editing,
 *   gen/tApp_templ.c => src/tApp.c
 * to avoid to be overwritten by tecsgen.
 */
/* #[<PREAMBLE>]#
 * Don't edit the comments between #[<...>]# and #[</...>]#
 * These comment are used by tecsmerege when merging.
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tApp_tecsgen.h"

#ifndef E_OK
#define E_OK 0     /* success */
#define E_ID (-18) /* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eMain
 * entry port: eMain
 * signature:  sTaskBody
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eMain_main
 * name:         eMain_main
 * global_name:  tApp_eMain_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void eMain_main() {
    syslog(LOG_NOTICE, "Commencing greeting.");
    ER e = cGreet_greetInKlingon();
    if (e < 0) {
        syslog(LOG_NOTICE, "Greeting failed: %d", e);
    }

    syslog(LOG_NOTICE, "-- end of application --");
    exitKernel();
    while (1) {
    }
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
