#include "../dxlib_ext/dxlib_ext.h"
#include "SkyBox.h"

SkyBox::SkyBox(bool hard_mode){
	//�X�J�C�{�b�N�X�̃p�����[�^�[��ǂݍ���
	SetDefaultLightParameter("directional_light_parameter.bin");
	//�X�J�C�{�b�N�X�̃��b�V�����쐬
	skybox_mesh_ = dxe::Mesh::CreateCubeMV(SKYBOX_VALUE.x, SKYBOX_VALUE.y, SKYBOX_VALUE.z);
	
	//������[�h�̎��̃X�J�C�{�b�N�X�̉摜
	if (hard_mode) {
		skybox_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_a.png"));
	}
	//�ȒP���[�h�̎��̃X�J�C�{�b�N�X�̉摜
	else {
		skybox_mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/skybox/_skybox_c.png"));
	}
	//�}�e���A����ǂݍ���
	skybox_mesh_->loadMaterial("material.bin");
	//�X�J�C�{�b�N�X�̏����\���ݒ�
	skybox_mesh_->rot_ = tnl::Quaternion::RotationAxis(SKYBOX_ROT_AXIS, tnl::ToRadian(SKYBOX_ROT_FIRST_VALUE));
}

SkyBox::~SkyBox(){

}

void SkyBox::update(float delta_time){
	//�X�J�C�{�b�N�X��������Ƃ�������
	skybox_mesh_->rot_ *= tnl::Quaternion::RotationAxis(SKYBOX_ROT_AXIS, tnl::ToRadian(SKYBOX_ROT_VALUE));
}

void SkyBox::draw(Shared<dxe::Camera> camera){

	//�X�J�C�{�b�N�X�̕`��
	skybox_mesh_->render(camera);
}
