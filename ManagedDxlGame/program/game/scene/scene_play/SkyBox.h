#pragma once

class SkyBox {
public:
	SkyBox(bool hard_mode);
	~SkyBox();

	void update(float delta_time);
	void draw(Shared<dxe::Camera> camera);

private:
	//スカイボックスのポインタ
	Shared<dxe::Mesh> skybox_mesh_;
	
	//スカイボックスのサイズ、横分割数、縦分割数
	const tnl::Vector3 SKYBOX_VALUE = { 30000.0f, 20.0f, 20.0f };
	//スカイボックスの回転向き
	const tnl::Vector3 SKYBOX_ROT_AXIS = { 0.0f, 1.0f, 0.0f };
	//スカイボックスの増加回転量
	const float SKYBOX_ROT_VALUE = 0.06f;
	//スカイボックスの初期回転量
	const int SKYBOX_ROT_FIRST_VALUE = 90;
};