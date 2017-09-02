DEPENDENCIES FOLDER ORGANISATION

Notation:
	- / Root
	- |-- child node
	- node written in plain texts are folders
	- node written between <> are files
	- (...) comments

/ Dependencies
	|-- Platforms (All, Windows, OSX...)
		|-- Include
			|-- Library folder (GL, SDL2, glm...)
				|-- <Header files>
			|-- <Version files>
		|-- Bin
			|-- Architecture (X86, X64...)
				|-- Debug
					|-- <Libraries compiled in debug>
				|-- Release
					|-- <Libraries compiled in release>
				|-- <3rd-party libraries only provided in release>

Remarks:
	The platform called 'All' is for libraries that will compile for each and every platforms.
	Basically, these libraries are header-only and only use standard c/c++.