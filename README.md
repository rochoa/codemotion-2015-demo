# Codemotion 2015: Node.js C++ modules demo

## Build

```shell
npm build .
```

## Demos

Blocking event loop with CPU bound loop

```shell
$ time node codemotion/1_cpu_bound.js
[ 42, 42, 42, 42 ]
Am I blocked?

real	0m4.051s
user	0m4.033s
sys	0m0.038s
```

Not blocking event loop due to setTimeout nature

```shell
$ time node codemotion/2_timeout.js
Am I blocked?
[ 42, 42, 42, 42 ]

real	0m1.043s
user	0m0.036s
sys	0m0.013s
```

Blocking event loop with C++ sleep in main thread

```shell
$ time node codemotion/3_task.js
[ 42, 42, 42, 42 ]
Am I blocked?

real	0m4.063s
user	0m0.056s
sys	0m0.060s
```

C++ callback are not enough when running on main event loop

```shell
$ time node codemotion/4_callback.js
[ 42, 42, 42, 42 ]
Am I blocked?

real	0m4.061s
user	0m0.058s
sys	0m0.059s
```

Async tasks using multiple threads, not blocking event loop

```shell
$ time node codemotion/5_async.js
Am I blocked?
[ 42, 42, 42, 42 ]

real	0m1.040s
user	0m0.034s
sys	0m0.013s
```

Threadpool size determines total time

```shell
$ export UV_THREADPOOL_SIZE=2
$ time node codemotion/5_async.js
Am I blocked?
[ 42, 42, 42, 42 ]

real	0m2.035s
user	0m0.033s
sys	0m0.013s
```

## License

Released under the [MIT License](http://www.opensource.org/licenses/MIT).
