/*
 * DummyServer.h
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#ifndef INCLUDE_DUMMYSERVER_H_
#define INCLUDE_DUMMYSERVER_H_

#undef GENERATE_XML
#include <ChimeraTK/ApplicationCore/ApplicationCore.h>

#define INPUT "dummy_server_input.xml"

#include <string>
#include <unordered_set>
#include "XMLParser.h"

namespace ctk = ChimeraTK;
/**
 * This module holds all control system variables read from the input xml file.
 * \attention
 * So far only the following types are supported:
 * - string
 * - int
 * - uint
 * - double
 * - float
 * If other types are used in the input xml file the module needs to be extended!
 */
struct XMLModule: public ctk::ApplicationModule {
  /**
   * Constructor
   * The default constructor is not used, since the maps are filled during construction.
   */
  XMLModule(EntityOwner *owner, const std::string &name,
      const std::string &description, bool eliminateHierarchy = false,
      const std::unordered_set<std::string> &tags = { });
  /**
   * The variable is needed to make correct connections. Name and directory information
   * is used to reconstruct the variable tree. Don't use a map, which avoids to define the
   * comparison operator of variables.
   * \remark
   * At the time of defining the connections (defineConnections()) the name of the
   * ScalarOutput is not yet available!
   */
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<std::string> > > stringParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<int> > > intParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<uint> > > uintParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<double> > > doubleParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<float> > > floatParameter;

  /**
   * Application core main loop.
   */
  void mainLoop();
};


struct DummyServer: public ctk::Application {

  DummyServer() : Application(xml_parser::getRootDir(std::string(INPUT))) {}
  ~DummyServer() {
    shutdown();
  }

  XMLModule xml { this, "xmlModule",
    "Module publishing all variables from the input xml file" };

  ctk::ControlSystemModule cs;

  void defineConnections();
};



#endif /* INCLUDE_DUMMYSERVER_H_ */
