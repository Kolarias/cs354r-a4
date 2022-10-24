**Build Process:**
1) Clone the repository with: 
```
Graders:
$ git clone --recurse-submodules -j8 -b codefreeze https://github.com/Kolarias/cs354r-a4.git
Everyone else:
$ git clone --recurse-submodules -j8 https://github.com/Kolarias/cs354r-a4.git
```
This clones my repository and the submodule godot-cpp and that submodule's submodule godot-headers. For graders, you clone specifically the codefreeze branch.

2) Run: 
```
$ cd cs354r-a4/Assignment4/godot-cpp
```
3) From within godot-cpp, we will need to generate the godot-cpp bindings. Run:
```
$ python3 /usr/bin/scons platform=linux generate_bindings=yes -j4 use_custom_api_file=yes custom_api_file=../api.json
```
If you have the scons alias, replace 'python 3 /usr/bin/scons' with just 'scons'. Replace 'linux' with whatever your platform is. This step will take a while; it generates the godot-cpp bindings necessary for GDNative. You can also copy in your pre-generated godot-cpp bindings if you have them. 

4) Run:
```
$ cd ..
$ python3 /usr/bin/scons platform=linux
```
Again, replace 'linux' if needed. Again, use the alias if you have it.

5) Import Assignment4/Project/project.godot into Godot and you can run the project!
