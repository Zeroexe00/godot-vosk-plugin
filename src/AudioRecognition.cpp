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
  ClassDB::bind_method(D_METHOD("get_is_error"), &AudioRecognition::get_is_error);
  ClassDB::bind_method(D_METHOD("get_error_message"), &AudioRecognition::get_error_message);
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
  ADD_SIGNAL(MethodInfo("final_translation", PropertyInfo(Variant::STRING, "final_translation_text")));
}

AudioRecognition::AudioRecognition()
{
}

AudioRecognition::~AudioRecognition()
{
  if (model)
  {
    vosk_model_free(model);
    model = nullptr;
    UtilityFunctions::print("Vosk model freed");
  }
}

void AudioRecognition::audio_recognition(String audioFilePath)
{
  Ref<FileAccess> wavin = FileAccess::open(audioFilePath, FileAccess::READ);
  if (!wavin.is_valid())
  {
    UtilityFunctions::print("Error: file could not be opened.");
    this->is_error = true;
    this->error_message = "Error: file could not be opened.";
    this->translation = "";
    return;
  }

  Error open_err = wavin->get_open_error();
  if (open_err != OK)
  {
    UtilityFunctions::print("Error opening file: " + String::num_int64(open_err));
    this->is_error = true;
    this->error_message = "Error opening file: " + String::num_int64(open_err);
    this->translation = "";
    return;
  }

  if (wavin->get_length() <= 44)
  {
    UtilityFunctions::print("File WAV not long enough");
    this->is_error = true;
    this->error_message = "File WAV not long enough";
    this->translation = "";
    return;
  }
  wavin->seek(44); // Saltar header WAV

  if (!model)
  {
    String abs_model_path = ProjectSettings::get_singleton()->globalize_path(model_path);
    model = vosk_model_new(abs_model_path.utf8().get_data());
    if (!model)
    {
      UtilityFunctions::print("Error loading Vosk model");
      this->is_error = true;
      this->error_message = "Error loading Vosk model";
      this->translation = "";
      return;
    }
    else
    {
      UtilityFunctions::print("Vosk model loaded");
    }
  }

  VoskRecognizer *recognizer = vosk_recognizer_new(model, 1.0 * mix_rate);
  if (!recognizer)
  {
    UtilityFunctions::print("Error creating Vosk recognizer");
    this->is_error = true;
    this->error_message = "Error creating Vosk recognizer";
    this->translation = "";
    return;
  }

  std::vector<char> buf(3200);
  while (true)
  {
    int bytes_read = wavin->get_buffer(reinterpret_cast<uint8_t *>(buf.data()), buf.size());
    if (bytes_read <= 0)
      break;

    int final = vosk_recognizer_accept_waveform(recognizer, buf.data(), bytes_read);
    if (print_translation)
    {
      UtilityFunctions::print(final ? vosk_recognizer_result(recognizer) : vosk_recognizer_partial_result(recognizer));
    }
  }

  const char *recognizedText = vosk_recognizer_final_result(recognizer);
  Variant json = JSON::parse_string(String(recognizedText));
  if (json.get_type() == Variant::DICTIONARY)
  {
    Dictionary dict = json;
    this->translation = dict.get("text", "");
  }
  else
  {
    this->translation = "";
    this->is_error = true;
    this->error_message = "Error parseando JSON";
  }
  emit_signal("final_translation", this->translation);

  vosk_recognizer_free(recognizer);
  wavin->close();
}

void AudioRecognition::audio_recognition_from_bytes(const PackedByteArray audioBytes)
{
  int buf = 3200;
  const int header_offset = 44; // WAV header offset
  int offset = header_offset;
  int final;

  if (!model)
  {
    String abs_model_path = ProjectSettings::get_singleton()->globalize_path(model_path);
    model = vosk_model_new(abs_model_path.utf8().get_data());
    if (!model)
    {
      UtilityFunctions::print("Error loading Vosk model");
      this->is_error = true;
      this->error_message = "Error loading Vosk model";
      this->translation = "";
      return;
    }
    else
    {
      UtilityFunctions::print("Vosk model loaded");
    }
  }
  VoskRecognizer *recognizer = vosk_recognizer_new(model, 1.0 * mix_rate);
  if (!recognizer)
  {
    UtilityFunctions::print("Error creating Vosk recognizer");
    this->translation = "";
    this->is_error = true;
    this->error_message = "Error creating Vosk recognizer";
    vosk_model_free(model); // Clean up model
    return;
  }
  const uint8_t *data_ptr = audioBytes.ptr();
  int total_size = audioBytes.size();

  while (offset < total_size)
  {
    int bytes_to_read = MIN(buf, total_size - offset);

    int final = vosk_recognizer_accept_waveform(recognizer, reinterpret_cast<const char *>(data_ptr + offset), bytes_to_read);

    offset += bytes_to_read;

    if (print_translation)
    {
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

  const char *recognizedText = vosk_recognizer_final_result(recognizer);
  Variant parsed_variant = JSON::parse_string(String(recognizedText));
  if (parsed_variant.get_type() == Variant::DICTIONARY)
  {
    Dictionary dict = parsed_variant;
    this->translation = dict.get("text", "");
  }
  else
  {
    this->translation = "";
    this->is_error = true;
    this->error_message = "Error parseando JSON";
  }
  emit_signal("final_translation", this->translation);

  // Clean up
  vosk_recognizer_free(recognizer);
}

String AudioRecognition::get_translation_result() const
{
  return translation;
}

bool AudioRecognition::get_print_translation() const
{
  return print_translation;
}

bool AudioRecognition::get_is_error() const
{
  return is_error;
}

String AudioRecognition::get_error_message() const
{
  return error_message;
}

void AudioRecognition::set_print_translation(bool p_print_translation)
{
  this->print_translation = p_print_translation;
}

String AudioRecognition::get_model_folder_path() const
{
  return model_path;
}

void AudioRecognition::set_model_folder_path(String model_path)
{
  this->model_path = model_path;
}

int AudioRecognition::get_mix_rate() const
{
  return mix_rate;
}

void AudioRecognition::set_mix_rate(int mix_rate)
{
  this->mix_rate = mix_rate;
}
