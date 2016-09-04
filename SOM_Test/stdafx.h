// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

//	Standard
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>

//	OpenCV
#include <opencv2/opencv.hpp>
#ifdef _DEBUG
#pragma comment(lib,"opencv_world310d.lib")		// opencv_world
#else
#pragma comment(lib,"opencv_world310.lib")			// opencv_world
#endif

//	Boost
#include <boost\filesystem.hpp>
#include <boost\format.hpp>
namespace fs = boost::filesystem;


// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă�������
#include "SelfOrganizingMap.h"