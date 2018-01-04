/*
 * XMLParser.cc
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */


#include "XMLParser.h"

#include <iostream>
#include <exception>

using namespace xml_parser;

void variable::print(){
  if(dir == direction::IN){
    std::cout << directory << "/" << name << " (read/write)"
    << "\t Desc: " << desciption
    << "\t unit: " << unit
    << "\t type: " << type
    << "\t number of elements: " << nElements << std::endl;
  } else {
    std::cout << directory << "/" << name << " (read only)"
    << "\t Directory: " << directory
    << "\t Desc: " << desciption
    << "\t unit: " << unit
    << "\t type: " << type
    << "\t number of elements: " << nElements << std::endl;
  }
}

void XMLParser::readVariable(const xmlpp::Element* element, std::string directory){
  variable tmp;
  tmp.directory = directory;
  tmp.name = element->get_attribute_value("name").c_str();
  for(const auto& child : element->get_children()){
      const xmlpp::Element *node = dynamic_cast<const xmlpp::Element*>(child);
      const xmlpp::TextNode *txt = dynamic_cast<const xmlpp::TextNode*>(child);
      if(!node){
        continue;
      }
      // check for additional "text" elements
      if(txt && txt->is_white_space()){
        continue;
      }
      // check for empty elements e.g. <unit></unit>
      if(!node->has_child_text()){
        continue;
      }
      if(child->get_name() == "description") {
        try{
          tmp.desciption = node->get_child_text()->get_content().c_str();
        } catch (xmlpp::exception &e){
          std::cerr << "While reading description data:" << e.what()
                  << std::endl;
          tmp.desciption = "";
        }
      } else if (child->get_name() == "unit"){
        try{
          tmp.unit = node->get_child_text()->get_content().c_str();
        } catch (xmlpp::exception &e){
          std::cerr << "While reading unit data:" << e.what()
                  << std::endl;
          tmp.unit = "";
        }
      } else if (child->get_name() == "value_type"){
        if(std::string(node->get_child_text()->get_content().c_str()).compare("string") == 0){
          tmp.type = varType::String;
        } else if (std::string(node->get_child_text()->get_content().c_str()).compare("int32") == 0) {
          tmp.type = varType::Int32;
        } else if (std::string(node->get_child_text()->get_content().c_str()).compare("uint32") == 0) {
          tmp.type = varType::UInt32;
        } else if (std::string(node->get_child_text()->get_content().c_str()).compare("double") == 0) {
          tmp.type = varType::Double;
        } else if (std::string(node->get_child_text()->get_content().c_str()).compare("float") == 0) {
          tmp.type = varType::Float;
        } else {
          tmp.type = varType::Unknown;
          std::cerr << "Read unknown type: " << node->get_child_text()->get_content().c_str() << std::endl;
        }
      } else if (child->get_name() == "direction"){
        std::string type =  node->get_child_text()->get_content().c_str();
        if(type.compare("application_to_control_system") == 0){
          tmp.dir = direction::OUT;
        } else if (type.compare("control_system_to_application") == 0){
          tmp.dir = direction::IN;
        } else {
          throw xmlpp::exception("Unknown data flow direction: " + type);
        }
      } else if (child->get_name() == "numberOfElements"){
        tmp.nElements = atoi(node->get_child_text()->get_content().c_str());
      } else {
        std::cout << "Did not consider the following variable information: " + child->get_name() << std::endl;
      }
    }
  vars.push_back(tmp);
}

void XMLParser::readDirectory(const xmlpp::Element* element, std::string directory){
  for(const auto& child : element->get_children()){
    const xmlpp::Element *child_element = dynamic_cast<const xmlpp::Element*>(child);
    if(!child_element)
      continue;
    if(child_element->get_name() == "directory") {
      std::string dir = directory;
      if(!directory.empty())
        dir.append("/");
      dir.append(child_element->get_attribute_value("name").c_str());
      readDirectory(child_element, dir);
    } else if (child_element->get_name() == "variable"){
      readVariable(child_element, directory);
    } else {
      throw xmlpp::exception(
                "Found unknown xml element: " + child_element->get_name());
    }
  }
}

void XMLParser::parse(){
  parser_.parse_file(inputFile_);
  // get root element
  const auto root = parser_.get_document()->get_root_node();
  if(root->get_name() != "application") {
    throw xmlpp::exception(
        "Expected root tag 'application' instead of: " + root->get_name());
  }

  readDirectory(root, "");
}

std::string xml_parser::getRootDir(std::string inputFile){
  xmlpp::DomParser parser;
  parser.parse_file(inputFile);
  const auto root = parser.get_document()->get_root_node();
  if(root->get_name() != "application") {
    throw xmlpp::exception(
        "Expected root tag 'application' instead of: " + root->get_name());
  }
  return std::string(root->get_attribute_value("name").c_str());
}

