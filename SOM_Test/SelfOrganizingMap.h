#pragma once

namespace som
{
	using namespace som;

	const int FEATURE_DIMS = 2;

	/**
	*	@struct Feature
	*	@brief	各ノードが持つ特徴ベクトル
	**/
	typedef struct Feature
	{
		std::vector<double> data;
		
		Feature() { data.assign(FEATURE_DIMS, 0); }
		void set(const std::vector<double> d)
		{
			assert((int)d.size() == FEATURE_DIMS);
			data = d;
		}

	}Feature;

	/**
	*	@struct Node
	*	@brief	SOMのノード
	**/
	typedef struct Node
	{
		som::Feature f;					//	自信の特徴ベクトル
		std::vector<Node*>	neighbor;	//	隣接ノードへのポインタ

	}Node;

	/**
	*	@class	SOMクラス
	**/
	class SelfOrganizingMap
	{
	private:

		int	m_height;
		int	m_width;
		int m_sz;
		std::vector<Node>	m_Map;	/**<	マップ	*/

	private:

		/**
		*	マップを初期化する
		*	@param	[in]	int	height	マップの高さ
		*	@param	[in]	int	width	マップの幅
		**/
		void InitMap(int height, int width);

		/**
		*	入力に最も近いノードインデックスを返す
		*	@param	[in]	som::Feature	feature	入力特徴
		*	@param	[out]	double			dist	距離
		*	@return	インデックス
		**/
		int	GetNearestNodeIndex(Feature feature, double &dist);

		/**
		*	暴露ステップ
		*	@param	[in]	som::Feature	feature	暴露する入力データ
		**/
		void Exposure(Feature feature);

	public:
		SelfOrganizingMap();
		SelfOrganizingMap(int height, int width);
		~SelfOrganizingMap();

		void Train(std::vector<Feature> datas, int Loop = 100);
	};
}

