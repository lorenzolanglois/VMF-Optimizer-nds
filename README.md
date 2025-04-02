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
>-normal

Use this argument to not remove properties useless on DS. This will make the map file compatible with Hammer.

## Credits
Fork from the project [VMF-Optimizer](https://github.com/dabmasterars/VMF-Optimizer) made by [dabmasterars](https://github.com/dabmasterars)
