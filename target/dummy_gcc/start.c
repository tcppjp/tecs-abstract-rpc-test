/*
 *		ダミーターゲットのスタートアップモジュール
 *
 *  $Id: start.c 305 2018-03-27 13:16:31Z ertl-hiro $
 */

extern void	_kernel_initialize_sections(void);
extern void sta_ker(void);

int
main(void)
{
	_kernel_initialize_sections();
	sta_ker();
	return(0);
}
