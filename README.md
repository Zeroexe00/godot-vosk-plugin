# Godot Vosk Plugin

Este es un wrapper/plugin de VOSK para Godot usando GDExtension para hacerlo compatible con el lenguaje gscript. VOSK es un toolkit para el reconocimiento del habla permitiendonos procesar archivos de audio
en formato WAV o tambien pudiendo usarlo para procesar audio capturado del microfono usando los modelos entrenados y disponibles en la misma pagina de VOSK.

- [VOSK](https://alphacephei.com/vosk/)
- [Modelos disponibles](https://alphacephei.com/vosk/models)  

## Atajos
- [Instalación](#instalacion)
- [Proyecto de ejemplo](#proyecto-de-ejemplo)
- [Compilación](#compilacion)

## Uso
### Instalación
Este plugin necesita como minimo la version 4.3 de Godot o superior. Copiar los contenidos de la carpeta addons en la carpeta addons de tu proyecto de godot


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


### Versiones de Godot compatibles:
| Version | Compatibility |
| ------ | --- |
| 4.3  | ✅ |
| 4.4  | ✅ |
| 4.4.1  | ✅ |


### Plataformas compatibles:
| Plataforma | Compatibility |
| ------ | --- |
| Windows 10/11  | ✅ |
| Linux  | ✅ |
| Android  | ✅ |
| Macos  | ❌ |


### Compilación
Este modulo necesita compilarse junto con el submodulo godot-cpp usando la herramiento SCons. Ademas
es necesario seguir estas instrucciones para poder compilar el plugin y esa funcional en el editor de Godot
[Guía de compilacion Godot](https://docs.godotengine.org/en/stable/contributing/development/compiling/index.html)


### Comando de compilacion:
```
scons -j<hilos> platform=<platforma>
```
- ```platform=<platform>```: Plataforma a la que necesitamos la compilación
- ```-j<hilos>```: Cantidad de hilos que puede usar SCons para la compilacion (opcional)
