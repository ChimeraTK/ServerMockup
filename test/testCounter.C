/*
 * testCounter.cc
 *
 *  Created on: Dec 4, 2017
 *      Author: zenker
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE sysTest

#include <iostream>
#include <string>

#include "DummyServer.h"
#include "ChimeraTK/ApplicationCore/TestFacility.h"
#include <mtca4u/RegisterPath.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

/**
 * Define a test app to test the XMLModule.
 * The trigger will be given by the control system to realise a
 * blocking read!
 */
struct testApp : public ChimeraTK::Application {
  testApp() : Application("test"){ }
  ~testApp() {
    shutdown();
  }

  XMLModule xml { this, "xmlModule",
      "Module publishing all variables from the input xml file" };

  ChimeraTK::ControlSystemModule cs;

  void defineConnections(){
    /**
     * This is the difference compared to DummyServer defineConnections().
     * Connect a control system variable to the xml module instead of the trigger module.
     * Now there is a blocking read in the XMLModule, which is used to step through the
     * application.
     */
    cs("trigger") >> xml.trigger;

    /**
     * Define all other connections as done in the DummyServer
     */
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
    dumpConnections();
  }
};

BOOST_AUTO_TEST_CASE( testCounter) {
  testApp app;
  app.defineConnections();
  // Add another variable in the cotrol system to read the counter
  app.xml.trigger >> app.cs("triggerCounter/");
  ChimeraTK::TestFacility tf;

  // Get the trigger variable thats blocking the application (i.e. XMLModule)
  auto writeTrigger = tf.getScalar<int>("trigger/");
  tf.runApplication();

  for(int i=0; i<5; ++i) {
    // change the trigger in order to step trough the application
    writeTrigger = 100 + i;
    writeTrigger.write();
    usleep(10000);
    tf.stepApplication();
    std::cout << tf.readScalar<int>("triggerCounter/") << std::endl;
  }
}


