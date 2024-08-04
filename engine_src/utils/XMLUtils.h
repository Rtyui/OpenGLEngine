#pragma once

#include "rendering/Mesh.h"
#include "rendering/Material.h"
#include "rendering/internal/Texture.h"
#include "ui/primitives/Font.h"

#include <vendor/pugixml/pugixml.hpp>
#include <vendor/glm/glm.hpp>

bool ParseVec3FromXMLNode(const pugi::xml_node& node, const char* vecName, glm::vec3& vec, const bool& mandatory);

bool ParseVec4FromXMLNode(const pugi::xml_node& node, const char* vecName, glm::vec4& vec, const bool& mandatory);

bool ParseUintFromXMLNode(const pugi::xml_node& node, const char* uintName, unsigned& val, const bool& mandatory);

bool ParseFloatFromXMLNode(const pugi::xml_node& node, const char* floatName, float& val, const bool& mandatory);

bool ParseBoolFromXMLNode(const pugi::xml_node& node, const char* boolName, bool& val, const bool& mandatory);

bool ParseMeshFromXMLNode(const pugi::xml_node& node, Mesh** mesh, const bool& mandatory);

bool ParseMaterialFromXMLNode(const pugi::xml_node& node, Material** material, const bool& mandatory);

bool ParseTextureFromXMLNode(const pugi::xml_node& node, Texture** texture);

bool ParseFontFromXMLNode(const pugi::xml_node& node, Font** font);

bool ParseStringFromXMLNode(const pugi::xml_node& node, const char* stringName, std::string val, const bool& mandatory);