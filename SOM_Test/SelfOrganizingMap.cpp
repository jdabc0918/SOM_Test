#include "stdafx.h"
#include "SelfOrganizingMap.h"

namespace som
{
	using namespace som;

	/**
	*	�}�b�v������������
	*	@param	[in]	int	height	�}�b�v�̍���
	*	@param	[in]	int	width	�}�b�v�̕�
	**/
	void SelfOrganizingMap::InitMap(int height, int width)
	{
		assert(height > 1 && width > 1);

		//	�N���A
		m_Map.clear();

		//	�쐬
		m_Map.assign(height * width, Node());
		for(int y=0;y<height;y++)
		{
			for (int x = 0; x < width; x++)
			{
				int i = y * width + x;	//	�C���f�b�N�X
				Node *n = &m_Map[i];	//	���ڃm�[�h

				//	�����ʏ�����
				n->f.data.clear();
				n->f.data.assign(FEATURE_DIMS, 250);

				//	���ڃm�[�h�̋ߖT�m�[�h���쐬
				n->neighbor.clear();	//	�N���A
				if (y == 0)	//	�ŏ�s
				{
					if (x == 0)
					{
						n->neighbor.push_back(&m_Map[1]);
						n->neighbor.push_back(&m_Map[width]);
					}
					else if (x == width - 1)
					{
						n->neighbor.push_back(&m_Map[2 * width - 1]);
						n->neighbor.push_back(&m_Map[width - 2]);
					}
					else
					{
						n->neighbor.push_back(&m_Map[i + 1]);
						n->neighbor.push_back(&m_Map[i + width]);
						n->neighbor.push_back(&m_Map[i - 1]);
					}
				}
				else if (y == height - 1)	//	�ŉ��s
				{
					if (x == 0)
					{
						n->neighbor.push_back(&m_Map[i - width]);
						n->neighbor.push_back(&m_Map[i + 1]);
					}
					else if (x == width - 1)
					{
						n->neighbor.push_back(&m_Map[i - width]);
						n->neighbor.push_back(&m_Map[i - 1]);
					}
					else
					{
						n->neighbor.push_back(&m_Map[i - width]);
						n->neighbor.push_back(&m_Map[i + 1]);
						n->neighbor.push_back(&m_Map[i - 1]);
					}
				}
				else if (x == 0)	//	�ō���
				{
					n->neighbor.push_back(&m_Map[i - width]);
					n->neighbor.push_back(&m_Map[i + 1]);
					n->neighbor.push_back(&m_Map[i + width]);
				}
				else if (x == width - 1)	//	�ŉE��
				{
					n->neighbor.push_back(&m_Map[i - width]);
					n->neighbor.push_back(&m_Map[i + width]);
					n->neighbor.push_back(&m_Map[i - 1]);
				}
				else //	����
				{
					n->neighbor.push_back(&m_Map[i - width]);
					n->neighbor.push_back(&m_Map[i + 1]);
					n->neighbor.push_back(&m_Map[i + width]);
					n->neighbor.push_back(&m_Map[i - 1]);
				}
			}
		}
	}

	/**
	*	���͂ɍł��߂��m�[�h�C���f�b�N�X��Ԃ�
	*	@param	[in]	som::Feature	feature	���͓���
	*	@param	[out]	double			dist	����
	*	@return	�C���f�b�N�X
	**/
	int	SelfOrganizingMap::GetNearestNodeIndex(Feature feature, double &dist)
	{
		Feature *f1 = &feature;

		double min_dist = DBL_MAX;
		int min_dist_index = 0;

		for (int i = 0; i < m_sz; i++)
		{
			Feature *f2 = &m_Map[i].f;

			double distance = 0;
			for (int j = 0; j < FEATURE_DIMS; j++)
			{
				double d = f1->data[j] - f2->data[j];
				distance += d*d;
			}
			distance = std::sqrt(distance);

			if (min_dist > distance)
			{
				min_dist = distance;
				min_dist_index = i;
			}
		}

		dist = min_dist;
		return min_dist_index;
	}

	/**
	*	�\�I�X�e�b�v
	*	@param	[in]	som::Feature	feature	�\�I������̓f�[�^
	**/
	void SelfOrganizingMap::Exposure(Feature feature)
	{
		double ratio = 0.1;	//	�߂Â��銄���B���S=ratio, ��=ratio^2

		double Dist;
		int NearestIndex = GetNearestNodeIndex(feature, Dist);

		Node *nearestNode = &m_Map[NearestIndex];
		for (int i = 0; i < FEATURE_DIMS; i++)	//	���SNode
		{
			nearestNode->f.data[i] += (feature.data[i] - nearestNode->f.data[i]) * ratio;
		}
		for (int i = 0; i < (int)nearestNode->neighbor.size(); i++)	//	�א�Node
		{
			Node *neighborNode = nearestNode->neighbor[i];
			for (int j = 0; j < FEATURE_DIMS; j++)
			{
				neighborNode->f.data[j] += (feature.data[j] - neighborNode->f.data[j]) * std::pow(ratio, 2);
			}
		}	
	}

	SelfOrganizingMap::SelfOrganizingMap()
	{
	}

	SelfOrganizingMap::SelfOrganizingMap(int height, int width)
	{
		m_height = height;
		m_width = width;
		m_sz = m_height * m_width;
		
		InitMap(m_height, m_width);
	}

	SelfOrganizingMap::~SelfOrganizingMap()
	{
	}

	void SelfOrganizingMap::Train(std::vector<Feature> datas, int Loop)
	{
		for (int loopNum = 0; loopNum < Loop; loopNum++)
		{
			for (int j = 0; j < (int)datas.size(); j++)
			{
				Exposure(datas[j]);
			}

			//	�f�o�b�O�p�I���W�ł����g���Ȃ�
			cv::Mat debugImage(500, 500, CV_8UC3, cv::Scalar::all(255));
			for (int i = 0; i < (int)datas.size(); i++)
			{
				cv::Point center((int)datas[i].data[0], (int)datas[i].data[1]);
				cv::circle(debugImage, center, 2, cv::Scalar(180, 180, 0), -1);
			}
			for (int i = 0; i < (int)m_Map.size(); i++)	//	�܂�����`��
			{
				Node *n = &m_Map[i];
				cv::Point from((int)n->f.data[0], (int)n->f.data[1]);
				for (int j = 0; j < (int)n->neighbor.size(); j++)
				{
					Node *neighbor = n->neighbor[j];
					cv::Point to((int)neighbor->f.data[0], (int)neighbor->f.data[1]);
					cv::line(debugImage, from, to, cv::Scalar(0, 0, 0));
				}
			}
			for (int i = 0; i < (int)m_Map.size(); i++)	//	�~��`��
			{
				Node *n = &m_Map[i];
				cv::Point center((int)n->f.data[0], (int)n->f.data[1]);
				cv::circle(debugImage, center, 4, cv::Scalar(0, 180, 180), -1);
			}
			cv::imshow("Debug Image", debugImage);
			cv::waitKey();
		}
	}
}
