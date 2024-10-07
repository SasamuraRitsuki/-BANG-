#pragma once

class SkyBox {
public:
	SkyBox(bool hard_mode);
	~SkyBox();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

private:
	//�X�J�C�{�b�N�X�̃|�C���^
	Shared<dxe::Mesh> skybox_mesh_;
	
	//�X�J�C�{�b�N�X�̃T�C�Y�A���������A�c������
	const tnl::Vector3 SKYBOX_VALUE = { 30000.0f, 20.0f, 20.0f };
	//�X�J�C�{�b�N�X�̉�]����
	const tnl::Vector3 SKYBOX_ROT_AXIS = { 0.0f, 1.0f, 0.0f };
	//�X�J�C�{�b�N�X�̑�����]��
	const float SKYBOX_ROT_VALUE = 0.06f;
	//�X�J�C�{�b�N�X�̏�����]��
	const int SKYBOX_ROT_FIRST_VALUE = 90;
};