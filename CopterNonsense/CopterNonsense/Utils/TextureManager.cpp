#include "TextureManager.h"	

TextureManager* TextureManager::p_manager_ = nullptr;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	assetCache_.clear(); //Clear the vector of all assets
}

int TextureManager::loadTexture(const std::string& filename, const std::string& name)
{
	
	if (!isInList(name))
	{
		TextureAsset a; 
		a.name = name;
		if (!a.texture.loadFromFile(resource_folder + filename))
		{
			std::cout << "Failed to load asset: " << assetCache_.back().name << std::endl;
			return(-1);
		}
		assetCache_.push_back(a);
		return(assetCache_.size() - 1);
	}
	return(getID(name));
}

int TextureManager::getID(const std::string& name)
{//Return the id of a texture with a given name in the cache list
	
	for (int i(0); i < assetCache_.size(); ++i)
		if (assetCache_[i].name == name)
			return(i);

	return(-1);
}

TextureManager* TextureManager::instance()
{
	if (!p_manager_)
		p_manager_ = new TextureManager();
	return(p_manager_);
}
