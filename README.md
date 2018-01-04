# Server Mockup
Read variables of a ChimeraTK server and create a server, that shows exactly the same variables. It is not filling anything meaningful to the variables - it is just showing them. This can be useful if the final ChimeraTK server is not working yet, but someone wants to prepare client applications using the ChimeraTK server variables.

## Operation
Since the server needs to know all variables during start time it expects an input xml file called:

```
ServerMockup_input.xml
```

Usally what should be done is to create a softlink in the server directory:

```
ln -s LLRFServer.xml ServerMockup_input.xml
```

**Since it is not known a priori which input file will be used the automatic call of the xml generator is disabled in the cmake config file!**

The corresponding mapping file (in the example above LLRFServer_mapping.xml) will be considered automatically.
In case the DOOCS adapter is used the *conf file is required. Again create a symbolic like to the original server config file:

```
ln -s LLRFServer.conf ServerMockup.conf
```