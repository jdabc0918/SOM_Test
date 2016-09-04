#include "stdafx.h"
#include "SelfOrganizingMap.h"

namespace som
{
	using namespace som;

	/**
	*	マップを初期化する
	*	@param	[in]	int	height	マップの高さ
	*	@param	[in]	int	width	マップの幅
	**/
	void SelfOrganizingMap::InitMap(int height, int width)
	{
		assert(height > 1 && width > 1);

		//	クリア
		m_Map.clear();

		//	作成
		m_Map.assign(height * width, Node());
		for(int y=0;y<height;y++)
		{
			for (int x = 0; x < width; x++)
			{
				int i = y * width + x;	//	インデックス
				Node *n = &m_Map[i];	//	注目ノード

				//	特徴量初期化
				n->f.data.clear();
				n->f.data.assign(FEATURE_DIMS, 250);

				//	注目ノードの近傍ノードを作成
				n->neighbor.clear();	//	クリア
				if (y == 0)	//	最上行
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
				else if (y == height - 1)	//	最下行
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
				else if (x == 0)	//	最左列
				{
					n->neighbor.push_back(&m_Map[i - width]);
					n->neighbor.push_back(&m_Map[i + 1]);
					n->neighbor.push_back(&m_Map[i + width]);
				}
				else if (x == width - 1)	//	最右列
				{
					n->neighbor.push_back(&m_Map[i - width]);
					n->neighbor.push_back(&m_Map[i + width]);
					n->neighbor.push_back(&m_Map[i - 1]);
				}
				else //	内部
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
	*	入力に最も近いノードインデックスを返す
	*	@param	[in]	som::Feature	feature	入力特徴
	*	@param	[out]	double			dist	距離
	*	@return	インデックス
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
	*	暴露ステップ
	*	@param	[in]	som::Feature	feature	暴露する入力データ
	**/
	void SelfOrganizingMap::Exposure(Feature feature)
	{
		double ratio = 0.1;	//	近づける割合。中心=ratio, 隣=ratio^2

		double Dist;
		int NearestIndex = GetNearestNodeIndex(feature, Dist);

		Node *nearestNode = &m_Map[NearestIndex];
		for (int i = 0; i < FEATURE_DIMS; i++)	//	中心Node
		{
			nearestNode->f.data[i] += (feature.data[i] - nearestNode->f.data[i]) * ratio;
		}
		for (int i = 0; i < (int)nearestNode->neighbor.size(); i++)	//	隣接Node
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

			//	デバッグ用！座標でしか使えない
			cv::Mat debugImage(500, 500, CV_8UC3, cv::Scalar::all(255));
			for (int i = 0; i < (int)datas.size(); i++)
			{
				cv::Point center((int)datas[i].data[0], (int)datas[i].data[1]);
				cv::circle(debugImage, center, 2, cv::Scalar(180, 180, 0), -1);
			}
			for (int i = 0; i < (int)m_Map.size(); i++)	//	まず線を描く
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
			for (int i = 0; i < (int)m_Map.size(); i++)	//	円を描く
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
