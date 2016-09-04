// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
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


// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#include "SelfOrganizingMap.h"