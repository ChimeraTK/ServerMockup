/*
 * test_parsing.cc
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE sysTest

#include "XMLParser.h"

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

using namespace std;

BOOST_AUTO_TEST_CASE( testXMLParsing){
  XMLParser parser("dummy_server_input.xml");
  parser.parse();
  for(auto &var: parser.vars){
    var.print();
  }
}

