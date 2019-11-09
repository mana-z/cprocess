# cprocess

A convenience wrapper around POSIX process creation API.

## Why?

It is a very frequent pattern among the C/C++ developers to overuse standard C `system()` and POSIX
`popen()` when it is not optimal, unnecessary and, often, even harmful. This little library (only 2
functions, with one of it being just for extra convenience) tries to offer an alternative having the
same convenience of `system()` and `popen()` yet having the strength and flexibility of
`fork()+exec()`.  

## What is wrong with `system()` and `popen()`?

These methods are spawning a subshell, so, in fact, when you do this:

```C
system("/my/binary");
```

you are getting process tree looking like this:

    /parent/process
    └──/bin/sh
       └──/my/binary

So there are performance implications of spawning an unnecessary intermediate process and parsing a
shell line which does not really need to be parsed.

Also, `system()` does not offer any form of interaction with the child outside the exit status and
any form of asynchronous work with the subprocess. The `popen()` call helps this a bit by allowing a
`FILE*` access to the process. This, however, only allows an uni-directional communication and does
not help with other bad traits of `system()`.

In both cases there are even some security implications about which you may read about in man of
`system()`. This makes them non-ideal in terms of performance, flexibility and security.

## And what is wrong with using `fork()+exec()` directly?

Nothing in essence. However, their handling takes some care and mistakes done using them are pretty
common, especially when IO redirection is needed. Many developers are prone to avoid them for that
reasons.

## What I need?

A Unix-like system and a C99 compiler.

In theory, it may be ported to Windows using their process and pipes API, but I'm not that familiar
with it nor I have any use for it, so it is up to your pull requests :-)

...and if you need to target a Unix-like system and have only an ANSI C compiler, please do yourself
a favor and get a better compiler. I find the unnecessary forcing of old standards
counter-productive and silly.

## OK, so how? (Or, finally the examples for the hungry)

Include the `process.h` header and add `process.c` to your build environment. You do not need any
other tweaks aside of having a newer C standard enabled.

for a synchronous spawn without IO redirection (much like `system()`) use like this:

```C
int retval = process_run((char* const[]){ "/my/binary", "arg1", "arg2", ... , 0 }, false);
```

The additional boolean arguments specifies if you want to run the process with clean environment
(true) or reuse the parents' (false).

For an asynchronous spawn without IO redirection , do this. The second argument is pretty much the
same:

```C
pid_t pid = proces_spawn((char* const[]){ "/my/binary", "arg1", "arg2", ..., 0 }, false, 0, 0);
```

You can wait for process finish using POSIX standard `waitpid()` call.

The remaining parameters are for IO redirection as shown here:

```C

int stdinPipe = 0;
int stdoutPipe = 0;
pid_t pid = proces_spawn((char* const[]){ "/my/binary", "arg1", "arg2", ..., 0 }, false,
    &stdinPipe, &stdoutPipe);
```

You can then use the received file descriptiors using POSIX `read()` and `write()` calls. And don't
forget to `close()` the descriptors after use!

You may use none, one of, or both pipes. Just pass the pointer if you want it or null if you don't.
If you don't use them, the subprocess will share stdin and/or stdout with the parent process.


## Ok, why, again, bother with subprocesses in general?

There are many reasons why use subprocesses, even when they are getting bit forgotten nowadays. Here
is a few:
- You may reuse lots of tools in environment
    - this is particulary strong in Linux world as there are solutions for lots of problems through
      simple CLI tools
    - You may minimize reinventing the wheel, which is a super-common pattern in C and C++ world
- You may easily make your program asynchronous
    - subprocesses are usually much safer than threads

## Can I use it in C++?

Sure! Just remember you still have to interface pipes using POSIX IO functions (`read()`, `write()`,
...).

## Can I use it in [insert other language]?

Probably, but make sure your language does not have subprocess handling in it's standard library. It
will most probably have a much better one already.

## License

This library is licensed under CC0 license, which is effectively public domain. You may use it and
modify it however you want and for whatever you want, be it hobby, commercial, world domination or
anything else. Just don't sue me if you accidentally blow up your mothership with it. No shield may
save you aganst a well placed fork bomb!

See COPYING if you need all the lawyer stuff.

