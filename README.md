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
## Structures, definitions and enumerations

```C
struct proxy_connection {
	char* description;	// Like "http[<src>-><dst>]"
	struct proxy_side* source;
	struct proxy_side* dest;
};

enum proxy_state {
	IDLE,
	CONNECTING,
	CONNECTED,
};

enum proxy_side_type {
	SOURCE,
	DEST,
};

typedef int (*proxy_connecting_fn)(struct proxy_side* side);

struct proxy_side {
	void* proxy_side_data;
	struct proxy_side_type side;
	char* description;		// like "ipv4[1.2.3.4:3234]
	int fd;
	enum proxy_state state;
	proxy_connecting_fn connecting_callback;
};

#define TO_PROXY_SIDE(proxy_data_obj) ...
#define TO_PROXY_CONNECTION_FROM_SOURCE(proxy_source_obj) ...
#define TO_PROXY_CONNECTION_FROM_DEST(proxy_dest_obj) ...

struct proxy_connection* proxy_side_get_connection(
	struct proxy_side *side);

struct proxy_side* proxy_side_get_other_side(
	struct proxy_side *side);

struct proxy_side* proxy_data_get_side(void *data);
```

