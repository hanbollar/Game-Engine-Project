#pragma once

#include <map>

#include "globals.h"

class TextHandler {
public:
	TextHandler();
	TextHandler(std::map<char, glm::mat4x2>* character_map);
	~TextHandler();

	void SetCharacterMapping(const std::map<char, glm::mat4x2>* new_character_map) {
		*character_map_ = *new_character_map;
	}

	// characters are returned where the letter/character represented is
	// within the texture coordinates. The texture coordinates for one
	// character is returned in the following ordering:
	// 0  2  
	// 1  3
	// In the mat4x2, rows : 0, 1, 2, 3.
	// TODO: POSSIBLY CHANGE AROUND THE ORDERING OF THE LOCATIONS
	glm::mat4x2 GetTextureCoordinatesForChar(char& c);

	void GetTextureCoordinatesForString(
		const char* string_of_characters,
		unsigned int& string_length,
		std::vector<glm::mat4x2>* texture_coordinates);

private:
	std::map<char, glm::mat4x2>* character_map_;
};
