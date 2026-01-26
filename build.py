import os
import subprocess
import sys

def build():
    # Configuration
    source_file = "attracteurs.cpp"
    output_name = "attracteurs_app"
    compiler = "g++"
    cpp_standard = "-std=c++17"
    
    # Bibliothèques SFML nécessaires
    sfml_libs = ["-lsfml-graphics", "-lsfml-window", "-lsfml-system"]
    
    # Adaptation selon l'OS
    if sys.platform == "win32":
        output_name += ".exe"
        # Ajoutez ici les chemins si SFML n'est pas dans les variables d'environnement
        # include_dir = "-I C:/SFML/include"
        # lib_dir = "-L C:/SFML/lib"
    else:
        # Sur Linux/macOS, on suppose que SFML est installé via le gestionnaire de paquets
        include_dir = ""
        lib_dir = ""

    print(f"--- Compilation de {source_file} ---")
    
    # Construction de la commande de compilation
    command = [
        compiler, 
        cpp_standard, 
        source_file, 
        "-o", output_name,
        "-O3" # Optimisation maximale pour la fluidité des calculs
    ]
    
    if include_dir: command.append(include_dir)
    if lib_dir: command.append(lib_dir)
    command.extend(sfml_libs)

    # Exécution
    try:
        result = subprocess.run(command, check=True, text=True, capture_output=True)
        print(f"Compilation réussie ! Exécutable créé : {output_name}")
        print("\nPour lancer l'application :")
        if sys.platform == "win32":
            print(f" ./{output_name}")
        else:
            print(f" ./{output_name}")
            
    except subprocess.CalledProcessError as e:
        print("Erreur de compilation :")
        print(e.stderr)

if __name__ == "__main__":
    build()
