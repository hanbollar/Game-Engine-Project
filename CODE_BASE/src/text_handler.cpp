#include "text_handler.h"

TextHandler::TextHandler() : character_map_(nullptr) {}

TextHandler::TextHandler(std::map<char, glm::mat4x2>* character_map)
	: character_map_(character_map) {}

TextHandler::~TextHandler() {
	delete character_map_;
}

glm::mat4x2 TextHandler::GetTextureCoordinatesForChar(char& c) {
	return character_map_->at(c);
}

void TextHandler::GetTextureCoordinatesForString(
	const char* string_of_characters,
	unsigned int& string_length,
	std::vector<glm::mat4x2>* texture_coordinates) {

	for (unsigned int i = 0; i < string_length; ++i) {
		texture_coordinates->push_back(character_map_->at(string_of_characters[i]));
	}
}

