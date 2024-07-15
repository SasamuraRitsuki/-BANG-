#include "../../../../../dxlib_ext/dxlib_ext.h"
#include "Spawner.h"
#include "enemy/EnemyTank.h"
#include "../../gm_obj/Bullet.h"


Spawner::Spawner(bool hard_mode) {
	//敵の生成
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy_.emplace_back(std::make_shared<EnemyTank>(FIRST_ENEMY_POS[i], ENEMY_ANGLE[i], hard_mode));
	}
}

Spawner::~Spawner() {

}

void Spawner::update(float delta_time) {

	auto e_it = enemy_.begin();
	// 敵リストをチェック
	while (e_it != enemy_.end()) {

		(*e_it)->update(delta_time);

		auto blt_it = (*e_it)->bullet_.begin();
		//弾リストをチェック
		while (blt_it != (*e_it)->bullet_.end()) {

			//弾が反射中の時
			if ((*blt_it)->getBulletReflected()) {
				// 敵自身の弾と敵の当たり判定の処理
				if (tnl::IsIntersectAABB((*e_it)->getTankPos(), (*e_it)->getTankSize(), (*blt_it)->getBulletPos(), (*blt_it)->getBulletSize())) {
					//ダメージを与える
					(*e_it)->setTankDamagedMoment(true);
					//弾を消す
					blt_it = (*e_it)->bullet_.erase(blt_it);
					continue;
				}
			}
			blt_it++;
		}
		if ((*e_it)->getTankDead()) {	//deadフラグで消滅
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