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
sources = Glob("src/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

if env["platform"] == "macos":
    env.Append(LIBPATH=['./src/macos'])
    env.Append(LIBS=['vosk'])
    library = env.SharedLibrary(
        "demo_project/bin/AudioRecognition.{}.{}.framework/helloWorld.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "windows":
    env.Append(LIBPATH=['./src/windows'])
    #scons removes libprefix in windows
    env.Append(LIBS=['libvosk'])
    library = env.SharedLibrary(
            "demo_project/bin/windows/AudioRecognition{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
            source=sources,
        )
elif env["platform"] == "linux":
    env.Append(LIBPATH=['./src/linux'])
    env.Append(LIBS=['vosk'])
    library = env.SharedLibrary(
        "demo_project/bin/linux/AudioRecognition{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )
elif env["platform"] == "android":
    env.Append(LIBPATH=['./src/android'])
    env.Append(LIBS=['vosk'])
    library = env.SharedLibrary(
        "demo_project/bin/android/AudioRecognition{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )
else:
    env.Append(LIBPATH=['./src/linux'])
    env.Append(LIBS=['vosk'])
    library = env.SharedLibrary(
        "demo_project/bin/AudioRecognition{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )



Default(library)
