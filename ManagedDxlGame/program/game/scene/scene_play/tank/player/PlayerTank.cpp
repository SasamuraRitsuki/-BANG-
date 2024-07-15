#define _USE_MATH_DEFINES 
#include "../../../../../dxlib_ext/dxlib_ext.h"
#include "PlayerTank.h"
#include "../../../../gm_mgr/ResourceManager.h"
#include "../../gm_obj/Wall.h"
#include "../../gm_obj/Floor.h"
#include "../../gm_obj/Bullet.h"
#include "../TankBase.h"
#include "../enemy_spawner/Spawner.h"
#include "../enemy_spawner/enemy/EnemyTank.h"

PlayerTank::PlayerTank() {
	tank_size_ = TANK_FORM * TANK_SIZE;

	tank_pos_ = FIRST_POS;
	tank_hp_ = TANK_HP;
	mesh_ = dxe::Mesh::CreateBoxMV(tank_size_,
		dxe::Texture::CreateFromFile("graphics/tank/tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_side.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_up.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_down.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_side_front.jpg"),
		dxe::Texture::CreateFromFile("graphics/tank/tank_side_front.jpg")
	);
	mesh_->scl_ = TANK_FORM;
	mesh_->pos_ = FIRST_POS;
	mesh_->setCullingMode(DX_CULLING_RIGHT);

	//回転を可能状態にしておく
	rotatable_ = true;
	//スピンゲージのチャージ
	rolling_elapsed_ = ROTATABLE_TIME;
	//画像を読み込む
	LoadCommonGraph();
	spin_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/yellow1.jpg");
	back_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/white.bmp");
	bullet_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/bullet.png");
	//音楽を読み込む
	LoadCommonSound();
;	rolling_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/rolling.mp3");
	jump_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/jump.mp3");
	shot_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/player_shot.mp3");
}

PlayerTank::~PlayerTank() {

}

void PlayerTank::update(float delta_time) {
	//シーケンスのアップデート
	sequence_.update(delta_time);

	//--------------------------------------hpバーの処理------------------------------------------------
	if (hp_calc_) {
		hp_elapsed_ += delta_time;
	}
	//減少しているHPバーが目指している長さ
	float next_hp_bar_right = HP_GAUGE_POS.x + static_cast<float>((DXE_WINDOW_WIDTH * tank_hp_) / (3 * TANK_HP));

	//hpの減り方を加速してから減速
	hp_bar_right_ = tnl::SmoothLerp(hp_bar_right_, next_hp_bar_right, HP_ELAPSED_LIMIT, hp_elapsed_, 0);

	if (hp_elapsed_ >= HP_ELAPSED_LIMIT) {
		hp_calc_ = false;
		hp_elapsed_ = 0;
	}
	
	//重力による落下処理
	jump_value_ -= GRAVITY_VALUE;
	//ジャンプの値を座標に反映
	tank_pos_.y += jump_value_;
	//座標をメッシュに反映
	mesh_->pos_ = tank_pos_;
}

bool PlayerTank::seqDead(const float delta_time) {
	if (!hp_calc_) {
		dead_ = true;
	}
	return true;
}

bool PlayerTank::seqNormal(const float delta_time) {
	//押されている方向に移動
	DirChange();
	//--------------------------------------弾の発射--------------------------------------------------------
	//弾が最大数まで出ていない時
	if (bullet_.size() <= BULLET_MAX - 1) {
		//マウスの座標のチェック
		AngleCalcToMouse();
		//左クリックした時
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
			//弾の向きを自機に向ける
			tnl::Vector3 move_dir = tnl::Vector3::TransformCoord({ 0,0,1 },
				tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(static_cast<float> (angle_to_mouse_))));
			//弾の生成
			bullet_.emplace_back(std::make_shared<Bullet>(tank_pos_, move_dir, angle_to_mouse_, false));
			//効果音
			PlaySoundMem(shot_snd_, DX_PLAYTYPE_BACK);
		}
	}
	//----------------------------------回転する--------------------------------------------------
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) &&	//右クリックが押されて
		rotatable_) {											//回転可能な時に
		rolling_ = true;
		rotatable_ = false;
		rolling_elapsed_ = 0;
		//効果音
		PlaySoundMem(rolling_snd_, DX_PLAYTYPE_BACK);
	}
	//回転中の時のみ
	if (!rotatable_) {
		//戦車の回転処理
		TankRolling(delta_time);
	}
	//-------------------------------------ジャンプの処理-------------------------------------------------------
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE) &&//スペースキーが押された時
		!falling) {										//落下中じゃない時
		//上方向に値を増加
		jump_value_ = JUMP_VALUE;
		//落下中にする
		falling = true;
		//効果音
		PlaySoundMem(jump_snd_, DX_PLAYTYPE_BACK);
	}

	//--------------------------------------動く床に乗った時の処理-------------------------------------------------------
	//摩擦の計算
	tnl::EasyAdjustObjectVelocity(CENTROID_RADIUS, MASS, FRICTION,
		past_move_vel_, move_vel_, center_of_gravity_);


	//動く床の処理
	MoveFloorCalc(move_floor1_state_, tank_pos_.x, floor_->getDeltaMoveFloor1Distance(), move_vel_.x);
	MoveFloorCalc(move_floor2_state_, tank_pos_.x, floor_->getDeltaMoveFloor2Distance(), move_vel_.x);
	//戦車を動かす
	tank_pos_ += move_vel_ * MOVE_SPEED;

	//--------------------------------------------------弾の処理など-------------------------------------------------------------------

	auto blt_it = bullet_.begin();
	//弾リストをチェック
	while (blt_it != bullet_.end()) {
		//弾の更新処理
		(*blt_it)->update(delta_time);

		auto e_it = spawner_->enemy_.begin();
		//弾が生きているか
		bool is_alive_blt = false;
		// 敵リストをチェック
		while (e_it != spawner_->enemy_.end()) {
			// 弾と敵の当たり判定の処理
			if (TryEnemyCollision((*blt_it), (*e_it)->getTankPos(), (*e_it)->getTankSize())) {
				//ダメージを与える
				(*e_it)->setTankDamagedMoment(true);
				//弾を消すフラグを立てる
				is_alive_blt = true;
			}
			e_it++;
		}
		if (is_alive_blt ||								//弾が生きてなかったら
			TryBulletToWallCollision(*blt_it, wall_) ||	//壁に当たったら
			damaged_) {									//ダメージを食らったら
			blt_it = bullet_.erase(blt_it);
			is_alive_bullet_ = false;
			continue;
		}
		blt_it++;
	}

	//----------------------------------初期位置に戻る-------------------------------------------
	auto e_it = spawner_->enemy_.begin();
	while (e_it != spawner_->enemy_.end()) {
		//敵と自機との当たり判定
		//ぶつかったらダメージ
		if (tnl::IsIntersectAABB((*e_it)->getTankPos(), (*e_it)->getTankSize(), tank_pos_, tank_size_)) {
			damaged_ = true;
			break;
		}
		e_it++;
	}

	if (tank_pos_.y <= DEAD_Y ||//落下したら
		damaged_) {				//ダメージ受けたら
		if (tank_hp_ > 1) {		//HPが0じゃない時は初期値に戻す
			tank_pos_ = FIRST_POS;
			mesh_->rot_ = { 0, 0, 0 ,1 };
			jump_value_ = 0;
		}
		move_floor1_state_.is_riding_ = false;
		move_floor2_state_.is_riding_ = false;
		damaged_ = false;
		hp_calc_ = true;
		tank_hp_--;
		//効果音
		PlaySoundMem(damage_snd_, DX_PLAYTYPE_BACK);
	}

	//HPがなくなったら死亡中に遷移
	if (tank_hp_ <= 0) {
		sequence_.change(&PlayerTank::seqDead);
	}
	return true;
}

void PlayerTank::draw(Shared<dxe::Camera> camera) {

	//弾の描画
	mesh_->render(camera);
	for (auto blt : bullet_) {
		blt->draw(camera);
	}

	spin_bar_right_ = 0.5f * rolling_elapsed_ * (DXE_WINDOW_WIDTH / (3 * ROLLING_TIME)) + SPIN_GAUGE_POS.x;

	//後ろの白い薄い背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//上
	DrawExtendGraph(UP_BACK_POS_FIRST.x, UP_BACK_POS_FIRST.y,
		UP_BACK_POS_LAST.x, UP_BACK_POS_LAST.y, back_gfx_, false);
	//下
	DrawExtendGraph(DOWN_BACK_POS_FIRST.x, DOWN_BACK_POS_FIRST.y,
		DOWN_BACK_POS_LAST.x, DOWN_BACK_POS_LAST.y, back_gfx_, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//背景の黒いバー
	DrawExtendGraph(HP_GAUGE_POS.x, HP_GAUGE_POS.y,
		HP_GAUGE_POS.x + DXE_WINDOW_WIDTH / 3, HP_GAUGE_POS.y + 40, hp_back_gfx_, false);
	//緑バー
	DrawExtendGraph(HP_GAUGE_POS.x, HP_GAUGE_POS.y,
		static_cast<int>(hp_bar_right_), HP_GAUGE_POS.y + 40, hp_gfx_, false);

	//背景の黒いバー
	DrawExtendGraph(SPIN_GAUGE_POS.x, SPIN_GAUGE_POS.y,
		SPIN_GAUGE_POS.x + DXE_WINDOW_WIDTH / 3, SPIN_GAUGE_POS.y + 40, hp_back_gfx_, false);
	//黄バー
	DrawExtendGraph(SPIN_GAUGE_POS.x, SPIN_GAUGE_POS.y,
		static_cast<int>(spin_bar_right_), SPIN_GAUGE_POS.y + 40, spin_gfx_, false);

	//文字表示
	DrawStringEx(HP_TEXT_POS.x, HP_TEXT_POS.y, TEXT_COLOR, "HP");
	DrawStringEx(SPIN_TEXT_POS.x, SPIN_TEXT_POS.y, TEXT_COLOR, "SPIN");
	DrawStringEx(SPIN_CLICK_TEXT_POS.x, SPIN_CLICK_TEXT_POS.y, TEXT_COLOR, "右クリック");

	int blt_remains = BULLET_MAX - static_cast<int>(bullet_.size());
	//左上の弾の描画
	for (int i = 0; i < blt_remains; i++) {
		DrawRotaGraph(static_cast<int>(BULLET_POS.x) + i * 64, static_cast<int>(BULLET_POS.y),
			BULLET_POS.z, 0, bullet_gfx_, true);
	}

}

void PlayerTank::LandingOnTheFloor() {
	jump_value_ = 0;
	falling = false;
}

void PlayerTank::DirChange() {
	//入力されている方向への仮のベクトルの値
	tnl::Vector2i move_dir_vector = {0,0};
	
	//水平方向が押されている時
	bool horizontal_input = false;
	//垂直方向が押されている時
	bool vertical_input = false;

	//入力に応じて仮のベクトルの値を出す
	//inputは平行な2方向が押されている時は、どちらにも動かさないようにする
	//(左右に押されている時は動かさないが、3方向押されている時は動かすようにするため)
	if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
		move_dir_vector.x += 1;
		vertical_input = !vertical_input;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
		move_dir_vector.x -= 1;
		vertical_input = !vertical_input;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
		move_dir_vector.y += 1;
		horizontal_input = !horizontal_input;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
		move_dir_vector.y -= 1;
		horizontal_input = !horizontal_input;
	}
	//ベクトルの値から角度を出す。
	int dir_angle = static_cast<int>(atan2(move_dir_vector.y, move_dir_vector.x) * (180.0 / M_PI));

	//向いている方向への前進の値を出す
	tnl::Quaternion now_rot_ = tnl::Quaternion::RotationAxis(
		{ 0, 1, 0 }, tnl::ToRadian(static_cast<float>(dir_angle)));

	//左右か上下のボタンが押されてる時は前進処理
	if (horizontal_input || vertical_input) {
		move_vel_ += tnl::Vector3::TransformCoord({ 0, 0, 1.0f }, now_rot_);
		//回転中は見た目はここでは動かさない
		if (!rolling_) {
			mesh_->rot_ = now_rot_;
		}
	}
}

void PlayerTank::AngleCalcToMouse() {
	mouse_pos_.x = tnl::Input::GetMousePosition().x - DXE_WINDOW_WIDTH / 2;
	mouse_pos_.y = tnl::Input::GetMousePosition().y - DXE_WINDOW_HEIGHT / 2;

	angle_to_mouse_ = static_cast<int>(atan2(mouse_pos_.y, mouse_pos_.x) * (180 / M_PI)) + 90;
}

void PlayerTank::TankRolling(float delta_time) {
	rolling_elapsed_ += delta_time;

	// 正規化
	float normalizedTime = static_cast<float>(rolling_elapsed_) / ROLLING_TIME;

	// イージング関数を適用
	float easedTime = EaseInOut(normalizedTime);


	//ぐるぐる回転
	if (rolling_) {
		// y の変化
		rolling_speed_ = easedTime * 30.0f;
		mesh_->rot_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(rolling_speed_));
	}

	if (rolling_elapsed_ >= ROLLING_TIME && rolling_) {
		rolling_ = false;//回転中をoffに
	}
	if (rolling_elapsed_ >= ROTATABLE_TIME) {
		rolling_elapsed_ = ROTATABLE_TIME;
		rotatable_ = true;//回転可能をonに
	}
}

float PlayerTank::EaseInOut(float t) {
	// t は時間（0.0 から 1.0 の範囲で変化する値）


	// 2乗のべき乗を使った急加速から減速への変化
	/*float easedValue = t < 0.5 ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2) / 2.0f;

	return easedValue;*/

	// 等減速直線運動を利用した値の補間
	float easedValue = tnl::DecelLerp(0, 1, 1, t);

	return easedValue;
}

void PlayerTank::MoveFloorCalc(MoveFloorState& floor_state, float& tank_pos_x, float delta_move_floor_distance,float move_vel) {

	if (floor_state.is_riding_) {
		// 動く床に乗り始めた最初のフレームの処理
		if (floor_state.just_started_riding_) {
			//乗った地点の座標を記憶
			floor_state.prev_tank_pos_x_ = tank_pos_x;
			floor_state.prev_delta_move_floor_distance_ = delta_move_floor_distance;
			// 最初のフレームフラグをオフにする
			floor_state.just_started_riding_ = false;  
		}
		//床で動けるようにする+-
		floor_state.prev_tank_pos_x_ += move_vel_.x * MOVE_SPEED;
		// 戦車の位置を更新する
		tank_pos_x = floor_state.prev_tank_pos_x_ + delta_move_floor_distance - floor_state.prev_delta_move_floor_distance_;
	}
	else {
		// 床から降りたとき、次に乗るときのために最初のフレームフラグをオンにする
		floor_state.just_started_riding_ = true;
	}
}

bool PlayerTank::TryEnemyCollision(std::shared_ptr<Bullet> blt, tnl::Vector3 enemy_pos, tnl::Vector3 enemy_size) {

	if (tnl::IsIntersectAABB(enemy_pos, enemy_size, blt->getBulletPos(), blt->getBulletSize())
		) {
		return true;
	}
	else return false;
}

void PlayerTank::setIsRidingMoveFloor1(bool is_riding) {
	move_floor1_state_.is_riding_ = is_riding;
}

void PlayerTank::setIsRidingMoveFloor2(bool is_riding) {
	move_floor2_state_.is_riding_ = is_riding;
}