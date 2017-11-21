# Dummy server
Read variables of a ChimeraTK server and create a server that show exactly the same variables. It is not filling anything meaningful to the variables it is just showing them. This can be useful if the final ChimeraTK server is not working yet, but someone wants to prepare client applications using the ChimeraTK servers variables.

## Operation
Since the server needs to know all variables during start time it expects an input xml file called:

```
dummy_server_input.xml
```

Usally what should be done is to create a softlink in the server directory:

```
ln -s LLRFServer.xml dummy_server_input.xml
```

The corresponding mapping file (in the example above LLRFServer_mapping.xml) will be considered automatically.
In case the DOOCS adapter is used the *conf file is required. Again create a symbolic like to the original server config file:

```
ln -s LLRFServer.conf dummy_server.conf
```