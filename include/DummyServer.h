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

struct XMLModule: public ctk::ApplicationModule {
  XMLModule(EntityOwner *owner, const std::string &name,
      const std::string &description, bool eliminateHierarchy = false,
      const std::unordered_set<std::string> &tags = { });
  std::map<std::pair<std::string, std::string>, ctk::ScalarOutput<std::string> > stringParameter;
  std::map<std::pair<std::string, std::string>, ctk::ScalarOutput<int> > intParameter;
  std::map<std::pair<std::string, std::string>, ctk::ScalarOutput<uint> > uintParameter;
  std::map<std::pair<std::string, std::string>, ctk::ScalarOutput<double> > doubleParameter;
  std::map<std::pair<std::string, std::string>, ctk::ScalarOutput<float> > floatParameter;

  /**
   * Application core main loop.
   */
  void mainLoop();
};


struct DummyServer: public ctk::Application {

  DummyServer() : Application(getRootDir(std::string(INPUT))) {}
  ~DummyServer() {
    shutdown();
  }

  XMLModule xml { this, "xmlModule",
    "Module reading and publishing all variables from the input xml file" };

  ctk::ControlSystemModule cs;

  void defineConnections();
};



#endif /* INCLUDE_DUMMYSERVER_H_ */
