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
	//�N���A�e�L�X�g�̍��W
	const tnl::Vector2i CLEAR_TEXT_POS = { 180,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//�Q�[���I�[�o�[�e�L�X�g�̍��W
	const tnl::Vector2i OVER_TEXT_POS = { 100,DXE_WINDOW_HEIGHT / 2 - RESULT_FONTSIZE / 2 - 50 };
	//�G���^�[�̍��W
	const tnl::Vector2i CLICK_TEXT_POS = { 345,460 };
	//�e�L�X�g�̃J���[�R�[�h
	const int TEXT_COLOR = 0xffd700;
	//�����w�i�̉摜�̓����x
	const int BACK_GFX_ALPHA = 125;

	//�o�ߎ���
	float timeleft_ = 0;
	//�_�ł̒l
	float elapsed_ = 0;
	//�Q�[�����N���A���ꂽ���ǂ���
	bool cleared_ = false;

	//���̍����w�i�̉摜�p�X
	int back_gfx_;
	//�Q�[���I�����̌��ʉ�
	int finish_snd_;
	//���艹�̉��y�p�X
	int enter_snd_;
};