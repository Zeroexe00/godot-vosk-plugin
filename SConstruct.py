#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

#For reference:
#- CCFLAGS are compilation flags shared between C and C++
#- CFLAGS are for C-specific compilation flags
#- CXXFLAGS are for C++-specific compilation flags
#- CPPFLAGS are for pre-processor flags
#- CPPDEFINES are for pre-processor defines
#- LINKFLAGS are for linking flags

#tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
env.Append(LIBPATH=['./src'])
#scons removes libprefix in windows
env.Append(LIBS=['liblibvosk'])
# env.Append(LINKFLAGS=['-llibvosk'])
sources = Glob("src/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demoproject/bin/AudioRecognition.{}.{}.framework/helloWorld.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "x11":
    library = env.SharedLibrary(
        "demoproject/bin/ubuntu-20-04/AudioRecognition{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demoproject/bin/windows/AudioRecognition{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)