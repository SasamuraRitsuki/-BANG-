#include "../../../../../dxlib_ext/dxlib_ext.h"
#include "Spawner.h"
#include "enemy/EnemyTank.h"
#include "../../gm_obj/Bullet.h"


Spawner::Spawner(bool hard_mode) {
	//�G�̐���
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy_.emplace_back(std::make_shared<EnemyTank>(FIRST_ENEMY_POS[i], ENEMY_ANGLE[i], hard_mode));
	}
}

Spawner::~Spawner() {

}

void Spawner::update(float delta_time) {

	auto e_it = enemy_.begin();
	// �G���X�g���`�F�b�N
	while (e_it != enemy_.end()) {

		(*e_it)->update(delta_time);

		auto blt_it = (*e_it)->bullet_.begin();
		//�e���X�g���`�F�b�N
		while (blt_it != (*e_it)->bullet_.end()) {

			//�e�����˒��̎�
			if ((*blt_it)->getBulletReflected()) {
				// �G���g�̒e�ƓG�̓����蔻��̏���
				if (tnl::IsIntersectAABB((*e_it)->getTankPos(), (*e_it)->getTankSize(), (*blt_it)->getBulletPos(), (*blt_it)->getBulletSize())) {
					//�_���[�W��^����
					(*e_it)->setTankDamagedMoment(true);
					//�e������
					blt_it = (*e_it)->bullet_.erase(blt_it);
					continue;
				}
			}
			blt_it++;
		}
		if ((*e_it)->getTankDead()) {	//dead�t���O�ŏ���
			e_it = enemy_.erase(e_it);
			continue;
		}
		e_it++;
	}
	if (enemy_.size() <= 0) {
		cleared_ = true;
	}
}

void Spawner::draw(Shared<dxe::Camera> camera) {
	for (auto enemy : enemy_) {
		enemy->draw(camera);
	}
}

bool Spawner::getTankCleared() const {
	return cleared_;
}