### Introduction

*CODEBREAKER* is [*Mastermind*](http://en.wikipedia.org/wiki/Mastermind_%28board_game%29) implementation for the [*Apple-1*](http://en.wikipedia.org/wiki/Apple_I) computer.

*CODEBREAKER* has three levels of difficulty:

| Level  | Slots | Colors | Attempts | Time(s) |
|--------|-------|--------|----------|---------|
| Novice | 4     | 6      | 10       | inf.    |
| Pro    | 5     | 8      | 12       | inf.    |
| Master | 5     | 8      | 12       | 60      |

**What sets *CODEBREAKER* apart from all those *other* Apple-1 titles?**

* *A true random number generator!*  Using cycle counting while waiting for keystrokes generates a very random number.  No need for seeds and no cheating possible (unless you hack the code :-).
* *A countdown timer!*  Using the 960 KHz clock of the Apple-1, *CODEBREAKER* can countdown the *Master* level with sub-second accuracy.  If you really want to stress yourself out, play as a *Master*.  ***You've got 60 seconds or you fail!***


### Tested Platforms

* OpenEmulator 1.0.2 (<https://code.google.com/p/openemulator/>)
* M.E.S.S. v0.145 (Mar  8 2012) - Multi Emulator Super System (<http://www.mess.org/>)
* Apple-1 Mimeo Clone 4+4 configuration (tested by Corey Cohen).  *This is about as close to a real Apple-1 as you can get without a million US dollars.*


### Download

```
git clone https://github.com/datajerk/codebreaker.git
```

*or*

Download <https://github.com/datajerk/codebreaker/archive/master.zip> and extract.


### Upload and Run

If your Apple-1 has 8K of contiguous memory starting at location `$0000` then use `codebreaker.8k.mon` or `codebreaker.8k.wav`.

Load/run with `codebreaker.8k.mon`:

1.  Hand type (brutal) or cut/paste `codebreaker.8k.mon` into the Apple-1 monitor and then type `280R`.

Load/run with `codebreaker.8k.wav`:

1.  Load up `codebreaker.8k.wav` into your audio player.

2.  Type:

	```
	C100R
	280.1AFFR 
	```
	
3.  Press *play*.

4.  To run, type:

	```
	280R
	```

<br>

If your Apple-1 has 8K of memory split into 4K banks at memory locations `$0000` and `$E000` (classic Apple-1 configuration) then use `codebreaker.4+4k.mon` or `codebreaker.4+4k.wav`.

> `$E000-$EFFF` *must be writable.*

Load/run with `codebreaker.4+4k.mon`:

1.  Hand type (brutal) or cut/paste `codebreaker.4+4k.mon` into the Apple-1 monitor and then type `280R`.

Load/run with `codebreaker.4+4k.wav`:

1.  Load up `codebreaker.4+4k.wav` into your audio player.

2.  Type:

	```
	C100R
	280.BFFR E000.EFFFR
	```
	
3.  Press *play*

4.  To run type:

	```
	280R
	```


### CFFA Notes

`codebreaker.po` contains 3 binaries: `CODEBREAKER`, `CODEBREAKER.PT1`, `CODEBREAKER.PT2`.

Use `CODEBREAKER` with systems that have 8k of contiguous memory starting at `$0000`.

Use `CODEBREAKER.PT1` and `.PT2` for split 8k systems (`$0000-$0FFF`, `$E000-$EFFF`)--load `PT1` before `PT2`.

In either case type `280R` to start.


### Build Notes

*CODEBREAKER* was written in C with a bit of assembly (keyboard input, timer, and 32-bit random number seed generator).  (*CODEBREAKER* may be the *only* Apple-1 title written in C :-).  

#### Prerequisites

* To compile, you will need to build cc65 (<http://cc65.github.io/cc65/>) with the `cc65-2.13.2-apple1.patch` (included).
* c2t (<https://github.com/datajerk/c2t>) for creating `.mon` and `.wav` files to load via monitor (CLI) or cassette port.
* AppleCommander (<http://applecommander.sourceforge.net/>) for creating a CFFA `.po` diskette image.

Contiguous 8K build:

```
rm codebreaker *.o
ca65 -D CODESEG=0 -t none --listing --list-bytes 100 -o getkey.o getkey.s 
ca65 -D CODESEG=0 -t none --listing --list-bytes 100 -o putchar.o putchar.s 
cl65 -D CODESEG=0 -T -O --static-locals -t apple1 -C apple1-8k.cfg -o codebreaker codebreaker.c getkey.o putchar.o

c2t -l codebreaker codebreaker.8k.mon
c2t -1et -r 48000 codebreaker codebreaker.8k.wav
```

Non-contiguous 4+4K build:

```
rm code.1 code.2 *.o 
ca65 -D CODESEG=1 -t none --listing --list-bytes 100 -o getkey.o getkey.s 
ca65 -D CODESEG=1 -t none --listing --list-bytes 100 -o putchar.o putchar.s 
cl65 -D CODESEG=1 -T -O --static-locals -t apple1 -C apple1.cfg -o code.header codebreaker.c getkey.o putchar.o

c2t -l code.1,280 code.2,e000 codebreaker.4+4k.mon
c2t -1et -r 48000 code.1,280 code.2,e000 codebreaker.4+4k.wav

```

CFFA `codebreaker.po` build:

```
applecmd='java -jar /path/to/AppleCommander.jar'

rm codebreaker.po
$applecmd -pro140 codebreaker.po codebreaker
tail -c $(( $(stat -f "%z" codebreaker) - 4 )) codebreaker | $applecmd -p codebreaker.po codebreaker bin 0x0280 bin
$applecmd -p codebreaker.po codebreaker.pt1 bin 0xe000 bin < code.2
$applecmd -p codebreaker.po codebreaker.pt2 bin 0x0280 bin < code.1
$applecmd -ll codebreaker.po 
```


### Changelog

* Initial 1.0 release, May 13 2012
