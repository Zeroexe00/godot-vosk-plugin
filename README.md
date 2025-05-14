# Godot Vosk Plugin

## Introducción
Este es un wrapper/plugin de VOSK para Godot usando GDExtension para hacerlo compatible con el lenguaje gscript. VOSK es un toolkit para el reconocimiento del habla permitiendonos procesar archivos de audio
en formato WAV o tambien pudiendo usarlo para procesar audio capturado del microfono usando los modelos entrenados y disponibles en la misma pagina de VOSK.

- [VOSK](https://alphacephei.com/vosk/)
- [Modelos disponibles](https://alphacephei.com/vosk/models)  

## Uso
### Proyecto de ejemplo
El proyecto de ejemplo contiene:
- Una escena 2D con
   - 2 botones:
     - Iniciar grabación
     - Archivo .wav
   - 2 nodos AudioStreamPlayer:
     - Uno para capturar audio desde el micrófono.
     - Otro para reproducir el audio grabado.
   - Una etiqueta para mostrar el texto reconocido.
   - Un nodo AudioRecognition que maneja la configuración y procesamiento del audio.


### Versiones compatibles:
| Version | Compatibility |
| ------ | --- |
| 4.3  | ✅ |
| 4.4  | ✅ |
| 4.4.1  | ✅ |

### Compilación
Este modulo necesita compilarse junto con el submodulo godot-cpp usando la herramiento SCons. Ademas
es necesario seguir estas instrucciones para poder compilar el plugin y esa funcional en el editor de Godot
[Guía de compilacion Godot](https://docs.godotengine.org/en/stable/contributing/development/compiling/index.html)

### Comando de compilacion:
```
scons -j<hilos> platform=<platforma>
```
- platform=<platform>: Plataforma a la que necesitamos la compilación
- -j<hilos>: Cantidad de hilos que puede usar SCons para la compilacion (opcional)
