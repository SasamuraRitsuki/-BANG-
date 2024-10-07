#pragma once

class SceneResult{
public:
	SceneResult(bool cleared = false);
	~SceneResult();

	void update(float delta_time);
	void draw();

private:
	//���ʂ̃t�H���g�̑傫��
	const int RESULT_FONTSIZE = 150;
	//�ʏ�t�H���g�̑傫��
	const int NORMAL_FONTSIZE = 50;

	//�N���A�̕���
	const std::string CLEAR_TEXT = "�Q�[���N���A";
	//�Q�[���I�[�o�[�̕���
	const std::string OVER_TEXT = "�Q�[���I�[�o�[";
	//�N���b�N�̕���
	const std::string CLICK_TEXT = "���N���b�N�������ăl";


	//�w�i�̕\�����W
	const tnl::Vector3 BACK_GROUND_POS = { 0.0f,0.0f,0.0f };
	//�N���A�e�L�X�g�̍��W
	const tnl::Vector2i CLEAR_TEXT_POS = { 180,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//�Q�[���I�[�o�[�e�L�X�g�̍��W
	const tnl::Vector2i OVER_TEXT_POS = { 100,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//�G���^�[�̍��W
	const tnl::Vector2i CLICK_TEXT_POS = { 345,460 };
	//�_�ł̒����l(sin�l�œ_�ł����邽��0)
	const float FLASHED_CENTER_VALUE = 0.0f;
	//�e�L�X�g�̃J���[�R�[�h
	const int TEXT_COLOR = 0xffd700;
	//�����w�i�̉摜�̓����x
	const int BACK_GFX_ALPHA = 125;
	//�_�Ŏ��Ԃ̒l
	const int FLASHED_VALUE = 5;
	//�����x���ő�̒l
	const int TRANSPARENCY_MAX = 255;

	//�o�ߎ���
	float timeleft_ = 0;
	//�_�ł̒l
	float flashed_value_ = 0;
	//�Q�[�����N���A���ꂽ���ǂ���
	bool cleared_ = false;

	//���̍����w�i�̉摜�p�X
	int back_gfx_;
	//�Q�[���I�����̌��ʉ�
	int finish_snd_;
	//���艹�̉��y�p�X
	int enter_snd_;
};