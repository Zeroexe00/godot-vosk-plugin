#include "AudioRecognition.h"
#include <vosk_api.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/json.hpp>

using namespace godot;

void AudioRecognition::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_print_translation"), &AudioRecognition::get_print_translation);
	ClassDB::bind_method(D_METHOD("set_print_translation", "p_print_translation"), &AudioRecognition::set_print_translation);
  ClassDB::bind_method(D_METHOD("get_model_folder_path"), &AudioRecognition::get_model_folder_path);
	ClassDB::bind_method(D_METHOD("set_model_folder_path", "model_path"), &AudioRecognition::set_model_folder_path);
  ClassDB::bind_method(D_METHOD("get_mix_rate"), &AudioRecognition::get_mix_rate);
	ClassDB::bind_method(D_METHOD("set_mix_rate", "mix_rate"), &AudioRecognition::set_mix_rate);
  ClassDB::bind_method(D_METHOD("get_translation_result"), &AudioRecognition::get_translation_result);
  ClassDB::bind_method(D_METHOD("audio_recognition", "audioFile"), &AudioRecognition::audio_recognition);
  ClassDB::bind_method(D_METHOD("audio_recognition_from_bytes", "audioFile"), &AudioRecognition::audio_recognition_from_bytes);

  ClassDB::add_property("AudioRecognition", PropertyInfo(Variant::STRING, "translation"), "", "get_translation_result");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "mix_rate"), "set_mix_rate", "get_mix_rate");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "model_path"), "set_model_folder_path", "get_model_folder_path");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "print_translation"), "set_print_translation", "get_print_translation");
}

AudioRecognition::AudioRecognition()
{
  
}

AudioRecognition::~AudioRecognition()
{
}


void AudioRecognition::audio_recognition(String audioFile)
{
  Ref<FileAccess> wavin = FileAccess::open(audioFile, FileAccess::READ);
  
  Error open_err = wavin->get_open_error();
  if (open_err != OK)
  {
    std::cerr << "Error opening file!\n";
    this->translation = String::num_int64(int(open_err));
    return;
  }

  std::vector<char> buf(3200); // Buffer for reading data
  int final;

  String abs_model_path = ProjectSettings::get_singleton()->globalize_path(model_path);
  VoskModel *model = vosk_model_new(abs_model_path.utf8().get_data());
  if (!model)
  {
    UtilityFunctions::print("Error loading Vosk model!");
    this->translation = "Error loading Vosk model!";
    return;
  }
  VoskRecognizer *recognizer = vosk_recognizer_new(model, 1.0 * mix_rate);
  if (!recognizer)
  {
    UtilityFunctions::print("Error creating Vosk recognizer!");
    this->translation = "Error creating Vosk recognizer!";
    vosk_model_free(model); // Clean up model
    return;
  }
  wavin->seek(44); // Skip the WAV header

  while (true)
  {
    int bytes_read = wavin->get_buffer(reinterpret_cast<uint8_t *>(buf.data()), buf.size());
    if (bytes_read <= 0)
    {
      break;
    }

    int final = vosk_recognizer_accept_waveform(recognizer, buf.data(), bytes_read);
    if(print_translation) {
      if (final)
      {
        UtilityFunctions::print(vosk_recognizer_result(recognizer));
      }
      else
      {
        UtilityFunctions::print(vosk_recognizer_partial_result(recognizer));
      }
    }
  }
  
  this->translation = vosk_recognizer_final_result(recognizer);

  // Clean up
  vosk_recognizer_free(recognizer);
  vosk_model_free(model);
  wavin->close();
}

void AudioRecognition::audio_recognition_from_bytes(const PackedByteArray audioBytes)
{
  int buf = 3200;
  const int header_offset = 44; // WAV header offset
  int offset = header_offset;
  int final;

  String abs_model_path = ProjectSettings::get_singleton()->globalize_path(model_path);
  VoskModel *model = vosk_model_new(abs_model_path.utf8().get_data());
  if (!model)
  {
    UtilityFunctions::print("Error loading Vosk model!");
    this->translation = "Error loading Vosk model!";
    return;
  }
  VoskRecognizer *recognizer = vosk_recognizer_new(model, 1.0 * mix_rate);
  if (!recognizer)
  {
    UtilityFunctions::print("Error creating Vosk recognizer!");
    this->translation = "Error creating Vosk recognizer!";
    vosk_model_free(model); // Clean up model
    return;
  }

  while (offset < audioBytes.size())
  {
    int bytes_to_read = MIN(buf, audioBytes.size() - offset);

    PackedByteArray bytes_read = audioBytes.slice(offset, offset + bytes_to_read);
    
    offset += bytes_to_read;
    
    int final = vosk_recognizer_accept_waveform(recognizer, reinterpret_cast<const char *>(bytes_read.ptr()), bytes_to_read);
    if(print_translation) {
      if (final)
      {
        UtilityFunctions::print(vosk_recognizer_result(recognizer));
      }
      else
      {
        UtilityFunctions::print(vosk_recognizer_partial_result(recognizer));
      }
    }
  }
  
  this->translation = vosk_recognizer_final_result(recognizer);

  // Clean up
  vosk_recognizer_free(recognizer);
  vosk_model_free(model);
}

String AudioRecognition::get_translation_result() const
{
  return translation;
}

bool AudioRecognition::get_print_translation() const {
  return print_translation;
}

void AudioRecognition::set_print_translation(bool p_print_translation) {
  this->print_translation = p_print_translation;
}

String AudioRecognition::get_model_folder_path() const {
  return model_path;
}

void AudioRecognition::set_model_folder_path(String model_path) {
  this->model_path = model_path;
}

int AudioRecognition::get_mix_rate() const {
  return mix_rate;
}

void AudioRecognition::set_mix_rate(int mix_rate) {
  this->mix_rate = mix_rate;
}