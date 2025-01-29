extends Node2D


func _on_button_pressed() -> void:
	var test = preload("res://bin/test_comando.wav")
	print(test.mix_rate)
	$AudioRecognition.mix_rate = test.mix_rate
	$Label2.text = str("esto es un text con la ddd: ", $AudioRecognition.mix_rate)
	$AudioRecognition.audio_recognition("res://bin/test_comando.wav")
	var textData = JSON.parse_string($AudioRecognition.translation)
	$Label.text = str("esto es un text con la translation: ", textData.text)

func _on_test()-> void:
	print('test')
	$Label2.text = "estoy testeando esto"
