extends Node2D

@onready var audioStreamPlayer: AudioStreamPlayer = $AudioStreamPlayer
@onready var button: Button = $Button2

var microphoneRecord: AudioEffectRecord
var recordedSample: AudioStreamWAV


func _ready() -> void:
	if OS.get_name() == "Android":
		$AudioRecognition.model_path = "user://model/"
	microphoneRecord = AudioServer.get_bus_effect(AudioServer.get_bus_index("Record"),0) as AudioEffectRecord


func _on_button_pressed() -> void:
	if not OS.has_feature("editor") and OS.get_name() == "Android":
		OS.request_permissions()
	#copy_folder_to_user("res://model/", "user://model/")
	var test = preload("res://test_comando.wav")
	var a = test.save_to_wav("user://test_comando.wav")
	print('resultados:', a)
	if FileAccess.file_exists("user://test_comando.wav"):
		print('existe')
	else:
		print("no existe")
	$AudioRecognition.mix_rate = test.mix_rate
	#$AudioRecognition.model_path = "user://model"
	$AudioRecognition.audio_recognition("res://test_comando.wav")
	print('translation: ', $AudioRecognition.translation)
	var textData = $AudioRecognition.translation
	$Label.text = str("Esto fue lo que se obtuvo: ", textData)
	
	
func mixStereoToMono(input: PackedByteArray) -> PackedByteArray:
	if input.size() % 4 == 0:
		var output: PackedByteArray = PackedByteArray()
		output.resize(input.size() / 2)
		var output_index = 0

		for n in range(0, input.size(), 4):
			var left_channel = int(input[n] | (input[n + 1] << 8))
			var right_channel = int(input[n + 2] | (input[n + 3] << 8))
			var mixed = (left_channel + right_channel) / 2
			output[output_index] = mixed & 0xFF
			output[output_index + 1] = (mixed >> 8) & 0xFF
			output_index += 2

		return output
	else:
		var output: PackedByteArray = PackedByteArray()
		output.resize(24)
		return output


func _on_button_toggled(toggled_on: bool) -> void:
	if toggled_on:
		if !microphoneRecord.is_recording_active(): 
			microphoneRecord.set_recording_active(true)
		button.modulate = Color.RED
	else:
		button.modulate = Color.WHITE
		microphoneRecord.set_recording_active(false)
		recordedSample = microphoneRecord.get_recording()
		recordedSample.data = mixStereoToMono(recordedSample.data)
		recordedSample.set_stereo(false)
		audioStreamPlayer.stream = recordedSample;
		audioStreamPlayer.play()
		$AudioRecognition.mix_rate = recordedSample.mix_rate
		$AudioRecognition.audio_recognition_from_bytes(recordedSample.data)
		var textData = $AudioRecognition.translation
		$Label.text = str("Esto fue lo que se obtuvo: ", textData)


func copy_folder_to_user(source_folder: String, destination_folder: String):
	var dir = DirAccess.open(source_folder)
	if not dir:
		print("❌ Could not open source folder: ", DirAccess.get_open_error())
		return
	if not DirAccess.dir_exists_absolute(destination_folder):
		DirAccess.make_dir_recursive_absolute(destination_folder)
		
	# List all files and subfolders
	dir.list_dir_begin()
	var file_name = dir.get_next()
	while file_name != "":
		var source_path = source_folder + "/" + file_name
		var destination_path = destination_folder + "/" + file_name
		if dir.current_is_dir():
			# Recursively copy subfolders
			copy_folder_to_user(source_path, destination_path)
		else:
			# Copy individual files
			var file = FileAccess.open(source_path, FileAccess.READ)
			if file:
				var data = file.get_buffer(file.get_length())
				var dest_file = FileAccess.open(destination_path, FileAccess.WRITE)
				if dest_file:
					dest_file.store_buffer(data)
					print("✅ Copied:", source_path, "->", destination_path)
				else:
					print("❌ Could not write to:", destination_path)
			else:
				print("❌ Could not read file:", source_path)
		
		file_name = dir.get_next()
		# Get next file/folder
	dir.list_dir_end()
