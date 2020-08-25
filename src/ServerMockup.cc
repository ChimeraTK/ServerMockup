/*
 * DummyServer.cc
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#include "ServerMockup.h"
#include "version.h"

void TemplateGroup::addElement(const xmlpp::Element* element){
  for(const auto& child : element->get_children()){
    const xmlpp::Element *child_element = dynamic_cast<const xmlpp::Element*>(child);
    if(!child_element)
      continue;
    if(child_element->get_name() == "directory") {
#ifdef DEBUG
      std::cout << this->getName() << "::Adding subdirectory: " << child_element->get_attribute_value("name").c_str() << std::endl;
#endif
      groups.emplace_back(TemplateGroup{this,child_element->get_attribute_value("name").c_str(),""});
      groups.back().addElement(child_element);
    } else if (child_element->get_name() == "variable"){
      xml_parser::addElement( *child_element, this, &_outputAccessorListMap, &_outputArrayAccessorListMap, &_inputAccessorListMap, &_inputArrayAccessorListMap);
    } else {
      throw xmlpp::exception(
                "Found unknown xml element: " + child_element->get_name());
    }
  }
}

void TemplateModule::addElement(const xmlpp::Element* element){
  for(const auto& child : element->get_children()){
    const xmlpp::Element *child_element = dynamic_cast<const xmlpp::Element*>(child);
    if(!child_element)
      continue;
    if(child_element->get_name() == "directory") {
#ifdef DEBUG
      std::cout << this->getName() << "::Adding subdirectory to top module: " << child_element->get_attribute_value("name").c_str() << std::endl;
#endif
      groups.emplace_back(TemplateGroup{this,child_element->get_attribute_value("name").c_str(),""});
      groups.back().addElement(child_element);
    } else if (child_element->get_name() == "variable"){
      xml_parser::addElement( *child_element, this, &_outputAccessorListMap, &_outputArrayAccessorListMap, &_inputAccessorListMap, &_inputArrayAccessorListMap);
    } else {
      throw xmlpp::exception(
                "Found unknown xml element: " + child_element->get_name());
    }
  }
}

ServerMockup::ServerMockup() : Application(xml_parser::getRootDir(std::string(INPUT))){
  xmlpp::DomParser parser;
  parser.parse_file(INPUT);
  // get root element
  const auto rootElement = parser.get_document()->get_root_node();
  if(rootElement->get_name() != "application") {
    throw xmlpp::exception(
        "Expected root tag 'application' instead of: " + rootElement->get_name());
  }
  for(const auto& child : rootElement->get_children()){
    const xmlpp::Element *child_element = dynamic_cast<const xmlpp::Element*>(child);
    if(!child_element)
      continue;
    if(child_element->get_name() == "directory") {
#ifdef DEBUG
      std::cout << "Adding directory: " << child_element->get_attribute_value("name").c_str() << std::endl;
#endif
      modules.emplace_back(TemplateModule{this,child_element->get_attribute_value("name").c_str(),""});
      modules.back().addElement(child_element);
    }
  }
}


void ServerMockup::defineConnections(){
  for(auto it = modules.begin(); it != modules.end(); it++){
    trigger.tick >> it->trigger;
    it->findTag("CS").connectTo(cs[it->getName()]);
  }
  cs["configuration"]("UpdateTime") >> trigger.period;
  ctk::VariableNetworkNode::makeConstant(true, AppVersion::major, 1) >> cs["Server"]["Version"]("major");
  ctk::VariableNetworkNode::makeConstant(true, AppVersion::minor, 1) >> cs["Server"]["Version"]("minor");
  ctk::VariableNetworkNode::makeConstant(true, AppVersion::patch, 1) >> cs["Server"]["Version"]("patch");
}
