#pragma once

enum aStarDir{D_NONE, D_UP, D_UPRIGHT, D_RIGHT, D_DOWNRIGHT, D_DOWN, D_DOWNLEFT, D_LEFT, D_UPLEFT, D_COUNT,};
enum {FP_NOWALK,FP_ISWALK,FP_CLOSE};

class Astar
{
public:
	struct asNode
	{
		int32			row,col;
		char			data;
		char			findState; // 0 δ�߹�, 1 �߹�;
		int32			h,g,f;

		asNode*	parent;
		asNode*	child;
	};
public:
	Astar(void);
	~Astar(void);
public:
	bool create(int32 row, int32 col, const char* data);
	bool find(int32 startRow, int32 startCol, int32 endRow, int32 endCol);
	asNode* end_node();
	static bool points(std::list<Point2>& vecPoints, asNode* node);
private:
	void _find(const int32 g, const Point2& curPos, const Point2& checkPos);
	bool _can_walk(const Point2& checkPos);
	bool _end_point(const int32 row, const int32 col);			// �Ƿ����յ�;
	asNode* getFminNode();						// �õ�������С�ĵ�;
	bool _check_point(const int32 row, const int32 col);
	void clearData();
private:
	asNode*							mData; // [mRow][mCol]
	int32							mRow, mCol;
	Point2							mStart;
	Point2							mEnd;
	asNode*							mEndNode;
	std::list<asNode*>				mList;
};