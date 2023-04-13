# Run Eclipse Editor with DSL
All the projects should be opened in the Eclipse IDE with the Xtext and Xtend functionalities install.

See installation instructions below:
https://www.eclipse.org/Xtext/download.html

1. Generate the artifacts using the `EnvironmentalMonitoring.xtext` file.
2. Run the Eclipse runtime IDE by right-clicking the sdu.abba.EnvironmentalMonitoring project and choose run as Eclipse Application.
3. In the new Eclipse window create a Java project. In that project create a file with the file extension of `.emconf`. Eclipse then prompts you to make this an Xtext project and you must say yes.
4. Insert this example program into the file.
```
machine MachineA
    sensor SensorA
        sampling rate 	= 5/s
        batch size 		= 100
```
5. Hit `CTRL+S` or `⌘+S` to generate the resulting JSON document. This is located in the `src-gen` folder.