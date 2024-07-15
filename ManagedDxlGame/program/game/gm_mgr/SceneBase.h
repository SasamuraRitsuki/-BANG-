#pragma once

class SceneBase {
public:
	virtual void update(float delta_time) {};
	virtual void draw() {};

protected:
	//bgmの音楽パス
	int bgm_snd_;
};