extends Node2D


func _on_button_pressed() -> void:
	$AudioRecognition.audio_recognition("res://bin/call-0.wav")
	var textData = JSON.parse_string($AudioRecognition.translation)
	$Label.text = str("esto es un text con la translation: ", textData.text)
