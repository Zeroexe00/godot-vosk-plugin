#include "HelloWorld.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void HelloWorld::_bind_methods() {
  
}

HelloWorld::HelloWorld() {
  if(Engine::get_singleton()->is_editor_hint()){
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
  }
  UtilityFunctions::print("hello world!");
}

HelloWorld::~HelloWorld() {

}

void HelloWorld::hello_world(String words){

}

void HelloWorld::_process(double delta){
  UtilityFunctions::print("hello world from process");
}