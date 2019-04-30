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
    << "\t Desc: " << description
    << "\t unit: " << unit
    << "\t type: " << type
    << "\t number of elements: " << nElements << std::endl;
  } else {
    std::cout << directory << "/" << name << " (read only)"
    << "\t Directory: " << directory
    << "\t Desc: " << description
    << "\t unit: " << unit
    << "\t type: " << type
    << "\t number of elements: " << nElements << std::endl;
  }
}

variable xml_parser::analyseNode(const xmlpp::Element &element){
  variable var;
  var.name = element.get_attribute_value("name").c_str();
  for(const auto& child : element.get_children()){
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
        var.description = node->get_child_text()->get_content().c_str();
      } catch (xmlpp::exception &e){
        std::cerr << "While reading description data:" << e.what()
                << std::endl;
        var.description = "";
      }
    } else if (child->get_name() == "unit"){
      try{
        var.unit = node->get_child_text()->get_content().c_str();
      } catch (xmlpp::exception &e){
        std::cerr << "While reading unit data:" << e.what()
                << std::endl;
        var.unit = "";
      }
    } else if (child->get_name() == "value_type"){
      //\ToDo: Get rid of this and use SupportedUserTypes for doing that in a generic way
      if(std::string(node->get_child_text()->get_content().c_str()).compare("string") == 0){
        var.type = ctk::DataType::string;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("int8") == 0) {
        var.type = ctk::DataType::int8;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("uint8") == 0) {
        var.type = ctk::DataType::uint8;;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("int16") == 0) {
        var.type = ctk::DataType::int16;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("uint16") == 0) {
        var.type = ctk::DataType::uint16;;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("int32") == 0) {
        var.type = ctk::DataType::int32;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("uint32") == 0) {
        var.type = ctk::DataType::uint32;;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("int64") == 0) {
        var.type = ctk::DataType::int64;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("uint64") == 0) {
        var.type = ctk::DataType::uint64;;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("double") == 0) {
        var.type = ctk::DataType::float64;
      } else if (std::string(node->get_child_text()->get_content().c_str()).compare("float") == 0) {
        var.type = ctk::DataType::float32;
      } else {
        std::cerr << "Read unknown type: " << node->get_child_text()->get_content().c_str() << std::endl;
      }
    } else if (child->get_name() == "direction"){
      std::string type =  node->get_child_text()->get_content().c_str();
      if(type.compare("application_to_control_system") == 0){
        var.dir = xml_parser::direction::OUT;
      } else if (type.compare("control_system_to_application") == 0){
        var.dir = xml_parser::direction::IN;
      } else {
        throw xmlpp::exception("Unknown data flow direction: " + type);
      }
    } else if (child->get_name() == "numberOfElements"){
      var.nElements = atoi(node->get_child_text()->get_content().c_str());
    }
  }
  return var;
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

void xml_parser::addElement(const xmlpp::Element &element, ctk::Module* owner,
      ctk::TemplateUserTypeMap<OutputList>* l1,
      ctk::TemplateUserTypeMap<OutputArrayList>* l2,
      ctk::TemplateUserTypeMap<InputList>* l3,
      ctk::TemplateUserTypeMap<InputArrayList>* l4){
  xml_parser::variable tmp = xml_parser::analyseNode(element);
#ifdef DEBUG
  std::cout << owner->getName() << "::Adding variable to top module: " << tmp.name << std::endl;
  tmp.print();
#endif
  if(tmp.nElements>1){
    if(tmp.dir == xml_parser::direction::OUT){
      auto myLambda = [&](auto arg) {
        auto& accessorList = boost::fusion::at_key<decltype(arg)>(l2->table);
        accessorList.emplace_back(ctk::ArrayOutput<decltype(arg)>{owner, tmp.name, "", tmp.nElements, tmp.description, {"CS"}});
      };
      ctk::callForType(tmp.type, myLambda);
    } else {
      auto myLambda = [&](auto arg) {
        auto& accessorList = boost::fusion::at_key<decltype(arg)>(l4->table);
        accessorList.emplace_back(ctk::ArrayPollInput<decltype(arg)>{owner, tmp.name, "", tmp.nElements, tmp.description, {"CS"}});
      };
      ctk::callForType(tmp.type, myLambda);
    }
  } else {
    if(tmp.dir == xml_parser::direction::OUT){
      auto myLambda = [&](auto arg) {
        auto& accessorList = boost::fusion::at_key<decltype(arg)>(l1->table);
        accessorList.emplace_back(ctk::ScalarOutput<decltype(arg)>{owner, tmp.name, "", tmp.description, {"CS"}});
      };
      ctk::callForType(tmp.type, myLambda);
    } else {
      auto myLambda = [&](auto arg) {
        auto& accessorList = boost::fusion::at_key<decltype(arg)>(l3->table);
        accessorList.emplace_back(ctk::ScalarPollInput<decltype(arg)>{owner, tmp.name, "", tmp.description, {"CS"}});
      };
      ctk::callForType(tmp.type, myLambda);
    }
  }
}

