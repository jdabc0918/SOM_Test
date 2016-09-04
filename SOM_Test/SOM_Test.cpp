// SOM_Test.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"


int main()
{
	//	�T���v��
	const int sampleNum = 300;
	std::vector<som::Feature> samples(sampleNum, som::Feature());

	////	�f�[�^����(�~)
	//double cx = 250;	//	X���S
	//double cy = 250;	//	Y���S
	//double radius = 150;	//	���a
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

	////	�f�[�^����(�O�p�`)
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

	//	�f�[�^����(���~�A����)
	double cx = 250;	//	X���S
	double cy = 250;	//	Y���S
	double radius = 150;	//	���a
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

	//	SOM���s
	som::SelfOrganizingMap SOM(6, 6);
	SOM.Train(samples, 1000000);

    return 0;
}

