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
typedef int (*epoll_handler_fn)(struct proxy_side* side
	uint32_t events);

struct proxy_side {
	void* proxy_side_data;
	struct proxy_side_type side;
	char* description;		// like "ipv4[1.2.3.4:3234]
	int fd;
	enum proxy_state state;
	proxy_connecting_fn connecting_callback;
	epoll_handler_fn epoll_callback;
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

## epoll handling
Epoll handling will use a list (or array) of `struct proxy_connection`
objects, and perform a basic search for the file descriptor in the
`source` and `dest` objects. The `source` and `dest` objects (which
are `struct proxy_side` objects) include a `epoll_callback` callback
for handling the events.

## Connection limits
It is foreseen that an invocation of the proxy (of what is provided
or by a wrapper/implementation) that there will simply be a number of
simple limits. These limits will be:
 * `MAX_NUM_CONNECTIONS`
 * `NUM_THREADS`

With these pre-defined at invocation time (or compile time), the
approach will be to simply allocate the structures as arrays. Then
lookups will simply be linear lookups.

## Built-in proxy type: static
For distribution, a single proxy type will be implemented and
available for usage. It is a so-described-and-named `static` proxy.
That is, it listens for connections on a tcp4 ip/port, and on
accepting, connects out to a tcp name/port. On connecting out, it
will prefer ipv6 by default, though will be configurable to prefer
ipv4 instead.

## Optionally asynchronous connections

```
accept -> proto stuff -> |connect| -> connected

connect:
 - setup dest fd
 - setup proxy connection struct
 - call seaproxy_connect(eventfd, source_fd, dest_fd)

seaproxy_connect
 - in program
   - seaproxy_socket_connect
 - with external callback
   - seaproxy_external_connect
     - this is done to allow name resolution and connect to be done
       in seperate thread to allow for maximum concurrency
     - call external callback
     - external callback code notifies with appropriate result
 - either
   - notify eventfd with (dest_fd * NUM_CONNECTION_FAIL_REASONS) if
     successful, and +CONNECTION_FAIL_REASON if failed

seaproxy_socket_connect:
 - name resolution etc
 - save and override dest_fd handler callback function
 - make fd non-block
 - do socket connect
 - wait for writable event
   - notify eventfd of destfd
 - de-override dest_fd handler callback function

CONNECTION_FAIL_REASON's:
 - name resolution failure
 - connection refused
 - other
```
