#pragma once

class SceneBase {
public:
	virtual void update(float delta_time) {};
	virtual void draw() {};

protected:
	//bgm‚Ì‰¹ŠyƒpƒX
	int bgm_snd_;
};