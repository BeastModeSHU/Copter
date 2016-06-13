#pragma once
#include <SFML\Graphics.hpp> 
#include <vector>
#include <iostream>

/*
Texture cache where all objects load their textures from
-Written by: M. Walker
*/
class TextureManager
{
public:
	~TextureManager();
	int loadTexture(const std::string & file_loc, const std::string& name);
	sf::Texture* getTexture(int i) { return &assetCache_[i].texture; };
	int getID(const std::string& name);
	void setResourceFolder(const std::string &folder) { resource_folder = folder; };//set which folder we're loading resources from
	static TextureManager* instance();
private:
	TextureManager();
	static TextureManager* p_manager_; 
	bool isInList(const std::string& name)
	{//if there's a texture with a given name in the cache return true else false
		for (int i(0); i < static_cast<int>(assetCache_.size()); ++i)
			if (assetCache_[i].name == name)
				return(true);
		return(false);
	}

	struct TextureAsset
	{
		TextureAsset()
			:name(" ")
		{

		}
		TextureAsset(const std::string& name)
			:name(name)
		{

		}
		sf::Texture texture;
		std::string name;
	};
	std::string resource_folder = "res//";//the folder where resources can be found 

	std::vector<TextureAsset> assetCache_;
};

