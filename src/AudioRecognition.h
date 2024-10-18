#ifndef AUDIO_RECOGNITION_H
#define AUDIO_RECOGNITION_H


#include <godot_cpp/classes/node.hpp>


namespace godot {
  
  class AudioRecognition: public Node {
    GDCLASS(AudioRecognition, Node)

    private:
      String translation;
      bool print_translation = false;
      String model_path;
      int mix_rate = 48000;


    protected:
      static void _bind_methods();

    public:
      AudioRecognition();
      ~AudioRecognition();

      String get_translation_result() const;

      bool get_print_translation() const;
      void set_print_translation(const bool p_print_translation);

      String get_model_folder_path() const;
      void set_model_folder_path(String model_path);

      int get_mix_rate() const;
      void set_mix_rate(int mix_rate);

      void audio_recognition(String audioFile);
      void audio_recognition_from_bytes(const PackedByteArray audioBytes);

  };
}

#endif