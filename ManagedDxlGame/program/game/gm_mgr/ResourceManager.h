#pragma once

class ResourceManager {
public:
	//画像データの読み込み
	int loadGraph(const std::string& file_path);
	//音楽データの読み込み
	int loadSound(const std::string& file_path);

	static ResourceManager* GetInstance();
	static void Destroy();

private:
	ResourceManager();
	~ResourceManager();

	//bgmやseの音量を80%に抑える
	const int SOUND_VOL = 8000;

	//画像ファイル
	std::unordered_map<std::string, int> graphics_map_;
	//音楽ファイル
	std::unordered_map<std::string, int> sounds_map_;
};