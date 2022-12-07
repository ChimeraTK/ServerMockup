[![pipeline status](https://gitlab.hzdr.de/llrf/chimeratk-server/dummy-server/badges/master/pipeline.svg)](https://gitlab.hzdr.de/llrf/chimeratk-server/dummy-server/pipelines)

# Server Mockup
Read variables of a ChimeraTK server and create a server, that shows exactly the same variables. 
It is not filling anything meaningful to the variables - it is just showing them. 
This can be useful if the final ChimeraTK server is not working yet, but someone wants to prepare client applications using the ChimeraTK server variables.

**Remark**: One additional variable is added for technical reasons that shows up in the control system as `DummyTrigger/tick`.

## Operation
Since the server needs to know all variables during start time it expects an input xml file called:

```
server-mockup_input.xml
```

Usally what should be done is to create a softlink in the server directory:

```
ln -s LLRFServer.xml server-mockup_input.xml
```

The corresponding mapping file (in the example above LLRFServer_mapping.xml) will be considered automatically.
In case the DOOCS adapter is used the *conf file is required. Again create a symbolic like to the original server config file:

```
ln -s LLRFServer.conf server-mockup.conf
```