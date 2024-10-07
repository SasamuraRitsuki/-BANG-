#pragma once
#include "../../gm_mgr/SceneBase.h"

class SceneTitle : public SceneBase {
public:
	SceneTitle();
	~SceneTitle();

	void update(float delta_time) override;
	void draw() override;
private:
	//���݂̏��
	tnl::Sequence<SceneTitle> sequence_ = tnl::Sequence<SceneTitle>(this, &SceneTitle::seqHome);
	//HOME���
	bool seqHome(float delta_time);
	//�V�ѕ��̉��
	bool seqPlayRule(float delta_time);
	//�I�����
	bool seqChoicing(float delta_time);
	//�ǂ̏�ԂɈړ����邩
	enum ChangeState {
		HOME,
		PLAY_RULE,
		CHOICING,
		EASY_GAME,
		HARD_GAME
	};

	//�����w�i�̃e�L�X�g�{�b�N�X�����֐�
	void CreateTextBox(tnl::Vector2i text_pos, std::string str, int box_gfx);
	//�{�^�����}�E�X�ɗ������̏������܂Ƃ߂��֐�
	void UpdateButtonGraphics(tnl::Vector2i text_pos, std::string str, int& box_gfx, int snd, ChangeState seq);
	//�}�E�X���e�L�X�g�{�b�N�X�ɗ������ǂ���
	bool IsMouseInTextBox(tnl::Vector2i text_box_pos_first,tnl::Vector2i text_box_pos_last );
	//�e�L�X�g�{�b�N�X�̍���̍��W
	tnl::Vector2i TextBoxPosFirst(tnl::Vector2i text_pos);
	//�e�L�X�g�{�b�N�X�̉E���̍��W
	tnl::Vector2i TextBoxPosLast(tnl::Vector2i text_pos, std::string str);

	//�_�ł̒����l(sin�l�œ_�ł����邽��0)
	const float FLASHED_CENTER_VALUE = 0.0f;
	//�^�C�g���̃t�H���g�̑傫��
	const int RESULT_FONTSIZE = 150;
	//�ʏ�t�H���g�̑傫��
	const int NORMAL_FONTSIZE = 50;
	//�N���b�N�t�H���g�̑傫��
	const int CLICK_FONTSIZE = 40;
	//�e�L�X�g�̃J���[�R�[�h
	const int TEXT_COLOR = 0xffd700;
	//�N���b�N�e�L�X�g�̃J���[�R�[�h
	const int CLICK_TEXT_COLOR = 0;
	//�^�C�g���̍����w�i�̉摜�̓����x
	const int TITLE_BACK_GFX_TRANSPARENCY = 50;
	//�����x���ő�̒l
	const int TRANSPARENCY_MAX = 255;
	//�e�L�X�g�̎���̍��w�i�̕�
	const int TEXT_AROUND_WIDTH = 5;
	//���ʂ̑傫��
	const int SND_VOL = 8000;
	//�_�Ŏ��Ԃ̒l
	const int FLASHED_VALUE = 5;
	//�S�p������2���̕�����邽�ߒ�������{��
	const int FULL_WIDTH_CHAR_RATIO = 2;
	//�^�C�g���̕���
	const std::string TITLE_TEXT = "��Ԃ�BANG!!!";
	//�N���b�N�̕���
	const std::string CLICK_TEXT = "���N���b�N�������ăl";
	//�X�^�[�g�̕���
	const std::string START_TEXT = "�X�^�[�g";
	//�V�ѕ��̕���
	const std::string PLAY_RULE_TEXT = "�V�ѕ�";
	//����̕���
	const std::string CLOSE_TEXT = "�߂�";
	//����̕���
	const std::string EASY_TEXT = "�ȒP";
	//����̕���
	const std::string HARD_TEXT = "���";

	//�w�i�̕\�����W
	const tnl::Vector3 BACK_GROUND_POS = { 0.0f,0.0f,0.0f };
	//�^�C�g���̃e�L�X�g�̍��W
	const tnl::Vector2i TITLE_TEXT_POS = { 150,50 };
	//�X�^�[�g�̃e�L�X�g�̍��W
	const tnl::Vector2i START_TEXT_POS = { DXE_WINDOW_WIDTH / 2 - (50 * 2),470 };
	//�V�ѕ��̃e�L�X�g�̍��W
	const tnl::Vector2i PLAY_RULE_TEXT_POS = { DXE_WINDOW_WIDTH / 2 - 75,555 };
	//�V�ѕ���ʂ̕���̃e�L�X�g�̍��W
	const tnl::Vector2i PLAY_RULE_CLOSE_TEXT_POS = { 970,120 };
	//�I����ʂ̕���̃e�L�X�g�̍��W
	const tnl::Vector2i CHOICING_CLOSE_TEXT_POS = { 1050,400 };
	//�ȒP�̃e�L�X�g�̍��W
	const tnl::Vector2i EASY_TEXT_POS = { 440,510 };
	//����̃e�L�X�g�̍��W
	const tnl::Vector2i HARD_TEXT_POS = { 740,510 };
	//�N���b�N�̃e�L�X�g�̍��W
	const tnl::Vector2i CLICK_TEXT_POS = { DXE_WINDOW_WIDTH / 2 - (CLICK_FONTSIZE * 5),640 };
	//�^�C�g���̍��w�i�̈ʒu�̍���̈ʒu
	const tnl::Vector2i TITLE_BACK_POS_FIRST = { 140,30 };
	//�^�C�g���̍��w�i�̈ʒu�̉E���̈ʒu
	const tnl::Vector2i TITLE_BACK_POS_LAST = { DXE_WINDOW_WIDTH - 140,210 };
	
	//�}�E�X�̍��W
	tnl::Vector2i mouse_pos_;
	//�w�i�̉摜�p�X
	int back_ground_gfx_;
	//���̍����w�i�̉摜�p�X
	int dark_text_box_gfx_;
	//�I������Ă鍕���w�i�̉摜�p�X
	int choiced_text_box_gfx_;
	//�V�ѕ��̉摜�p�X
	int play_rule_gfx_;
	//�V�ѕ��̑I�����̉��y�p�X
	int pray_rule_snd_;
	//���艹�̉��y�p�X
	int enter_snd_;
	//�X�^�[�g�{�^���Ƀ}�E�X�����Ă��鎞�̕\���摜
	int start_box_gfx_;
	//�V�ѕ��{�^���Ƀ}�E�X�����Ă��鎞�̕\���摜
	int pray_rule_box_gfx_;
	//����{�^���Ƀ}�E�X�����Ă��鎞�̕\���摜
	int close_box_gfx_;
	//�ȒP�{�^���Ƀ}�E�X�����Ă��鎞�̕\���摜
	int easy_box_gfx_;
	//����{�^���Ƀ}�E�X�����Ă��鎞�̕\���摜
	int hard_box_gfx_;
	//�o�ߎ���
	float elapsed_ = 0;
	//�_�ł̒l
	float flashed_value_ = 0;
	//HOME��ʂ�\������t���O
	bool home_flag_ = false;
	//�V�ѕ���\������t���O
	bool play_rule_flag_ = false;
	//�I����ʂ�\������t���O
	bool choicing_flag_ = false;
};