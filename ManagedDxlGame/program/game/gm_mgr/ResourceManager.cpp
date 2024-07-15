#include "../../dxlib_ext/dxlib_ext.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ResourceManager* ResourceManager::GetInstance()
{
	static ResourceManager* instance = nullptr;
	if (!instance) {
		instance = new ResourceManager();
	}
	return instance;
}

void ResourceManager::Destroy()
{
	delete GetInstance();
}

int ResourceManager::loadGraph(const std::string& file_path)
{
	auto it = graphics_map_.find(file_path);

	if (it != graphics_map_.end()) {
		return graphics_map_[file_path];
	}
	int gfx_hdl = LoadGraph(file_path.c_str(), true);
	graphics_map_.insert(std::make_pair(file_path, gfx_hdl));
	return gfx_hdl;
}

int ResourceManager::loadSound(const std::string& file_path)
{
	auto it = sounds_map_.find(file_path);

	if (it != sounds_map_.end()) {
		return sounds_map_[file_path];
	}

	int snd_hdl = LoadSoundMem(file_path.c_str());
	sounds_map_.insert(std::make_pair(file_path, snd_hdl));
	//âπó í≤êÆ
	SetVolumeSoundMem(SOUND_VOL, snd_hdl);
	return snd_hdl;
}
