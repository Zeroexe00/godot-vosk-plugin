extends Node2D


func _on_button_pressed() -> void:
	if not OS.has_feature("editor") and OS.get_name() == "Android":
		OS.request_permissions()
	copy_folder_to_user("res://model/", "user://model/")
	var test = preload("res://test_comando.wav")
	var a = test.save_to_wav("user://test_comando.wav")
	print('resultados:', a)
	if FileAccess.file_exists("user://test_comando.wav"):
		print('existe')
	else:
		print("no existe")
	$AudioRecognition.mix_rate = test.mix_rate
	$Label2.text = str("esto es un text con la ddd: ", $AudioRecognition.mix_rate)
	$AudioRecognition.model_path = "user://model"
	$AudioRecognition.audio_recognition("user://test_comando.wav")
	print($AudioRecognition.translation)
	var textData = JSON.parse_string($AudioRecognition.translation)
	$Label.text = str("esto es un text con la translation: ", textData)


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
