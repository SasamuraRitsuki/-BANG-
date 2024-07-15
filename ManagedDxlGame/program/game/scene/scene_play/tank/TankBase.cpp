#include "../../../../dxlib_ext/dxlib_ext.h"
#include "../../../gm_mgr/ResourceManager.h"
#include "TankBase.h"
#include"../gm_obj/Wall.h"
#include "../gm_obj/Bullet.h"
#include"enemy_spawner/Spawner.h"
#include"player/PlayerTank.h"

bool TankBase::TryBulletToWallCollision(std::shared_ptr<Bullet> blt, std::shared_ptr<Wall> wall_) {
	//’e‚Æ•Ç‚ª“–‚½‚Á‚½‚Æ‚«
	if (tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), wall_->getWall1Pos(), wall_->getWall1Size()) ||
		tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), wall_->getWall2Pos(), wall_->getWall2Size()) ||
		tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), wall_->getWall3Pos(), wall_->getWall3Size()) ||
		tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), wall_->getWall4Pos(), wall_->getWall4Size()) ||
		tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), wall_->getWall5Pos(), wall_->getWall5Size())
		) {
		//true‚ð•Ô‚·
		return true;
	}
	else return false;
}

void TankBase::LoadCommonGraph() {
	hp_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/green1.jpg");
	hp_back_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/black2.jpg");
}

void TankBase::LoadCommonSound() {
	damage_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/damage_se.mp3");
}

void TankBase::setTankPos(tnl::Vector3 tank_pos) {
	tank_pos_ = tank_pos;
}

void TankBase::setTankDamaged(bool damaged) {
	damaged_ = damaged;
}

void TankBase::setTankHp(int hp) {
	tank_hp_ = hp;
}

void TankBase::setTankDamagedMoment(bool damaged_moment) {
	damaged_moment_ = damaged_moment;
}

void TankBase::setPlayerPointer(std::shared_ptr<PlayerTank> player) {
	player_ = player;
}

void TankBase::setFloorPointer(std::shared_ptr<Floor> floor) {
	floor_ = floor;
}

void TankBase::setWallPointer(std::shared_ptr<Wall> wall) {
	wall_ = wall;
}

void TankBase::setSpawnerPointer(std::shared_ptr<Spawner> spawner) {
	spawner_ = spawner;
}

tnl::Vector3 TankBase::getTankPos() const {
	return tank_pos_;
}

tnl::Vector3 TankBase::getTankSize() const {
	return tank_size_;
}

int TankBase::getTankHp() const {
	return tank_hp_;
}

bool TankBase::getTankDead() const {
	return dead_;
}

bool TankBase::getTankRolling() const {
	return rolling_;
}

bool TankBase::getTankDamaged() const {
	return damaged_;
}