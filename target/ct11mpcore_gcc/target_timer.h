/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2007-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: target_timer.h 545 2018-11-19 16:19:47Z ertl-hiro $
 */

/*
 *		タイマドライバ（CT11MPCore用）
 */

#ifndef TOPPERS_TARGET_TIMER_H
#define TOPPERS_TARGET_TIMER_H

/*
 *  ターゲットのハードウェア資源の定義
 */
#include "ct11mpcore.h"

/*
 *  高分解能タイマドライバ
 *
 *  MPCoreのプライベートタイマとウォッチドッグを用いて高分解能タイマを
 *  実現する．
 */
#define USE_MPCORE_TMRWDG_HRT
#include "mpcore_timer.h"

/*
 *  タイムウィンドウ／オーバランタイマドライバ
 */
#include "sp804.h"
#include <sil.h>

/*
 *  タイムウィンドウ／オーバランタイマドライバで使用するタイマに関する指定
 */
#define TWDTIMER_TIMER_BASE		EB_TIMER0_BASE
#define OVRTIMER_TIMER_BASE		EB_TIMER1_BASE
#define TIMER_TOTIMER_IRQNO		EB_IRQNO_TIMER01

#define SP804_CONFIG		(SP804_INT_ENABLE|SP804_MODE_ONESHOT \
									|SP804_SIZE_32|SP804_PRESCALE_1)

/*
 *  タイムウィンドウ／オーバランタイマ割込みハンドラ登録のための定数
 *
 *  タイムウィンドウタイマ割込みの優先度は，高分解能タイマ割込みと同じ
 *  にしなければならない．
 */
#define INHNO_TOTIMER		TIMER_TOTIMER_IRQNO		/* 割込みハンドラ番号 */
#define INTNO_TOTIMER		TIMER_TOTIMER_IRQNO		/* 割込み番号 */
#define INTPRI_TOTIMER		INTPRI_TIMER			/* 割込み優先度 */
#define INTATR_TOTIMER		TA_NULL					/* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  タイムウィンドウ／オーバランタイマの初期化処理
 */
extern void target_totimer_initialize(intptr_t exinf);

/*
 *  タイムウィンドウ／オーバランタイマの停止処理
 */
extern void target_totimer_terminate(intptr_t exinf);

/*
 *  タイムウィンドウタイマの動作開始
 *
 *  QEMUでは，ロードレジスタに0を設定すると警告メッセージが出るため，1
 *  を設定するようにしている．
 */
Inline void
target_twdtimer_start(PRCTIM twdtim)
{
#ifdef TOPPERS_USE_QEMU
	if (twdtim == 0U) {
		twdtim = 1U;
	}
#endif /* TOPPERS_USE_QEMU */
	sil_wrw_mem(SP804_LR(TWDTIMER_TIMER_BASE), (uint32_t) twdtim);
	sil_wrw_mem(SP804_CR(TWDTIMER_TIMER_BASE), SP804_ENABLE|SP804_CONFIG);
}

/*
 *  タイムウィンドウタイマの停止
 *
 *  ここで割込み要求をクリアすると，タイムウィンドウ／オーバランタイマ
 *  割込みハンドラでどちらの割込みが発生したか判別できないため，クリア
 *  しない．
 */
Inline PRCTIM
target_twdtimer_stop(void)
{
	sil_wrw_mem(SP804_CR(TWDTIMER_TIMER_BASE), SP804_DISABLE|SP804_CONFIG);
/*	sil_wrw_mem(SP804_ICR(TWDTIMER_TIMER_BASE), 0U); */
	return((PRCTIM) sil_rew_mem(SP804_CVR(TWDTIMER_TIMER_BASE)));
}

/*
 *  タイムウィンドウタイマの現在値の読出し
 */
Inline PRCTIM
target_twdtimer_get_current(void)
{
	return((PRCTIM) sil_rew_mem(SP804_CVR(TWDTIMER_TIMER_BASE)));
}

#ifdef TOPPERS_SUPPORT_OVRHDR

/*
 *  オーバランタイマの動作開始
 *
 *  QEMUでは，ロードレジスタに0を設定すると警告メッセージが出るため，1
 *  を設定するようにしている．
 */
Inline void
target_ovrtimer_start(PRCTIM ovrtim)
{
#ifdef TOPPERS_USE_QEMU
	if (ovrtim == 0U) {
		ovrtim = 1U;
	}
#endif /* TOPPERS_USE_QEMU */
	sil_wrw_mem(SP804_LR(OVRTIMER_TIMER_BASE), (uint32_t) ovrtim);
	sil_wrw_mem(SP804_CR(OVRTIMER_TIMER_BASE), SP804_ENABLE|SP804_CONFIG);
}

/*
 *  オーバランタイマの停止
 *
 *  ここで割込み要求をクリアすると，タイムウィンドウ／オーバランタイマ
 *  割込みハンドラでどちらの割込みが発生したか判別できないため，クリア
 *  しない．
 */
Inline PRCTIM
target_ovrtimer_stop(void)
{
	sil_wrw_mem(SP804_CR(OVRTIMER_TIMER_BASE), SP804_DISABLE|SP804_CONFIG);
/*	sil_wrw_mem(SP804_ICR(OVRTIMER_TIMER_BASE), 0U); */
	return((PRCTIM) sil_rew_mem(SP804_CVR(OVRTIMER_TIMER_BASE)));
}

/*
 *  オーバランタイマの現在値の読出し
 */
Inline PRCTIM
target_ovrtimer_get_current(void)
{
	return((PRCTIM) sil_rew_mem(SP804_CVR(OVRTIMER_TIMER_BASE)));
}

#endif /* TOPPERS_SUPPORT_OVRHDR */

/*
 *  タイムウィンドウ／オーバランタイマ割込みハンドラ
 */
extern void	target_totimer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_TIMER_H */
