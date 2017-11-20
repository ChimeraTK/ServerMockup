/*
 * DummyServer.cc
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#include "DummyServer.h"
#include "XMLParser.h"

XMLModule::XMLModule(EntityOwner *owner, const std::string &name,
    const std::string &description, bool eliminateHierarchy,
    const std::unordered_set<std::string> &tags) :
    ctk::ApplicationModule(owner, name, description, eliminateHierarchy, tags) {

  xml_parser::XMLParser parser(INPUT);
  parser.parse();

  for(auto &var : parser.vars){
    var.print();
    if(var.type == xml_parser::varType::Int32){
      intParameter[make_pair(var.directory, var.name)].replace(ctk::ScalarOutput<int>{this, var.name, var.unit, var.desciption,
                      { "CS" }});
    } else if (var.type == xml_parser::varType::UInt32){
      uintParameter[make_pair(var.directory, var.name)].replace(ctk::ScalarOutput<uint>{this, var.name, var.unit, var.desciption,
                                  { "CS" }});
    } else if (var.type == xml_parser::varType::Double){
      doubleParameter[make_pair(var.directory, var.name)].replace(ctk::ScalarOutput<double>{this, var.name, var.unit, var.desciption,
                            { "CS" }});
    } else if (var.type == xml_parser::varType::Float){
      floatParameter[make_pair(var.directory, var.name)].replace(ctk::ScalarOutput<float>{this, var.name, var.unit, var.desciption,
                                  { "CS" }});
    } else if (var.type == xml_parser::varType::String){
      stringParameter[make_pair(var.directory, var.name)].replace(ctk::ScalarOutput<std::string>{this, var.name, var.unit, var.desciption,
                                  { "CS" }});
    }
  }


}

void XMLModule::mainLoop(){
  while(1){
    sleep(0.5);
  }
}


void DummyServer::defineConnections(){
  for(auto  i = xml.intParameter.begin(), e = xml.intParameter.end(); i != e; i++){
    std::cout << "Adding: " << i->first.first << "\t" << i->first.second << std::endl;
    i->second >> cs[i->first.first](i->first.second);
  }
  for(auto  i = xml.uintParameter.begin(), e = xml.uintParameter.end(); i != e; i++){
    std::cout << "Adding: " << i->first.first << "\t" << i->first.second << std::endl;
    i->second >> cs[i->first.first](i->first.second);
  }
  for(auto  i = xml.stringParameter.begin(), e = xml.stringParameter.end(); i != e; i++){
    std::cout << "Adding: " << i->first.first << "\t" << i->first.second << std::endl;
    i->second >> cs[i->first.first](i->first.second);
  }
  for(auto  i = xml.doubleParameter.begin(), e = xml.doubleParameter.end(); i != e; i++){
    std::cout << "Adding: " << i->first.first << "\t" << i->first.second << std::endl;
    i->second >> cs[i->first.first](i->first.second);
  }
  for(auto  i = xml.floatParameter.begin(), e = xml.floatParameter.end(); i != e; i++){
    std::cout << "Adding: " << i->first.first << "\t" << i->first.second << std::endl;
    i->second >> cs[i->first.first](i->first.second);
  }

  dumpConnections();
}
