#define _USE_MATH_DEFINES
#include "../../../../../../dxlib_ext/dxlib_ext.h"
#include "EnemyTank.h"
#include "../../player/PlayerTank.h"
#include "../../../../../gm_mgr/ResourceManager.h"
#include "../../../gm_obj/Bullet.h"
#include "../../../gm_obj/Wall.h"
#include "../../TankBase.h"

EnemyTank::EnemyTank(const tnl::Vector3& spawn_pos, const float& dir_angle, bool hard_mode) {
	tank_size_ = TANK_FORM * TANK_SIZE;
	tank_pos_ = spawn_pos;
	dir_angle_ = dir_angle;
	hard_mode_ = hard_mode;

	mesh_ = dxe::Mesh::CreateBoxMV(tank_size_,
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_up.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_down.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side_front.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/enemy_tank_side_front.jpg")
	);
	mesh_->scl_ = TANK_FORM;
	mesh_->pos_ = spawn_pos;
	mesh_->setCullingMode(DX_CULLING_RIGHT);

	//最初に一回ランダムで動かす
	RandomDirChange();

	//hpの設定
	tank_hp_ = TANK_HP;
	//弾の撃つ間隔の抽選
	RandomShotCount();

	//画像を読み込む
	LoadCommonGraph();
	//音楽データを読み込む
	LoadCommonSound();
	shot_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enemy_shot.mp3");
	dead_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enemy_dead.mp3");
	reflection_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/reflection_shot.mp3");
}

EnemyTank::~EnemyTank() {

}

void EnemyTank::update(float delta_time) {
	//シーケンスのアップデート
	sequence_state_.update(delta_time);

	//HPバーの減少に用いる時間経過
	if (is_hp_counting_) {
		hp_elapsed_ += delta_time;
	}
	//-------------------------------弾のリストの処理------------------------
	auto blt_it = bullet_.begin();
	while (blt_it != bullet_.end()) {
		//弾が消えている可能性があるからチェック
		if ((*blt_it) == nullptr) {
			continue;
		}
		//弾のリストの更新処理
		(*blt_it)->update(delta_time);

		//弾が自機と当たった時
		if (TryBulletCollision(*blt_it)) {
			//回転していたら
			if (player_->getTankRolling()) {
				//向きを変える
				(*blt_it)->BulletDir(true);
				//効果音
				PlaySoundMem(reflection_snd_, DX_PLAYTYPE_BACK);
			}
			else {
				//自機がダメージ
				player_->setTankDamaged(true);
			}
		}
		if (TryBulletToWallCollision(*blt_it, wall_) ||	//弾が壁に当たったら
			player_->getTankDamaged()) {				//自機がダメージを受けたら
			blt_it = bullet_.erase(blt_it);
			continue;
		}
		blt_it++;
	}
}

bool EnemyTank::seqNotShooting(float delta_time) {
	//時間経過
	random_elapsed_ += delta_time;
	//ランダムに向きを変える時間の処理
	if (random_elapsed_ >= DIR_CHANGE_COUNT) {
		RandomDirChange();
		random_elapsed_ = 0;
	}
	//角度をdir_valueだけ変える
	dir_angle_ += dir_value_;

	//-------------------------------メッシュの更新-----------------------------------------
	//向き
	mesh_->rot_ = tnl::Quaternion::RotationAxis(
		TANK_AXIS, tnl::ToRadian(static_cast<float> (dir_angle_)));
	//進み
	//進む指令が出ている時
	if (go_judge_) {
		move_vel_ += tnl::Vector3::TransformCoord(TANK_DIR, mesh_->rot_);
	}
	//質量と摩擦の計算
	tnl::EasyAdjustObjectVelocity(CENTROID_RADIUS, MASS, FRICTION,
		past_move_vel_, move_vel_, center_of_gravity_);
	//戦車の座標を動かす
	tank_pos_ += move_vel_ * MOVE_SPEED;
	//メッシュの戦車の位置を更新
	mesh_->pos_ = tank_pos_;
	
	//弾を発射するタイミングになった時
	if (shot_elapsed_ >= shot_count_) {
		//弾を発射するまでの秒数をリセット
		shot_elapsed_ = 0;
		//状況を「弾の発射中」に変える
		sequence_shooting_.change(&EnemyTank::seqShooting);
	}
	return true;
}

bool EnemyTank::seqShooting(float delta_time) {
	if (sequence_shooting_.isStart()) {
		//自機との角度を計算
		AngleCalcToTank();
		//弾の向きを自機に向ける
		tnl::Vector3 move_dir = tnl::Vector3::TransformCoord(TANK_DIR,
			tnl::Quaternion::RotationAxis(TANK_AXIS, tnl::ToRadian(static_cast<float>(angle_to_tank_))));
		//向きを自機に向ける
		mesh_->rot_ = tnl::Quaternion::RotationAxis(TANK_AXIS, tnl::ToRadian(static_cast<float>(angle_to_tank_)));
		//弾生成
		bullet_.emplace_back(std::make_shared<Bullet>(tank_pos_, move_dir, angle_to_tank_, true));
		//効果音
		PlaySoundMem(shot_snd_, DX_PLAYTYPE_BACK);
		//弾の撃つ間隔の抽選
		RandomShotCount();
	}
	//弾を発射してから規定の時間が過ぎたら
	if (shot_elapsed_ >= SHOT_INTERVAL) {
		
		//弾を発射するまでの秒数をリセット
		shot_elapsed_ = 0;
		//状態を戻す
		sequence_shooting_.change(&EnemyTank::seqNotShooting);
	}
	return true;
}

bool EnemyTank::seqNormal(float delta_time) {
	//生きている時の更新処理
	UpdateAliveTank(delta_time);
	//ダメージを受けたら
	if (damaged_moment_) {
		//効果音
		PlaySoundMem(damage_snd_, DX_PLAYTYPE_BACK);
		//HPが1以上ならダメージ中に遷移
		if (tank_hp_ > TANK_DAED_HP) {
			sequence_state_.change(&EnemyTank::seqDamaged);
		}
	}
	//hpがなくなったら死亡中へ遷移
	if (tank_hp_ <= TANK_DAED_HP) {
		sequence_state_.change(&EnemyTank::seqDead);
	}
	return true;
}

bool EnemyTank::seqDamaged(float delta_time) {
	//ダメージを受けた時
	if (damaged_moment_) {
		//戦車のHPを減らす
		tank_hp_--;
		//hpのカウントを始める
		is_hp_counting_ = true;
		//hpバーが減る基準点を現在のhpバーの右端に変更
		hp_bar_start_right = hp_bar_length_;
		//hpバーの経過時間の値をリセット
		hp_elapsed_ = 0;
		//ダメージを受けた瞬間の判定をリセット
		damaged_moment_ = false;

		//また、ダメージ中にダメージを受けた時
		if (!sequence_state_.isStart()) {
			//効果音
			PlaySoundMem(damage_snd_, DX_PLAYTYPE_BACK);
		}
	}
	//生きている時の更新処理
	UpdateAliveTank(delta_time); 
	//規定時間で通常時に遷移
	if (hp_elapsed_ >= HP_ELAPSED_LIMIT) {
		is_hp_counting_ = false;
		sequence_state_.change(&EnemyTank::seqNormal);
	}
	//hpがなくなったら死亡中へ遷移
	if (tank_hp_ <= TANK_DAED_HP) {
		sequence_state_.change(&EnemyTank::seqDead);
	}
	return true;
}

bool EnemyTank::seqDead(float delta_time) {
	if(sequence_state_.isStart()) {
		//hpバーが減る基準点を現在のhpバーの右端に変更
		hp_bar_start_right = hp_bar_length_;
		hp_elapsed_ = 0;
		is_hp_counting_ = true;
	}
	//hpが減り切ったら死亡
	if (hp_bar_right_ <= screen_hp_bar_pos_.x) {
		dead_ = true;
		//効果音
		PlaySoundMem(dead_snd_, DX_PLAYTYPE_BACK);
	}
	return true;
}

void EnemyTank::UpdateAliveTank(float delta_time) {
	//時間経過
	shot_elapsed_ += delta_time;
	//弾の発射状況に応じて更新処理
	sequence_shooting_.update(delta_time);
}

void EnemyTank::draw(Shared<dxe::Camera> camera) {
	//現在のカメラ位置から見たHPバーの位置を出す
	screen_hp_bar_pos_ = tnl::Vector3::ConvertToScreen(
		{ tank_pos_.x - HP_BAR_HALF, HP_BAR_HEIGHT, tank_pos_.z },
		DXE_WINDOW_WIDTH,
		DXE_WINDOW_HEIGHT,
		camera->view_,
		camera->proj_);

	//メッシュの描画
	mesh_->render(camera);
	//弾の描画
	for (auto blt : bullet_) {
		blt->draw(camera);
	}
	//HPバーの描画
	
	//HPバーの計算
	HpCalc();
	int screen_hp_bar_pos_x = static_cast<int>(screen_hp_bar_pos_.x);
	int screen_hp_bar_pos_y = static_cast<int>(screen_hp_bar_pos_.y);
	//背景の黒いバー
	DrawExtendGraph(screen_hp_bar_pos_x, screen_hp_bar_pos_y,
		screen_hp_bar_pos_x + BAR_WIDTH.x, screen_hp_bar_pos_y + BAR_WIDTH.y, hp_back_gfx_, false);
	//緑バー
	DrawExtendGraph(screen_hp_bar_pos_x, screen_hp_bar_pos_y,
		static_cast<int>(hp_bar_right_), screen_hp_bar_pos_y + BAR_WIDTH.y, hp_gfx_, false);
}

void EnemyTank::RandomShotCount() {
	//難しいモードの時の弾の撃つ間隔の設定
	if (hard_mode_) {
		shot_count_ = SHOT_COUNT_AVG_HARD;
	}
	//簡単モードの時の弾の撃つ間隔の設定
	else {
		shot_count_ = SHOT_COUNT_AVG_EASY;
	}
	//弾を撃つ間隔の抽選（-0.9から+1.0のランダムを取る）
	shot_count_ += (((rand() % RANDOM_SHOT_RANGE) - RANDOM_SHOT_OFFSET) * RANDOM_SHOT_SCALE);
}

void EnemyTank::RandomDirChange() {
	//向きをランダムで変える
	dir_value_ = static_cast<float>((rand() % RANDOM_DIR_RANGE) - RANDOM_DIR_OFFSET) * RANDOM_DIR_POWER;
	//進むか止まるかをランダムで決める
	go_judge_ = rand() % RANDOM_GO_RANGE;
}

void EnemyTank::AngleCalcToTank() {
	//自機との距離を計算
	distance_to_tank_.x = static_cast<int>(tank_pos_.x - player_->getTankPos().x);
	distance_to_tank_.y = static_cast<int>(tank_pos_.z - player_->getTankPos().z);
	//自機との角度を計算
	angle_to_tank_ = static_cast<int>(atan2(distance_to_tank_.x, distance_to_tank_.y) * RAD_TO_DEG) + ANGLE_OFFSET;
}

void EnemyTank::HpCalc() {
	//減少しているHPバーが目指している長さ
	float next_hp_bar_length = static_cast<float>(BAR_WIDTH.x) -
		((tank_hp_ - TANK_HP) * -(static_cast<float>(BAR_WIDTH.x) / TANK_HP));
	if (is_hp_counting_) {
		//hpの減り方を加速してから減速
		hp_bar_length_ = tnl::SmoothLerp(hp_bar_start_right, next_hp_bar_length, HP_ELAPSED_LIMIT, hp_elapsed_, SMOOTHING_ITERATIONS);
	}
	//HPバーのX座標を実際の座標の位置に
	hp_bar_right_ = screen_hp_bar_pos_.x + hp_bar_length_;
}

bool EnemyTank::TryBulletCollision(std::shared_ptr<Bullet> blt) {

	if (tnl::IsIntersectAABB(blt->getBulletPos(), blt->getBulletSize(), player_->getTankPos(), player_->getTankSize())
		) {
		return true;
	}
	else return false;
}