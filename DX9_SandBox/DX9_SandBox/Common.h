#pragma once

//
// Common.h
// 共通で使用するマクロや定数の定義
//

#ifndef _Common_h_
#define _Common_h_

#include <windows.h>
#include <stdio.h> //for sprintf

// メモリの解放
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

// 参照カウンタのデクリメント
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// エラーの報告とアプリケーションの終了
#define ERROR_EXIT() { int line = __LINE__; const char *file = __FILE__;\
	char msg[_MAX_FNAME + _MAX_EXT + 256];\
	char drive[_MAX_DRIVE];\
	char dir[_MAX_DIR];\
	char fname[_MAX_FNAME];\
	char ext[_MAX_EXT];\
	_splitpath(file, drive, dir, fname, ext);\
	sprintf(msg, "何らかのエラーが発生したためアプリケーションを終了します\r\n"\
		"ファイル : %s%s\r\n"\
		"行番号 : %d", fname, ext, line);\
	MessageBox(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);\
	PostQuitMessage(1);\
}

#endif // _Common_h_