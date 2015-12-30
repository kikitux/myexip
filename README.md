# myexip
cli to resolve external ipv4 and ipv6 using myexternalip.com

## Compile

```make
make
```

```bash
cc -o myexip $(pkg-config --cflags libcurl) myexip.c $(pkg-config --libs libcurl)
```

Working compilers: gcc, clang

## Usage

```bash
$ ./myexip -h
use: ./myexip [-6]

$ ./myexip
222.111.66.11

$ ./myexip -6
2022:c227:627:0:2f:df:f1:8f
```



