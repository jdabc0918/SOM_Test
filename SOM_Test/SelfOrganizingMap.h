#pragma once

namespace som
{
	using namespace som;

	const int FEATURE_DIMS = 2;

	/**
	*	@struct Feature
	*	@brief	�e�m�[�h���������x�N�g��
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
	*	@brief	SOM�̃m�[�h
	**/
	typedef struct Node
	{
		som::Feature f;					//	���M�̓����x�N�g��
		std::vector<Node*>	neighbor;	//	�אڃm�[�h�ւ̃|�C���^

	}Node;

	/**
	*	@class	SOM�N���X
	**/
	class SelfOrganizingMap
	{
	private:

		int	m_height;
		int	m_width;
		int m_sz;
		std::vector<Node>	m_Map;	/**<	�}�b�v	*/

	private:

		/**
		*	�}�b�v������������
		*	@param	[in]	int	height	�}�b�v�̍���
		*	@param	[in]	int	width	�}�b�v�̕�
		**/
		void InitMap(int height, int width);

		/**
		*	���͂ɍł��߂��m�[�h�C���f�b�N�X��Ԃ�
		*	@param	[in]	som::Feature	feature	���͓���
		*	@param	[out]	double			dist	����
		*	@return	�C���f�b�N�X
		**/
		int	GetNearestNodeIndex(Feature feature, double &dist);

		/**
		*	�\�I�X�e�b�v
		*	@param	[in]	som::Feature	feature	�\�I������̓f�[�^
		**/
		void Exposure(Feature feature);

	public:
		SelfOrganizingMap();
		SelfOrganizingMap(int height, int width);
		~SelfOrganizingMap();

		void Train(std::vector<Feature> datas, int Loop = 100);
	};
}

