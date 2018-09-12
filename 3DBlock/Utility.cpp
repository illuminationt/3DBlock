#include"Vertex.h"
#include"Graphics.h"
#include"Utility.h"


float random(int min, int max) {
	std::random_device rnd;
	// メルセンヌ・ツイスタ 引数は初期シード値 ファンクタを渡す
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<int> rand(min,max);
	
	return (float)rand(mt);
}