#include"Vertex.h"
#include"Graphics.h"
#include"Utility.h"


float random(int min, int max) {
	std::random_device rnd;
	// �����Z���k�E�c�C�X�^ �����͏����V�[�h�l �t�@���N�^��n��
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<int> rand(min,max);
	
	return (float)rand(mt);
}