#pragma once

class EnemyTank;

class Spawner {
public:
	Spawner(bool hard_mode);
	~Spawner();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

	//�G�̃��X�g
	std::list<std::shared_ptr<EnemyTank>> enemy_;

	//�Q�b�^�[
	bool getTankCleared() const;
	
private:
	static const int ENEMY_MAX = 3;
	//�G�̍��W
	const tnl::Vector3 FIRST_ENEMY_POS[ENEMY_MAX] = { {1200.0f,0.1f,500.0f},
		{ 200.0f,0.1f,2000.0f },{ 1000.0f,0.1f,2000.0f } };
	//�G�̏�������
	const float ENEMY_ANGLE[ENEMY_MAX] = { 270.0f ,180.0f ,180.0f };

	//�Q�[���N���A�̔���
	bool cleared_ = false;
};