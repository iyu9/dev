#pragma once

//
// Common.h
// ���ʂŎg�p����}�N����萔�̒�`
//

#ifndef _Common_h_
#define _Common_h_

#include <windows.h>
#include <stdio.h> //for sprintf

// �������̉��
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

// �Q�ƃJ�E���^�̃f�N�������g
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// �G���[�̕񍐂ƃA�v���P�[�V�����̏I��
#define ERROR_EXIT() { int line = __LINE__; const char *file = __FILE__;\
	char msg[_MAX_FNAME + _MAX_EXT + 256];\
	char drive[_MAX_DRIVE];\
	char dir[_MAX_DIR];\
	char fname[_MAX_FNAME];\
	char ext[_MAX_EXT];\
	_splitpath(file, drive, dir, fname, ext);\
	sprintf(msg, "���炩�̃G���[�������������߃A�v���P�[�V�������I�����܂�\r\n"\
		"�t�@�C�� : %s%s\r\n"\
		"�s�ԍ� : %d", fname, ext, line);\
	MessageBox(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);\
	PostQuitMessage(1);\
}

#endif // _Common_h_