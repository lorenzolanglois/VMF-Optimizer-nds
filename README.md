# VMF Optimizer
Program to compress Valve Map Files (VMF) to make them optimized enough for nds roms. Supposedly compatible with any Source Engine game.

Primilary useful for my other project [Stanley-DS](https://github.com/lorenzolanglois/Stanley-DS).

## How it works
The program will remove parameters that have default values, as well as useless characters like spaces, tabulations and new line characters.

This process is useful and almost necessary for .vmf map files used on the Nintendo DS, since the memory is (very) limited.

## Instruction
1. Launch *vmfoptimizer.exe* (on Windows) or *vmfoptimizer* (on Linux)
2. Drop your .vmf file into the command window or type its name manually
3. After several seconds, the optimized vmf will be created in the same directory as the original file

## Arguments
>[file] ([file] [file]...)

Use file as argument(s) rather than typing the filename during launch time.
<br>

>-nonds

Keep properties normally removed for NDS. This will ensure that the map file is compatible with Hammer. (Similar to how original VMF Optimizer works)
<br>

>-keep

Keep useless characters including spaces, tabulations and new lines. Mostly useful for debugging and to make the VMF file more readable to humans.
<br>

>-nolog

Disable log file.
<br>

>-logfile [filename]

Change the name of the log file. This has no effet when used with *-nolog*.

## Credits
Fork from the project [VMF-Optimizer](https://github.com/dabmasterars/VMF-Optimizer) made by [dabmasterars](https://github.com/dabmasterars)
