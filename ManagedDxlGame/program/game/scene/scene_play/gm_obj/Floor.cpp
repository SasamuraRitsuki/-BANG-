#pragma once
#include "../../../../dxlib_ext/dxlib_ext.h"
#include "floor.h"

Floor::Floor(bool hard_mode) {
	//難しいモードの時、床を速く
	if (hard_mode) {
		move_floor_speed_ = MOVE_FLOOR_SPEED_HARD;
	}
	//簡単モードの時、床を遅く
	else {
		move_floor_speed_ = MOVE_FLOOR_SPEED_EASY;
	}
	//--------------------------------床1-----------------------------------------
	floor1_mesh_ = dxe::Mesh::CreateCubeMV(FLOOR_SIZE);
	floor1_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray3.jpg"));
	floor1_mesh_->scl_ = FLOOR1_FORM;
	floor1_mesh_->pos_ = floor1_pos_;
	floor1_mesh_->setCullingMode(DX_CULLING_RIGHT);

	//当たり判定用のサイズの計算
	floor1_size_v = FLOOR1_FORM * FLOOR_SIZE;
	//--------------------------------床2-----------------------------------------
	floor2_mesh_ = floor1_mesh_->createClone();
	floor2_mesh_->scl_ = FLOOR2_FORM;
	floor2_mesh_->pos_ = floor2_pos_;
	floor2_mesh_->setCullingMode(DX_CULLING_RIGHT);

	//当たり判定用のサイズの計算
	floor2_size_v = FLOOR2_FORM * FLOOR_SIZE;
	//--------------------------------動く床1-----------------------------------------
	move_floor1_mesh_ = dxe::Mesh::CreateCubeMV(MOVE_FLOOR_SIZE);
	move_floor1_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/gray1.jpg"));
	move_floor1_mesh_->scl_ = MOVE_FLOOR_FORM;
	move_floor1_mesh_->pos_ = MOVE_FLOOR1_POS;
	move_floor1_mesh_->setCullingMode(DX_CULLING_RIGHT);
	//--------------------------------動く床2-----------------------------------------
	move_floor2_mesh_ = move_floor1_mesh_->createClone();
	move_floor2_mesh_->scl_ = MOVE_FLOOR_FORM;
	move_floor2_mesh_->pos_ = MOVE_FLOOR2_POS;
	move_floor2_mesh_->setCullingMode(DX_CULLING_RIGHT);

	//動く床の当たり判定用のサイズの計算
	move_floor_size_v = MOVE_FLOOR_FORM * MOVE_FLOOR_SIZE;
}

Floor::~Floor() {

}

void Floor::update(float delta_time) {
	elapsed_ += delta_time;

	delta_move_floor1_distance_ = sin(elapsed_ * move_floor_speed_) * MOVE_FLOOR_DISTANCE;

	move_floor1_pos_.x = MOVE_FLOOR1_POS.x + delta_move_floor1_distance_;

	delta_move_floor2_distance_ = -sin(elapsed_ * move_floor_speed_) * MOVE_FLOOR_DISTANCE;

	move_floor2_pos_.x = MOVE_FLOOR2_POS.x + delta_move_floor2_distance_;

	//メッシュに反映
	move_floor1_mesh_->pos_.x = move_floor1_pos_.x;
	move_floor2_mesh_->pos_.x = move_floor2_pos_.x;
}

void Floor::draw(Shared<dxe::Camera> camera) {
	floor2_mesh_->render(camera);

	move_floor2_mesh_->render(camera);

	move_floor1_mesh_->render(camera);

	floor1_mesh_->render(camera);
}
