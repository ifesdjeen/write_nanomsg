# Write Nanomsg 

Write Nanomsg is a small and simple plugin that sends collectd messages
to the Nanomsg endpoint.

# Usage

Specify write_nanomsg plugin in your `collectd.conf` file:

```
LoadPlugin write_nanomsg
```

Add a configuration section for the listening nanomsg node:

```
<Plugin write_nanomsg>
 <Server "example">
    Uri "tcp://127.0.0.1:5555"
 </Server>
</Plugin>
```

# Building

```
autoreconf --install
./configure --prefix /path/to/your_collectd
make
make install
```

# License

Licensed under the MIT License. 

Copyright (c) 2014 Oleksandr Petrov

