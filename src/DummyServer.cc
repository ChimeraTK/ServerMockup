/*
 * DummyServer.cc
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#include "DummyServer.h"
#include "XMLParser.h"

#include <boost/thread/thread.hpp>

XMLModule::XMLModule(EntityOwner *owner, const std::string &name,
    const std::string &description, bool eliminateHierarchy,
    const std::unordered_set<std::string> &tags) :
    ctk::ApplicationModule(owner, name, description, eliminateHierarchy, tags) {

  xml_parser::XMLParser parser(INPUT);
  parser.parse();

  for(auto &var : parser.vars){
#ifdef DEBUG
    var.print();
#endif
    switch(var.type){
      case xml_parser::varType::Int32:
        if(var.dir == xml_parser::direction::IN){
          if(var.nElements > 1){
            ain_intParameter.emplace_back(std::make_pair(var, ctk::ArrayPollInput<int>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            in_intParameter.emplace_back(std::make_pair(var, ctk::ScalarPollInput<int>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        } else {
          if(var.nElements > 1){
            aout_intParameter.emplace_back(std::make_pair(var, ctk::ArrayOutput<int>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            out_intParameter.emplace_back(std::make_pair(var, ctk::ScalarOutput<int>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        }
        break;
      case xml_parser::varType::UInt32:
        if(var.dir == xml_parser::direction::IN){
          if(var.nElements > 1){
            ain_uintParameter.emplace_back(std::make_pair(var, ctk::ArrayPollInput<uint>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            in_uintParameter.emplace_back(std::make_pair(var, ctk::ScalarPollInput<uint>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        } else {
          if(var.nElements > 1){
            aout_uintParameter.emplace_back(std::make_pair(var, ctk::ArrayOutput<uint>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            out_uintParameter.emplace_back(std::make_pair(var, ctk::ScalarOutput<uint>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        }
        break;
      case xml_parser::varType::Double:
        if(var.dir == xml_parser::direction::IN){
          if(var.nElements > 1){
            ain_doubleParameter.emplace_back(std::make_pair(var, ctk::ArrayPollInput<double>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            in_doubleParameter.emplace_back(std::make_pair(var, ctk::ScalarPollInput<double>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        } else {
          if(var.nElements > 1){
            aout_doubleParameter.emplace_back(std::make_pair(var, ctk::ArrayOutput<double>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            out_doubleParameter.emplace_back(std::make_pair(var, ctk::ScalarOutput<double>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        }
        break;
      case xml_parser::varType::Float:
        if(var.dir == xml_parser::direction::IN){
          if(var.nElements > 1){
            ain_floatParameter.emplace_back(std::make_pair(var, ctk::ArrayPollInput<float>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            in_floatParameter.emplace_back(std::make_pair(var, ctk::ScalarPollInput<float>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        } else {
          if(var.nElements > 1){
            aout_floatParameter.emplace_back(std::make_pair(var, ctk::ArrayOutput<float>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            out_floatParameter.emplace_back(std::make_pair(var, ctk::ScalarOutput<float>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        }
        break;
      case xml_parser::varType::String:
        if(var.dir == xml_parser::direction::IN){
          if(var.nElements > 1){
            ain_stringParameter.emplace_back(std::make_pair(var, ctk::ArrayPollInput<std::string>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            in_stringParameter.emplace_back(std::make_pair(var, ctk::ScalarPollInput<std::string>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        } else {
          if(var.nElements > 1){
            aout_stringParameter.emplace_back(std::make_pair(var, ctk::ArrayOutput<std::string>{this, var.name, var.unit, var.nElements, var.desciption,
              { "CS" }}));
          } else {
            out_stringParameter.emplace_back(std::make_pair(var, ctk::ScalarOutput<std::string>{this, var.name, var.unit, var.desciption,
              { "CS" }}));
          }
        }
        break;
      default:
        break;
    }
  }
}

void XMLModule::mainLoop(){
  while(1){
    /**
     * \internal
     *  Setting an interruption point is included in read() methods of ChimeraTK but not in write()!
     *  Anyway here we don't call read() or write() and thus set it by hand here!
     */
    boost::this_thread::interruption_point();
    sleep(2);
  }
}


void DummyServer::defineConnections(){
  for(auto  i = xml.in_intParameter.begin(), e = xml.in_intParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.in_uintParameter.begin(), e = xml.in_uintParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.in_stringParameter.begin(), e = xml.in_stringParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.in_doubleParameter.begin(), e = xml.in_doubleParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.in_floatParameter.begin(), e = xml.in_floatParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }

  for(auto  i = xml.out_intParameter.begin(), e = xml.out_intParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.out_uintParameter.begin(), e = xml.out_uintParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.out_stringParameter.begin(), e = xml.out_stringParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.out_doubleParameter.begin(), e = xml.out_doubleParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.out_floatParameter.begin(), e = xml.out_floatParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }

  for(auto  i = xml.ain_intParameter.begin(), e = xml.ain_intParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.ain_uintParameter.begin(), e = xml.ain_uintParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.ain_stringParameter.begin(), e = xml.ain_stringParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.ain_doubleParameter.begin(), e = xml.ain_doubleParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }
  for(auto  i = xml.ain_floatParameter.begin(), e = xml.ain_floatParameter.end(); i != e; i++){
    cs[i->first.directory](i->first.name) >> i->second;
  }

  for(auto  i = xml.aout_intParameter.begin(), e = xml.aout_intParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.aout_uintParameter.begin(), e = xml.aout_uintParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.aout_stringParameter.begin(), e = xml.aout_stringParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.aout_doubleParameter.begin(), e = xml.aout_doubleParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
  for(auto  i = xml.aout_floatParameter.begin(), e = xml.aout_floatParameter.end(); i != e; i++){
    i->second >> cs[i->first.directory](i->first.name);
  }
#ifdef DEBUG
  dumpConnections();
#endif
}
