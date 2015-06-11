#Stockfish Interface Library

The purpose of this library is to provide an interface into Stockfish that will allow for an iOS app to use Stockfish in much the same manner as a desktop app would and without needing to change a single line of Stockfish code in the process.

To accomplish this task we do 3 things.
1) Take advantage of the fact that Stockfish adheres to the UCI protocol. 
	a) See: http://support.stockfishchess.org/kb/advanced-topics/uci-protocol
2) Communicate with Stockfish by injecting commands into stdin (Stockfish on the commandline listens on stdin).
3) Receive Stockfish output by intercepting stdout buffer and interpreting it and passing this on to the GUI.

###Quick Overview on Using this Library:

1. Download the stockfish source from: https://stockfishchess.org/download/
2. Extract the zip into the src directory of this project
3. Add the files into the Xcode project. 
4. Change each of the .cpp file types to 'Objective C++ Source'
5. Include this project in your Xcode workspace


