@tool
extends EditorPlugin


func _enter_tree() -> void:
	print("AudioRecognition Enable")


func _exit_tree() -> void:
	print("AudioRecognition Disable")
