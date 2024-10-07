#include "../dxlib_ext/dxlib_ext.h"
#include "SkyBox.h"

SkyBox::SkyBox(bool hard_mode){
	//スカイボックスのパラメーターを読み込む
	SetDefaultLightParameter("directional_light_parameter.bin");
	//スカイボックスのメッシュを作成
	skybox_mesh_ = dxe::Mesh::CreateCubeMV(SKYBOX_VALUE.x, SKYBOX_VALUE.y, SKYBOX_VALUE.z);
	
	//難しいモードの時のスカイボックスの画像
	if (hard_mode) {
		skybox_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_a.png"));
	}
	//簡単モードの時のスカイボックスの画像
	else {
		skybox_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_c.png"));
	}
	//マテリアルを読み込む
	skybox_mesh_->loadMaterial("material.bin");
	//スカイボックスの初期表示設定
	skybox_mesh_->rot_ = tnl::Quaternion::RotationAxis(SKYBOX_ROT_AXIS, tnl::ToRadian(SKYBOX_ROT_FIRST_VALUE));
}

SkyBox::~SkyBox(){

}

void SkyBox::update(float delta_time){
	//スカイボックスをちょっとずつ動かす
	skybox_mesh_->rot_ *= tnl::Quaternion::RotationAxis(SKYBOX_ROT_AXIS, tnl::ToRadian(SKYBOX_ROT_VALUE));
}

void SkyBox::draw(Shared<dxe::Camera> camera){

	//スカイボックスの描画
	skybox_mesh_->render(camera);
}
