#include "register_types.h"
#include "AudioRecognition.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_audio_recognition(ModuleInitializationLevel p_level){
  if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  ClassDB::register_class<AudioRecognition>();
}

void uninitialize_audio_recognition(ModuleInitializationLevel p_level){
  if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }
}

extern "C"{
  GDExtensionBool GDE_EXPORT audio_recognition_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_audio_recognition);
    init_obj.register_terminator(uninitialize_audio_recognition);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
    
    return init_obj.init();
  }
}