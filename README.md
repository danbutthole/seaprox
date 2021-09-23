# seaprox: a c based socket proxy interface
# Background
There are many async IO type frameworks, where IO for multiple
can be handled by a single thread/process. The approach for having
an all encompassing framework is not problematic in itself, since
a programmer can get much done with such a thing in a short period,
but there is a place for more "solution specific" frameworks.

Seaprox will be a light framework, which encompasses the functions
required for proxying between filedescriptors. For example, being
a socks, http, redir, and so on proxy.

The thing to notice about such proxies, there is a lot of commonality
between the types, for example they all:
 * have an in fd and an out fd
 * they can all do with some method for 'queuing'
 * they all have some amount of 'setup' and connection startup
 * they setup can be abstracted away easily in c because c is great

# Seaprox specialities
## Interproxy communications
One extension will be made, to allow a seaprox-to-seaprox connection
which can multiplex other proxy connections. This would allow
something like:

```
http-----seaprox===(seaprox-proto)===seaprox-----conn 1
socks---/                                   \----conn 2
```

For 'simplicity' sake, this seaprox-proto will be sctp based and make
use of demarcated messages in the sctp stream. Space for development
of a UDP userspace based seaprox-proto-udp will be provided to allow
for the most efficient implementation.

## Extendinding seaprox
The base proxying technique (something like ncat) will be provided,
and then the extended proxy techniques will be left for implemenation
by the reader (actually the same author). But this will be provided
for by allowing wrapping/linking around/into this library by other
higher level languages (or still C). This allows the proxy proto
specific abstractions to be handled separately in smaller or problem
specific code bases.

# Design
## Structs

