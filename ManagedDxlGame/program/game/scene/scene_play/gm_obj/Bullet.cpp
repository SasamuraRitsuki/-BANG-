#include "../../../../dxlib_ext/dxlib_ext.h"
#include "Bullet.h"

Bullet::Bullet(const tnl::Vector3& spawn_pos, const tnl::Vector3& move_dir,const int& dir_value, const bool& enemy) {
	//“G‚È‚ç
	if (enemy) {
		texture = dxe::Texture::CreateFromFile("graphics/black1.bmp");
	}
	//–¡•û‚È‚ç
	else {
		texture = dxe::Texture::CreateFromFile("graphics/yellow1.jpg");
	}
	mesh_ = dxe::Mesh::CreateCubeMV(BULLET_SIZE);
	mesh_->setTexture(texture);
	mesh_->scl_ = BULLET_FORM;
	move_dir_ = move_dir;

	//’e‚ªo‚Ä‚­‚é‚Æ‚±‚ë‚Í­‚µ‘O‘¤
	blt_pos_ = spawn_pos + (move_dir * BULLET_SPEED * 7);
	//’e‚Ì•ûŒü‚ğŒ‚‚Á‚½•ûŒü‚É
	mesh_->rot_ = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(static_cast<float>(dir_value)));

	mesh_->setCullingMode(DX_CULLING_RIGHT);


	blt_size_v = BULLET_FORM * BULLET_SIZE;

	//Å‰‚Ì’e‚ÌŒü‚«
	BulletDir(blt_reflected_);
}

Bullet::~Bullet() {

}

void Bullet::update(float delta_time) {

	blt_pos_ += move_dir_ * BULLET_SPEED * static_cast<float>(blt_dir_);

	//ƒƒbƒVƒ…‚É”½‰f
	mesh_->pos_ = blt_pos_;
}

void Bullet::draw(Shared<dxe::Camera> camera) {
	mesh_->render(camera);
}

void Bullet::BulletDir(bool bullet_reflection) {

	if (!bullet_reflection) {
		blt_dir_ = BULLET_DIR;
	}
	//”½Ë‚µ‚Ä‚½‚ç‹tŒü‚«‚É
	else{
		blt_dir_ = BULLET_REFLECT_DIR;
		blt_reflected_ = true;
	}
}