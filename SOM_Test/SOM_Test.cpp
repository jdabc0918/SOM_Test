// SOM_Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


int main()
{
	//	サンプル
	const int sampleNum = 300;
	std::vector<som::Feature> samples(sampleNum, som::Feature());

	////	データ生成(円)
	//double cx = 250;	//	X中心
	//double cy = 250;	//	Y中心
	//double radius = 150;	//	半径
	//for (int i = 0; i < sampleNum; i++)
	//{
	//	som::Feature *f = &samples[i];
	//	double rad = ((double)std::rand() / RAND_MAX) * 2 * M_PI;	//	0~2pi
	//	double rand_x = 50*(double)std::rand() / RAND_MAX;
	//	double rand_y = 50*(double)std::rand() / RAND_MAX;
	//	double X = radius * std::cos(rad) + cx + rand_x;
	//	double Y = radius * std::sin(rad) + cy + rand_y;
	//	f->set(std::vector<double>{X, Y});
	//}

	////	データ生成(三角形)
	//cv::Point2d a(100, 100), b(250, 400), c(400, 100);
	//for (int i = 0; i < sampleNum; i++)
	//{
	//	som::Feature *f = &samples[i];
	//	double rand1 = (double)rand() / RAND_MAX;
	//	double rand2 = (double)rand() / RAND_MAX;
	//	double min = std::min(rand1, rand2);
	//	double max = std::max(rand1, rand2);
	//	cv::Point2d p = a * min + b*(1 - max) + c*(max - min);
	//	f->set(std::vector<double>{p.x,p.y});
	//}

	//	データ生成(半円、直線)
	double cx = 250;	//	X中心
	double cy = 250;	//	Y中心
	double radius = 150;	//	半径
	for (int i = 0; i < sampleNum / 2; i++)
	{
		som::Feature *f = &samples[i];
		double rad = ((double)std::rand() / RAND_MAX) * M_PI;	//	0~2pi
		double rand_x = 50 * (double)std::rand() / RAND_MAX;
		double rand_y = 50 * (double)std::rand() / RAND_MAX;
		double X = radius * std::cos(rad) + cx + rand_x;
		double Y = radius * std::sin(rad) + cy + rand_y;
		f->set(std::vector<double>{X, Y});
	}
	for (int i = sampleNum / 2; i < sampleNum; i++)
	{
		som::Feature *f = &samples[i];
		double rand_x = 50 * (double)std::rand() / RAND_MAX;
		double rand_y = 50 * (double)std::rand() / RAND_MAX;
		double X = ((double)std::rand() / RAND_MAX) * 300 + 100 + rand_x;	//	100~400
		double Y = 1.0 / 3.0 * X + rand_y;
		f->set(std::vector<double>{X, Y});
	}

	//	SOM実行
	som::SelfOrganizingMap SOM(6, 6);
	SOM.Train(samples, 1000000);

    return 0;
}

