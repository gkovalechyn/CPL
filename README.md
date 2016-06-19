#CPL

####Summary
This was supposed to be a library that would decrypt files from an encrypted jar file. It was designed to be used with
the Bukkit minecraft server where a plugin loader would load this library and request the classes to be decrypted. All 
without having to depend on exernal libraries since C++ filled with huge libraries that I only need 1 thing of and of
course I have to compile the library myself which is just fucking amazing...

####Functionality
- Contains a simple UDP client used for connecting to the server that would make sure that this, the plugin loader and 
the plugin to be loaded haven't been modified.
- Shit code, because why not.
- SHA-256 hash function. [http://www.zedwood.com/article/cpp-sha256-function](http://www.zedwood.com/article/cpp-sha256-function)
- AES encryption/decryption. I can't remember right now where I got it from, but there's a link on the file itself.

####Errors
- For some reason it doesn't work with Java's AES/CBC/PKCS5Padding encryption.

####Compiler stuff
- IDE used: NetBeans
- Compiler used: TDM-MinGW64

#####x86_64
Don't forget to add "-Dx86_64" to the preprocessor options.

- gcc compiler flags: -m64 -static-libgcc -static-libstdc++ -Wl,--add-stdcall-alias -shared
- g++ compiler flags: -m64 -static-libgcc -static-libstdc++ -Wl,--add-stdcall-alias -shared

#####x86
- gcc compiler flags: -static-libgcc -static-libstdc++ -Wl,--add-stdcall-alias -shared
- g++ compiler flags: -static-libgcc -static-libstdc++ -Wl,--add-stdcall-alias -shared

####Link to the other projects:
[Link](https://github.com/gkovalechyn/CPL2)