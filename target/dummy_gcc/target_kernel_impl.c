/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2013-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: target_kernel_impl.c 606 2018-12-13 16:13:01Z ertl-hiro $
 */

/*
 *		ターゲット依存モジュール（ダミーターゲット用）
 */

#include "kernel_impl.h"
#include "task.h"
#include "svc_manage.h"
#ifdef TOPPERS_SUPPORT_OVRHDR
#include "overrun.h"
#endif /* TOPPERS_SUPPORT_OVRHDR */

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_DSP_ENTER
#define LOG_DSP_ENTER(p_tcb)
#endif /* LOG_DSP_ENTER */

#ifndef LOG_DSP_LEAVE
#define LOG_DSP_LEAVE(p_tcb)
#endif /* LOG_DSP_LEAVE */

/*
 *  ディスパッチャ本体
 */
static void
dispatcher(void)
{
	LOG_DSP_ENTER(p_runtsk);

/* dispatcher_0: */
	p_runtsk = p_schedtsk;
	if (p_runtsk != NULL) {
		/* 自タスク（p_runtsk）のTCBからスタックポインタを復帰する */
		LOG_DSP_LEAVE(p_runtsk);
		/* 自タスクのTCBから実行再開番地を復帰し，そこへ分岐する */
	}

	/*
	 *  アイドル処理
	 */
	/* 割込みを許可したらCPUロック解除状態になるよう準備する */
	/* 割込みをすべて許可する */
	while (true) {
#ifdef TOPPERS_CUSTOM_IDLE
		TOPPERS_CUSTOM_IDLE();
#else /* TOPPERS_CUSTOM_IDLE */
		/* 割込み発生を待つ */
#endif /* TOPPERS_CUSTOM_IDLE */
	}
}

/*
 *  最高優先順位タスクへのディスパッチ
 */
void
dispatch(void)
{
#ifdef TOPPERS_SUPPORT_OVRHDR
	ovrtimer_stop();					/* オーバランタイマの停止 */
#endif /* TOPPERS_SUPPORT_OVRHDR */
	/* スクラッチレジスタを除くすべてのレジスタをスタックに保存する */
	/* スタックポインタを自タスク（p_runtsk）のTCBに保存する */
	/* dispatch_rを，実行再開番地として自タスクのTCBに保存する */
	dispatcher();	/* コンパイラの警告抑止のため，ここからは呼ぶ */

/* dispatch_r: */
	/* スクラッチレジスタを除くすべてのレジスタをスタックから復帰する */
#ifdef TOPPERS_SUPPORT_OVRHDR
	ovrtimer_start();					/* オーバランタイマの動作開始 */
#endif /* TOPPERS_SUPPORT_OVRHDR */
}

/*
 *  ディスパッチャの動作開始
 */
void
start_dispatch(void)
{
	/* タスクコンテキストに切り換える */
	/* スタックをIDが1のタスクのスタック領域に切り換える */
	/* CPUロック状態・割込み優先度マスク全解除状態にする */
	/* dispatcher_0に分岐する */
}

/*
 *  現在のコンテキストを捨ててディスパッチ
 */
void
exit_and_dispatch(void)
{
	/* dispatcherに分岐する */
}

/*
 *  割込みハンドラ出入口処理
 */
void
int_handler_entry(void)
{
}

/*
 *  CPU例外ハンドラ出入口処理
 */
void
exc_handler_entry(void)
{
}

/*
 *  ソフトウェア割込みによるサービスコールの出入口処理
 */
typedef ER_UINT	(*SVC)();				/* svc_table.cで定義 */
extern const SVC svc_table[];			/* svc_table.cで定義 */
extern const FN tmax_fncd;				/* kernel_cfg.cで定義 */
extern const SVCINIB svcinib_table[];	/* kernel_cfg.cで定義 */


ER_UINT
svc_entry(intptr_t par1, intptr_t par2, intptr_t par3,
								intptr_t par4, intptr_t par5, FN fncd)
{
	ER_UINT		ercd;

	if (fncd <= 0) {
		/*
		 *  カーネルのサービスコールの場合
		 */
		if (fncd < TMIN_FNCD) {
			ercd = E_RSFN;
			goto error_exit;
		}
		ercd = (*(svc_table[-fncd]))(par1, par2, par3, par4, par5);
	}
	else {
		/*
		 *  拡張サービスコールの場合
		 */
		const SVCINIB	*p_svcinib;

		if (fncd > tmax_fncd) {
			ercd = E_RSFN;
			goto error_exit;
		}
		p_svcinib = &svcinib_table[fncd - 1];
		if (p_svcinib->extsvc == NULL) {
			ercd = E_RSFN;
			goto error_exit;
		}
		ercd = (*(p_svcinib->extsvc))(par1, par2, par3, par4, par5, 0);
	}

  error_exit:
	return(ercd);
}

/*
 *  カーネルの終了処理の呼出し
 */
void
call_exit_kernel(void)
{
	/* 非タスクコンテキストに切り換える */
	/* exit_kernelに分岐する */
	while (true) ;
}

/*
 *  タスク開始時処理
 */
void
start_r(void)
{
#ifdef TOPPERS_SUPPORT_OVRHDR
	ovrtimer_start();					/* オーバランタイマの動作開始 */
#endif /* TOPPERS_SUPPORT_OVRHDR */
	/* CPUロック解除状態にする */
	/* 自タスク（p_runtsk）の起動番地を，拡張情報をパラメータとして呼び出す */
	/* ext_tskに分岐する */
}

/*
 *  システムログの低レベル出力のための初期化
 *
 *  セルタイプtPutLogSIOPort内に実装されている関数を直接呼び出す．
 */
extern void	tPutLogSIOPort_initialize(void);

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 *  SIOを初期化
	 */
#ifndef TOPPERS_OMIT_TECS
	tPutLogSIOPort_initialize();
#endif /* TOPPERS_OMIT_TECS */
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	while (true) ;
}

/*
 *  ソフトウェア割込みを用いたサービスコールの呼び出し
 */
ER
call_svc(FN fncd, intptr_t par1, intptr_t par2, intptr_t par3,
										intptr_t par4, intptr_t par5)
{
	return(0);
}
