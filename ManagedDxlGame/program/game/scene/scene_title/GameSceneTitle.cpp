#include "../../../dxlib_ext/dxlib_ext.h"
#include "GameSceneTitle.h"
#include "../scene_play/GameScenePlay.h"
#include "../../gm_mgr/GameManager.h"
#include "../../gm_mgr/ResourceManager.h"

SceneTitle::SceneTitle() {
	SetBackgroundColor(32, 32, 32);

	//�摜��ǂݍ���
	back_ground_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/title_back_ground.png");
	dark_text_box_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/gray1.jpg");
	choiced_text_box_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/gray3.jpg");
	play_rule_gfx_ = ResourceManager::GetInstance()->loadGraph("graphics/play_rule.png");
	//���y��ǂݍ���
	bgm_snd_ = ResourceManager::GetInstance()->loadSound("sound/bgm/menu_bgm.mp3");
	pray_rule_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/pray_rule_se.mp3");
	enter_snd_ = ResourceManager::GetInstance()->loadSound("sound/se/enter_se.mp3");

	//�{�^���͑I������Ă��Ȃ��摜�ɕς��Ă���
	start_box_gfx_ = dark_text_box_gfx_;
	pray_rule_box_gfx_ = dark_text_box_gfx_;
	close_box_gfx_ = dark_text_box_gfx_;
	
	//bgm�𗬂�
	PlaySoundMem(bgm_snd_, DX_PLAYTYPE_LOOP);
}

SceneTitle::~SceneTitle() {
	StopSoundMem(bgm_snd_);
}

void SceneTitle::update(float delta_time) {
	mouse_pos_.x = static_cast<int>(tnl::Input::GetMousePosition().x);
	mouse_pos_.y = static_cast<int>(tnl::Input::GetMousePosition().y);
	//�V�[�P���X�̍X�V����
	sequence_.update(delta_time);
}

bool SceneTitle::seqHome(float delta_time) {
	if (sequence_.isStart()) {
		//draw�֐��p��HOME��ʂ̃t���O�𗧂Ă�
		home_flag_ = true;
		//draw�֐��p�̗V�ѕ��̃t���O�����낷
		play_rule_flag_ = false;
		//draw�֐��p�̑I����ʂ̃t���O�����낷
		choicing_flag_ = false;
		//�o�ߎ��Ԃ����Z�b�g
		elapsed_ = 0;
	}
	//���Ԍo��
	elapsed_ += delta_time;
	//�_�ŗp�̌v�Z
	flashed_value_ = sin(elapsed_ * 5);

	//�X�^�[�g�{�^���̏���
	UpdateButtonGraphics(START_TEXT_POS,START_STR, start_box_gfx_,
		enter_snd_, ChangeState::CHOICING);
	//�V�ѕ��{�^���̏���
	UpdateButtonGraphics(PLAY_RULE_TEXT_POS, PLAY_RULE_STR, pray_rule_box_gfx_,
		pray_rule_snd_, ChangeState::PLAY_RULE);

	return true;
}

bool SceneTitle::seqPlayRule(float delta_time) {
	if (sequence_.isStart()) {
		//draw�֐��p�ɗV�ѕ��̃t���O�𗧂Ă�
		play_rule_flag_ = true;
		//�u���N���b�N�������ăl�v���\���ɂ���
		flashed_value_ = 0;
	}
	//����{�^���̏���
	UpdateButtonGraphics(PLAY_RULE_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_,
		pray_rule_snd_, ChangeState::HOME);

	return true;
}

bool SceneTitle::seqChoicing(float delta_time) {
	if (sequence_.isStart()) {
		//draw�֐��p�ɑI����ʂ̃t���O�𗧂Ă�
		choicing_flag_ = true;
		//draw�֐��p��HOME��ʂ̃t���O�����낷
		home_flag_ = false;
		//�u���N���b�N�������ăl�v���\���ɂ���
		flashed_value_ = 0;
	}
	//�ȒP�{�^���̏���
	UpdateButtonGraphics(EASY_TEXT_POS, EASY_STR, easy_box_gfx_,
		enter_snd_, ChangeState::EASY_GAME);
	//����{�^���̏���
	UpdateButtonGraphics(HARD_TEXT_POS, HARD_STR, hard_box_gfx_,
		enter_snd_, ChangeState::HARD_GAME);
	//����{�^���̏���
	UpdateButtonGraphics(CHOICING_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_,
		pray_rule_snd_, ChangeState::HOME);

	return true;
}

void SceneTitle::draw() {
	//�w�i�摜�̕`��
	DrawGraphF(0,0, back_ground_gfx_,false);

	//�����x���グ��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TITLE_BACK_GFX_ALPHA);
	//�^�C�g���̗��̍����w�i
	DrawExtendGraph(TITLE_BACK_POS_FIRST.x, TITLE_BACK_POS_FIRST.y,
		TITLE_BACK_POS_LAST.x, TITLE_BACK_POS_LAST.y, dark_text_box_gfx_, false);
	//�V�ѕ������Ă��鎞�̌��̔w�i
	if (play_rule_flag_) {
		DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, dark_text_box_gfx_, false);
	}
	//�����x��߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//�^�C�g���̕\��
	SetFontSize(RESULT_FONTSIZE);
	DrawStringEx(TITLE_TEXT_POS.x, TITLE_TEXT_POS.y, TEXT_COLOR, TITLE_STR.c_str());
	
	//�N���b�N�e�L�X�g�̕\��
	SetFontSize(CLICK_FONTSIZE);
	if (flashed_value_ > 0) {
		DrawStringEx(CLICK_TEXT_POS.x, CLICK_TEXT_POS.y, 0, CLICK_STR.c_str());
	}

	SetFontSize(NORMAL_FONTSIZE);
	if (home_flag_) {
		//�u�X�^�[�g�v�̃e�L�X�g�{�b�N�X
		CreateTextBox(START_TEXT_POS, START_STR, start_box_gfx_);
		//�u�V�ѕ��v�̃e�L�X�g�{�b�N�X
		CreateTextBox(PLAY_RULE_TEXT_POS, PLAY_RULE_STR, pray_rule_box_gfx_);
	}
	//�V�ѕ��̕\��
	if (play_rule_flag_) {
		//�V�ѕ��̉摜�̕\��
		DrawGraphF(0, 0, play_rule_gfx_, true);
		//�u����v�̃e�L�X�g�{�b�N�X
		CreateTextBox(PLAY_RULE_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_);
	}
	//�I����ʂ̕\��
	if (choicing_flag_) {
		//�u�ȒP�v�̃e�L�X�g�{�b�N�X
		CreateTextBox(EASY_TEXT_POS, EASY_STR, easy_box_gfx_);
		//�u����v�̃e�L�X�g�{�b�N�X
		CreateTextBox(HARD_TEXT_POS, HARD_STR, hard_box_gfx_);
		//�u����v�̃e�L�X�g�{�b�N�X
		CreateTextBox(CHOICING_CLOSE_TEXT_POS, CLOSE_STR, close_box_gfx_);
	}
}

void SceneTitle::UpdateButtonGraphics(tnl::Vector2i text_pos, std::string str,
	int& box_gfx, int snd, ChangeState seq) {
	//�}�E�X���X�^�[�g�{�^���ɗ��Ă�����
	if (IsMouseInTextBox(TextBoxPosFirst(text_pos), TextBoxPosLast(text_pos, str))) {
		//�I�𒆂̉摜�Ƀe�L�X�g�{�b�N�X��ς���
		box_gfx = choiced_text_box_gfx_;
		//���N���b�N���ꂽ��
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
			//���ʉ��𗬂�
			PlaySoundMem(snd, DX_PLAYTYPE_BACK);

			//�I����ʂɑJ�ڂ��鎞
			if (seq == ChangeState::CHOICING) {
				//�I����ʂɑJ��
				sequence_.change(&SceneTitle::seqChoicing);
			}
			//�V�ѕ���ʂɑJ�ڂ��鎞
			else if (seq == ChangeState::PLAY_RULE) {
				//�V�ѕ���ʂɑJ��
				sequence_.change(&SceneTitle::seqPlayRule);
			}
			//HOME��ʂɑJ�ڂ��鎞
			else if (seq == ChangeState::HOME) {
				//�V�ѕ���ʂɑJ��
				sequence_.change(&SceneTitle::seqHome);
			}
			//�Q�[����ʂɑJ�ڂ��鎞
			else {
				//�ȒP���[�h�̎�
				if (seq == ChangeState::EASY_GAME) {
					//�Q�[����ʂɑJ��
					auto mgr = GameManager::GetInstance();
					mgr->changeScene(std::make_shared<ScenePlay>(false));
				}
				//������[�h�̎�
				else if (seq == ChangeState::HARD_GAME) {
					//�Q�[����ʂɑJ��
					auto mgr = GameManager::GetInstance();
					mgr->changeScene(std::make_shared<ScenePlay>(true));
				}
			}
		}
	}
	else {
		//�}�E�X�����Ă��Ȃ����͈Â����Ă���
		box_gfx = dark_text_box_gfx_;
	}
}

bool SceneTitle::IsMouseInTextBox(
	tnl::Vector2i text_box_pos_first, tnl::Vector2i text_box_pos_last) {
	//�}�E�X���e�L�X�g�{�b�N�X�̒��ɗ�����
	if (mouse_pos_.x >= text_box_pos_first.x && mouse_pos_.x <= text_box_pos_last.x &&
		mouse_pos_.y >= text_box_pos_first.y && mouse_pos_.y <= text_box_pos_last.y) {
		return true;
	}
	else {
		return false;
	}
}

void SceneTitle::CreateTextBox(tnl::Vector2i text_pos, std::string str, int box_gfx) {
	tnl::Vector2i start_box_pos_first = TextBoxPosFirst(text_pos);
	tnl::Vector2i start_box_pos_last = TextBoxPosLast(text_pos, str);
	//�e�L�X�g�{�b�N�X�̕`��
	DrawExtendGraph(start_box_pos_first.x, start_box_pos_first.y,
		start_box_pos_last.x, start_box_pos_last.y, box_gfx, false);
	//�����̕`��
	DrawStringEx(text_pos.x, text_pos.y, TEXT_COLOR, str.c_str());
}

tnl::Vector2i SceneTitle::TextBoxPosFirst(tnl::Vector2i text_pos) {
	//�e�L�X�g�{�b�N�X�̍���̍��W
	return { text_pos.x - TEXT_AROUND_WIDTH,text_pos.y - TEXT_AROUND_WIDTH };
}

tnl::Vector2i SceneTitle::TextBoxPosLast(tnl::Vector2i text_pos, std::string str) {
	return { text_pos.x + (NORMAL_FONTSIZE * static_cast<int>(str.length()) / 2) + 
		TEXT_AROUND_WIDTH, text_pos.y + NORMAL_FONTSIZE + TEXT_AROUND_WIDTH };
}