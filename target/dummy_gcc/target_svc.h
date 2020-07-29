/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2018 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: target_svc.h 536 2018-11-16 07:03:31Z ertl-hiro $
 */

/*
 *		ソフトウェア割込みによるサービスコール呼出し（ダミーターゲット用）
 */

#ifndef TOPPERS_TARGET_SVC_H
#define TOPPERS_TARGET_SVC_H

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ソフトウェア割込みを用いたサービスコールの呼び出し方法
 */
#define call_svc	_kernel_call_svc

extern ER	call_svc(FN fncd, intptr_t par1, intptr_t par2,
							intptr_t par3, intptr_t par4, intptr_t par5);

#define CAL_SVC_0M(TYPE, FNCD) do {										\
	return((TYPE) call_svc(FNCD, 0, 0, 0, 0, 0));						\
} while (false)

#define CAL_SVC_1M(TYPE, FNCD, TYPE1, PAR1) do {						\
	return((TYPE) call_svc(FNCD, (intptr_t)(PAR1), 0, 0, 0, 0));		\
} while (false)

#define CAL_SVC_2M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2) do {			\
	return((TYPE) call_svc(FNCD, (intptr_t)(PAR1), (intptr_t)(PAR2), 	\
															0, 0, 0));	\
} while (false)

#define CAL_SVC_3M(TYPE, FNCD, TYPE1, PAR1,								\
									TYPE2, PAR2, TYPE3, PAR3) do {		\
	return((TYPE) call_svc(FNCD, (intptr_t)(PAR1), (intptr_t)(PAR2), 	\
											(intptr_t)(PAR3), 0, 0));	\
} while (false)

#define CAL_SVC_4M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2,				\
									TYPE3, PAR3, TYPE4, PAR4) do {		\
	return((TYPE) call_svc(FNCD, (intptr_t)(PAR1), (intptr_t)(PAR2), 	\
								(intptr_t)(PAR3), (intptr_t)(PAR4), 0));\
} while (false)

#define CAL_SVC_5M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2,				\
						TYPE3, PAR3, TYPE4, PAR4, TYPE5, PAR5) do {		\
	return((TYPE) call_svc(FNCD, (intptr_t)(PAR1), (intptr_t)(PAR2), 	\
				(intptr_t)(PAR3), (intptr_t)(PAR4), (intptr_t)(PAR5)));	\
} while (false)

#define CAL_SVC_1M_SYSTIM(TYPE, FNCD, TYPE1, PAR1) do {					\
	return((TYPE) call_svc(FNCD, (SYSTIM)(PAR1), 0, 0, 0, 0));			\
} while (false)

/*
 *  サービスコール呼出し関数の展開
 */
#ifndef TOPPERS_SVC_CALL
#include "arch/gcc/tool_svc.h"
#endif /* TOPPERS_SVC_CALL */

/*
 *  cal_svcサービスコールの呼出し
 */
Inline ER_UINT
cal_svc(FN fncd, intptr_t par1, intptr_t par2,
							intptr_t par3, intptr_t par4, intptr_t par5)
{
	if (fncd > 0) {
		CAL_SVC_5M(ER_UINT, fncd, intptr_t, par1, intptr_t, par2,
							intptr_t, par3, intptr_t, par4, intptr_t, par5);
	}
	else {
		return(E_RSFN);
	}
}

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_SVC_H */
